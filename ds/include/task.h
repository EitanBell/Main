/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Task   			  *
*				                 *
**************************************/

/*------------------------------------------------------------------------------
TASK:
	 Task is a program or parts of it that can be executed at a specific time.

MAIN FUNCTIONALITY:
	TaskCreate, TaskDestroy, TaskGetRunTime, TaskSetRunTime, TaskExecuteFunc,
	TaskGetUID.
------------------------------------------------------------------------------*/

#ifndef __TASK_H__
#define __TASK_H__

#include <time.h> /* time_t */

#include "uid.h" /* ilrd_uid_t */

/*-----------------------------------------------------------------------------
Description: 
	The task_action_func_t is a pointer to function to run with param when
	TaskExecuteFunc.
	NOTE: task_action_func_t pointing to NULL will cause an undifined behavior.
	
Parameters:
	param - Pointer to paramter to do action with.
	
Return Value:
	Return the number of second that task need to TaskExecuteFunc again.
	In fail return negative integer.
-----------------------------------------------------------------------------*/
typedef int(*task_action_func_t)(void *param);

/*-----------------------------------------------------------------------------
Description: 
	The task_cleanup_func_t is a pointer to function to run when TaskDestroy.
	NOTE: task_cleanup_func_t pointing to NULL will cause an undifined behavior.
	
Parameters:
	param - Pointer to paramter to cleanup with.
	
Return Value:
	None.
-----------------------------------------------------------------------------*/
typedef void(*task_cleanup_func_t)(void *param);  

/*-----------------------------------------------------------------------------
Description: 
	task_t is the handel to work with. 
-----------------------------------------------------------------------------*/
typedef struct task task_t;

/*-----------------------------------------------------------------------------
Description: 
	Creating new Task with the parameters that received.
	Resources for the Task is allocated,
	and can be freed with TaskDestroy.
	
Parameters:
	action_func - Pointer to action function.
	action_param - Pointer to paramter to use with action_func.
	cleanup_func - Pointer to cleanup function.
	cleanup_param - Pointer to paramter to use with cleanup_func.
	run_time - time to TaskExecuteFunc in second.
	
Return Value:
	Return a pointer to the new Task, or NULL in fail.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
task_t *TaskCreate(task_action_func_t action_func, void *action_param,
                   task_cleanup_func_t cleanup_func, void *cleanup_param,
                   time_t run_time);

/*-----------------------------------------------------------------------------
Description: 
	Cleanup and destroy a task.
	free the resources that was allocated by TaskCreate.
	
Parameters:
	task - Pointer to the Task to be destroyed.
	
Return Value:
	None.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void TaskDestroy(task_t *task);

/*-----------------------------------------------------------------------------
Description: 
	Gets the run time of a given task.

Parameters:
	task - Task to get run time from.

Return Value:
	Return the run time of task.

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
time_t TaskGetRunTime(const task_t *task);

/*-----------------------------------------------------------------------------
Description: 
	Sets a run time for a task.

Parameters:
	task - Task to set run time to.
	run_time - run time (in second) to set in the given task.

Return Value:
	None.

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
void TaskSetRunTime(task_t *task, time_t run_time);


/*-----------------------------------------------------------------------------
Description: 
	Execute task with action_func and param.
	
Parameters:
	task - Task to execute.
	
Return Value:
	return The status of the action function - 0 on success,
	 negative value on fail.
	 
Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
int TaskExecuteFunc(const task_t *task);

/*-----------------------------------------------------------------------------
Description: 
	Bring the Unique ID of a task.

Parameters:
	task - Task to get UID from.

Return Value:
	Return the UID of a task.

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
ilrd_uid_t TaskGetUID(const task_t *task);

#endif /* __TASK_H__ */


