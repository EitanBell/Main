/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Eliran	              *
*	Date: 06/03/23	                  *
*	Subject: Watchdog				  *
*				                	  *
**************************************/
#define _GNU_SOURCE

#include <assert.h>		/* assert */
#include <stdlib.h>		/* fork */
#include <unistd.h>		/* pid_t, execl */
#include <pthread.h>	/* create, join */
#include <semaphore.h>	/* sem_t, sem_init */
#include <fcntl.h> 		/* O_CREAT */
#include <signal.h>		/* sigaction, kill */

#include "scheduler.h"	/* scheduler API */
#include "watchdog.h" 	/* watchdog API */

#define SEND_SIG_INTERVAL (1)	
#define	CHECK_SIG_INTERVAL (5)
#define IS_PARENT_PROC(pid) (0 < pid)
#define IS_CHILD_PROC(pid) (0 == pid)
#define ENV_NAME ("/env_wd")
#define SEM_NAME ("/sem_wd")
#define WD_EXE_PATH ("../WD_PROC/a.out")
#define IS_WD_ALIVE (NULL != getenv("WD_VAR_ENV"))

enum status
{
	FAIL = -1,
	SUCCESS
};

typedef enum state
{
	DEAD = 0,
	ALIVE,
	STOP
}state_t;

typedef struct args
{
	scheduler_t *sched;
	char **argv;	
}args_t;

/*------------------------------ GLOBAL VARIABLES ----------------------------*/

static state_t g_state = DEAD;

static sem_t *g_sem = NULL;
static pid_t g_wd_pid = 0;
static pthread_t g_app_thread = {0};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/

static void SigHandler(int signal);
static int InitSigHandler(void);

static int InitSchedAndTasks(args_t **task_args, const char **argv);

static int TaskSendSig(void *param);
static int TaskCheckSig(void *param);

static void TaskCleanUp(void *param);

static void *start_routine(void *args);
static int RestartWD(args_t *args);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*---------------------------------- WDStart ---------------------------------*/

int WDStart(int argc, const char **argv)
{
	int status = SUCCESS;
	args_t *task_args = NULL;
	const char *env_var = "WD_VAR_ENV";
	
	assert(NULL != argv);
	(void)argc;
	
	
	if (SUCCESS != InitSigHandler())
	{
		return FAIL;
	}

	g_sem = sem_open(SEM_NAME, O_CREAT, S_IRWXO | S_IRWXU, 0);
	if (SEM_FAILED == g_sem)
	{
		return FAIL;
	}
	
	if (FAIL == InitSchedAndTasks(&task_args, argv))
	{
		sem_close(g_sem);
		
		return FAIL;
	}

	if (!IS_WD_ALIVE)
	{
		RestartWD(task_args);
	}
	else
	{
		g_wd_pid = getppid();
		
		if (SUCCESS != sem_post(g_sem))
		{
			return FAIL;
		}
	}
	while (0 != pthread_create(&g_app_thread, NULL, &start_routine, task_args)){};
	
	
	unsetenv(env_var);
	return status;	
}


/*--------------------------------- WDStop -----------------------------------*/

void WDStop(void)
{
	size_t i = 0;
	
	for (i = 0; CHECK_SIG_INTERVAL > i; ++i)
	{
		kill(g_wd_pid, SIGUSR2);
		kill(getpid(), SIGUSR2);		
	}
	
	pthread_join(g_app_thread, NULL);
	sem_unlink("/sem_wd");
	sem_close(g_sem);
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*-------------------------------- SigHandler --------------------------------*/

static void SigHandler(int signal)
{
	if ((SIGUSR1 == signal) && (STOP != g_state))
	{
		g_state = ALIVE;
	}
	else if (SIGUSR2 == signal)
	{
		g_state = STOP;
	}
}

/*------------------------------ InitSigHandler ------------------------------*/

static int InitSigHandler(void)
{
	int status = SUCCESS;
	struct sigaction sig_handler = {0};
	
	sig_handler.sa_handler = &SigHandler;
	
	status = (FAIL == sigaction(SIGUSR1, &sig_handler, NULL));
	status += (FAIL == sigaction(SIGUSR2, &sig_handler, NULL));
	
	return status;
}

/*------------------------------ InitSchedTasks ------------------------------*/

static int InitSchedAndTasks(args_t **task_args, const char **argv)
{
	scheduler_t *sched = NULL;
	ilrd_uid_t task_send = {0};
	ilrd_uid_t task_check = {0};

	assert(NULL != task_args);
	
	*task_args = (args_t *)malloc(sizeof(args_t));
	if (NULL == task_args)
	{
		return FAIL;
	}
	
	(*task_args)->sched = SchedCreate();
	if (NULL == (*task_args)->sched)
	{
		free(*task_args);
		*task_args = NULL;
		
		return FAIL;
	}
	
	(*task_args)->argv = (char **)argv;
	sched = (*task_args)->sched;
 
 	task_send = SchedAddTask(sched, &TaskSendSig, NULL, &TaskCleanUp, 
															  NULL, time(NULL));
								
	task_check = SchedAddTask(sched, &TaskCheckSig, *task_args, &TaskCleanUp,
										 NULL, time(NULL) + CHECK_SIG_INTERVAL);				
								
	if (UIDIsEqual(task_send, bad_uid) || UIDIsEqual(task_check, bad_uid))
	{
		SchedDestroy(((args_t *)task_args)->sched);
		free(task_args);
		task_args = NULL;
		
		return FAIL;
	}
	
	return SUCCESS;
}


/*------------------------------- TaskCleanUp --------------------------------*/

static void TaskCleanUp(void *param)
{
	(void)param;
	
	return;
}

/*-------------------------------- TaskSendSig -------------------------------*/

static int TaskSendSig(void *param)
{
	(void)param;
	
	kill(g_wd_pid, SIGUSR1);
	return SEND_SIG_INTERVAL;
}

/*------------------------------- TaskCheckSig -------------------------------*/

static int TaskCheckSig(void *param)
{
	args_t *args = NULL;

	args = (args_t *)param;
	switch (g_state)
	{
		case ALIVE:
		{
			g_state = DEAD;
			break;
		}
		case STOP:
		{
			SchedStop(args->sched);
			break;
		}
		case DEAD:
		{
			if (FAIL == RestartWD(args))
			{
				SchedStop(args->sched);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	
	return CHECK_SIG_INTERVAL;
}

/*------------------------------ start_routine -------------------------------*/

static void *start_routine(void *args)
{
	assert(NULL != args);
	
	SchedRun(((args_t *)args)->sched);
	SchedDestroy(((args_t *)args)->sched);
	
	free(args);
	args = NULL;
	
	pthread_exit(NULL);	
}

/*---------------------------------- RestartWD --------------------------------*/

static int RestartWD(args_t *args)
{
	g_wd_pid = fork();
	
	if (FAIL == g_wd_pid)
	{
		return FAIL;
	}
	else if (IS_CHILD_PROC(g_wd_pid) && 
			(SUCCESS != execvp(WD_EXE_PATH, args->argv)))
	{
		return FAIL;
	}
	
	sem_wait(g_sem);
	g_state = ALIVE;
		
	return SUCCESS;
}

