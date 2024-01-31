/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Eliran	              *
*	Date: 06/03/23	                  *
*	Subject: Watchdog				  *
*				                	  *
**************************************/
#define _GNU_SOURCE

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <stdlib.h>		/* fork */
#include <unistd.h>		/* pid_t, execl */
#include <sys/wait.h>	/* waitpid */
#include <pthread.h>	/* create, join */
#include <semaphore.h>	/* sem_t, sem_init */
#include <fcntl.h> 		/* O_CREAT */
#include <sys/sem.h>	/* sem_open */
#include <signal.h>		/* kill */

#include "scheduler.h"	/* scheduler API */
#include "watchdog.h" 	/* watchdog API */

#define SEND_SIG_INTERVAL (1)	
#define	CHECK_SIG_INTERVAL (5)
#define IS_CHILD_PROC(pid) (0 == pid)
#define SEM_NAME ("/sem_wd")
#define APP_EXE_PATH ("../watchdog/a.out")

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

state_t g_state = DEAD;

sem_t *g_sem = NULL;
pid_t g_app_pid = 0;

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/

static void SigHandler(int signal);
static int InitSigHandler(void);
static int InitSchedAndTasks(args_t **task_args, char **argv);

static int TaskSendSig(void *param);
static int TaskCheckSig(void *param);

static void TaskSendCleanUp(void *param);
static void TaskCheckCleanUp(void *param);

static int RestartAPP(args_t *args);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
int main(int argc, char **argv)
{
	args_t *task_args = NULL;
	const char *env_var = "WD_VAR_ENV";
	
	assert(NULL != argv);
	assert(0 < argc);
	
	setenv(env_var, "exist", 0);    
	
	g_app_pid = getppid();
	
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
		return FAIL;
	}
	
	if (SUCCESS != sem_post(g_sem))
	{
		return FAIL;
	}
	
	SchedRun(((args_t *)task_args)->sched);
	SchedDestroy(((args_t *)task_args)->sched);
	
	free(task_args);
	task_args = NULL;
	
	sem_close(g_sem);
	
	return 0;
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
	struct sigaction sig_handler = {0};
	sig_handler.sa_handler = &SigHandler;
	
	if (SUCCESS != sigaction(SIGUSR1, &sig_handler, NULL) ||
		SUCCESS != sigaction(SIGUSR2, &sig_handler, NULL))
	{
		 return FAIL;
	}
	
	return SUCCESS;
}

/*------------------------------ InitSchedTasks ------------------------------*/

static int InitSchedAndTasks(args_t **task_args, char **argv)
{
	scheduler_t *sched = NULL;
	ilrd_uid_t task_send = {0};
	ilrd_uid_t task_check = {0};

	assert(NULL != task_args);
	
	*task_args = (args_t *)malloc(sizeof(args_t));
	if (NULL == task_args)
	{
		SchedDestroy((*task_args)->sched);
		(*task_args)->sched = NULL;
		
		return FAIL;
	}
	
	(*task_args)->sched = SchedCreate();
	if (NULL == (*task_args)->sched)
	{
		return FAIL;
	}
	
	(*task_args)->argv = (char **)argv;
	sched = (*task_args)->sched;
 
 	task_send = SchedAddTask(sched, &TaskSendSig, NULL, &TaskSendCleanUp, 
															  NULL, time(NULL));
								
	task_check = SchedAddTask(sched, &TaskCheckSig, *task_args, &TaskCheckCleanUp,
										 NULL, time(NULL) + CHECK_SIG_INTERVAL);				
								
	if (UIDIsEqual(task_send, bad_uid) || UIDIsEqual(task_check, bad_uid))
	{
		return FAIL;
	}
	
	return SUCCESS;
}


/*----------------------------- TaskSendCleanUp ------------------------------*/

static void TaskSendCleanUp(void *param)
{
	(void)param;
	
	return;
}

/*----------------------------- TaskCheckCleanUp -----------------------------*/

static void TaskCheckCleanUp(void *param)
{
	(void)param;
	
	return;
}

/*-------------------------------- TaskSendSig -------------------------------*/

static int TaskSendSig(void *param)
{
	(void)param;
	kill(g_app_pid, SIGUSR1);
	
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
			if (FAIL == RestartAPP(args))
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

/*--------------------------------- RestartAPP -------------------------------*/
 
static int RestartAPP(args_t *args)
{
	g_app_pid = fork();
	
	if (FAIL == g_app_pid)
	{
		return FAIL;
	}
	else if (IS_CHILD_PROC(g_app_pid) && 
			(SUCCESS != execvp(APP_EXE_PATH, args->argv)))
	{
		return FAIL;
	}
	
	sem_wait(g_sem);
		
	return SUCCESS;
}


