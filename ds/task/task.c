/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: task   			  *
*				                 *
**************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "task.h" /* my_task */

struct task 
{
	task_action_func_t action_func;
	void *action_param;
	
	task_cleanup_func_t cleanup_func;
	void *cleanup_param;
	
	time_t run_time;

	ilrd_uid_t uid;
};

static void SetTask(task_t *task, task_action_func_t action_func, 
				void *action_param, task_cleanup_func_t cleanup_func,
				      void *cleanup_param, time_t run_time, ilrd_uid_t uid);

/*---------------------------- SortListCreate --------------------------------*/

task_t *TaskCreate(task_action_func_t action_func, void *action_param,
                   task_cleanup_func_t cleanup_func, void *cleanup_param,
                   									    time_t run_time)	
{
	task_t *new_task = NULL;
	ilrd_uid_t task_uid = {0};
	
	
	assert(action_func);
	assert(cleanup_func);
	
	task_uid = UIDGenerate();
	if (UIDIsEqual(task_uid, bad_uid))
	{
		return NULL;
	}
	
	new_task = (task_t *)malloc(sizeof(task_t));
	if (NULL == new_task)
	{
		return NULL;
	}
	
	SetTask(new_task, action_func, action_param, cleanup_func, cleanup_param,
				  								run_time, task_uid);
	return new_task;
}

/*------------------------------ TaskDestroy ---------------------------------*/

void TaskDestroy(task_t *task)
{
	task->cleanup_func(task->cleanup_param);
	
	free(task);
	task = NULL;
}

/*----------------------------- TaskGetRunTime -------------------------------*/

time_t TaskGetRunTime(const task_t *task)
{
	return (task->run_time);
}

/*----------------------------- TaskSetRunTime -------------------------------*/

void TaskSetRunTime(task_t *task, time_t run_time)
{
	assert(task);
	assert(-1 != run_time);
	
	task->run_time = run_time;
}

/*----------------------------- TaskExecuteFunc ------------------------------*/

int TaskExecuteFunc(const task_t *task)
{
	assert(task);
	
	return (task->action_func(task->action_param));
}

/*------------------------------- TaskGetUID ---------------------------------*/

ilrd_uid_t TaskGetUID(const task_t *task)
{
	assert(task);
	
	return (task->uid);
}


/*--------------------------- HELPER FUNCTIONS -------------------------------*/

/*------------------------------- SetTask ------------------------------------*/

static void SetTask(task_t *task, task_action_func_t action_func, 
				void *action_param, task_cleanup_func_t cleanup_func,
				       void *cleanup_param, time_t run_time, ilrd_uid_t uid)
{
	task->action_func = action_func;
	task->action_param = action_param;

	task->cleanup_func = cleanup_func;
	task->cleanup_param = cleanup_param;

	task->run_time = run_time;

	task->uid = uid;
}


