/*************************************
*				   				     *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer: Matan                  *
*	Date: 03/01/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: Schedualer   	   	     *
*				                 	 *
**************************************/

#include <stdlib.h>		/* malloc, free */
#include <unistd.h>		/* sleep */
#include <assert.h>		/* assert */
#include <time.h>	     /* time_t */

#include "priority_queue.h"   /* ilrd_pq_t */
#include "uid.h"       	 	/* ilrd_uid_t */
#include "task.h"			/* task_t */
#include "scheduler.h"		/* scheduler */

struct scheduler
{
	ilrd_pq_t *pq;
	int stop_status;
}; 

enum is_run
{
	STOP,
	RUN
};
enum boolean
{
	TRUE = 1,
	FALSE = 0
};

enum return_status
{
	FAIL = 0,
	SUCCESS
};

static int CmpFuncRunTime(const void *curr_task, const void *new_task);
static int IsMatchUID(const void *task, void *uid);
static void WaitUntilExeTime(task_t *task);
static ilrd_pq_t *SchedToPQ(scheduler_t *sched);

/*------------------------------ SchedCreate ---------------------------------*/

scheduler_t *SchedCreate(void)
{
	scheduler_t *scheduler = NULL;
	
	scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	 
	scheduler->pq = PQCreate(&CmpFuncRunTime);
	if (NULL == SchedToPQ(scheduler))
	{
		free(scheduler);
		scheduler = NULL;
		
		return NULL;
	}
	
	scheduler->stop_status = STOP;
	
	return scheduler;
}

/*----------------------------- SchedDestroy ---------------------------------*/

void SchedDestroy(scheduler_t *sched)
{
	assert(sched);
	
	SchedClearTasks(sched);
	PQDestroy(SchedToPQ(sched));
	
	free(sched);
	sched = NULL;
}

/*----------------------------- SchedAddTask ---------------------------------*/

ilrd_uid_t SchedAddTask(scheduler_t *sched, sched_action_func_t action_func, 
					 void *action_param, sched_cleanup_func_t cleanup_func, 
								   void *cleanup_param, time_t run_time)
{
	task_t *new_task = NULL;
	ilrd_uid_t new_uid = bad_uid;
	int status_PQEnqueue = 0;
	
	assert(sched);
	assert(SchedToPQ(sched));
	assert(action_func);
	assert(cleanup_func);
	assert(-1 != run_time);
	
	new_task = TaskCreate(action_func, action_param, cleanup_func, 
										     cleanup_param, run_time);
	if (NULL != new_task)
	{
		status_PQEnqueue = PQEnqueue(SchedToPQ(sched), new_task);
		if (1 == status_PQEnqueue)
		{
			TaskDestroy(new_task);
			return bad_uid;
		}
		
		new_uid = TaskGetUID(new_task);
	}
	
	return new_uid;
}

/*---------------------------- SchedRemoveTask -------------------------------*/

int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t uid)
{
	task_t *task_to_remove = NULL;
	int status = FALSE;
	
	assert(sched);
	assert(SchedToPQ(sched));
	
	task_to_remove = PQErase(SchedToPQ(sched), &uid, &IsMatchUID);
	if (NULL != task_to_remove)
	{
		TaskDestroy(task_to_remove);
		status = TRUE;
	}

	return status;
}

/*------------------------------- SchedRun -----------------------------------*/

int SchedRun(scheduler_t *sched)
{
	task_t *curr_task = NULL;
	ilrd_uid_t curr_uid = bad_uid;
	int exec_status = FAIL;
	int status = FAIL;
	
	assert(sched);
	
	sched->stop_status = RUN;
	
	while ((RUN == sched->stop_status) && !PQIsEmpty(SchedToPQ(sched)))
	{
		curr_task = (task_t *)PQPeek(SchedToPQ(sched));
		curr_uid = TaskGetUID(curr_task);
	
		WaitUntilExeTime(curr_task);
		exec_status = TaskExecuteFunc(curr_task);
		
		if (0 < exec_status)
		{
			PQErase(SchedToPQ(sched), &curr_uid, &IsMatchUID);
			exec_status += time(NULL);
			TaskSetRunTime(curr_task, exec_status);
			PQEnqueue(SchedToPQ(sched), curr_task);
		}
		else if (0 > exec_status)
		{
			status += 1;
			SchedRemoveTask(sched, curr_uid);
		}
		else 
		{
			SchedRemoveTask(sched, curr_uid);
		}
	}
	return status;
}

/*------------------------------- SchedStop ----------------------------------*/

void SchedStop(scheduler_t *sched)
{
	assert(sched);
	
	sched->stop_status = STOP;
}

/*------------------------------- SchedIsEmpty -------------------------------*/

int SchedIsEmpty(const scheduler_t *sched)
{
	assert(sched);
	
	return (PQIsEmpty(SchedToPQ((scheduler_t *)sched)));
}

/*--------------------------------- SchedSize --------------------------------*/

size_t SchedSize(const scheduler_t *sched)
{
	assert(sched);
	
	return (PQSize(SchedToPQ((scheduler_t *)sched)));
}

/*------------------------------ SchedClearTasks -----------------------------*/

void SchedClearTasks(scheduler_t *sched)
{
	assert(sched);
	
	while (!SchedIsEmpty(sched))
	{
		SchedRemoveTask(sched, TaskGetUID((task_t *)PQPeek(SchedToPQ(sched))));
	}
}


/*--------------------------- HELPER FUNCTIONS -------------------------------*/

/*---------------------------- CmpFuncRunTime --------------------------------*/

static int CmpFuncRunTime(const void *curr_task, const void *new_task)
{
	assert(curr_task);
	assert(new_task);
	
	return(TaskGetRunTime((task_t *)curr_task) - 
								    TaskGetRunTime((task_t *)new_task));
}

/*------------------------------- SetTask ------------------------------------*/

static int IsMatchUID(const void *task, void *uid)
{
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};

	assert(task);
	assert(uid);
	
	uid1 = TaskGetUID((task_t *)task);
	uid2 = *(ilrd_uid_t *)uid;
	
	return (UIDIsEqual(uid1, uid2));
}

/*-------------------------- WaitUntilExeTime --------------------------------*/

static void WaitUntilExeTime(task_t *task)
{	
	time_t time_to_wait = 0;
	time_t curr_time = 0;

	assert(task);

	curr_time = time(NULL);
	time_to_wait = (TaskGetRunTime(task) - curr_time);
    
	if ((time_to_wait > 0) && (curr_time > -1)) 
	{		
		while (0 < sleep(time_to_wait))		
		{
		}
    }
}

/*------------------------------ SchedToPQ -----------------------------------*/

static ilrd_pq_t *SchedToPQ(scheduler_t *sched)
{
	return (sched->pq);
}

