/*************************************
*				   				     *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer: Matan                  *
*	Date: 03/01/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: Schedualer   	   	     *
*				                 	 *
**************************************/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>		/*size_t*/

#include "uid.h"        /* ilrd_uid_t */


/*-----------------------------------------------------------------------------
Description:
	The wrapper for scheduler implementation.
-----------------------------------------------------------------------------*/
typedef struct scheduler scheduler_t;

/*-----------------------------------------------------------------------------
Description:
	A function declaration of the task function to execute, chosen
	by the user.
-----------------------------------------------------------------------------*/
typedef int(*sched_action_func_t)(void *param);

/*-----------------------------------------------------------------------------
Description:
	A function declaration of the cleanup function for the task to 
	execute after the execution function runs, to clear all the resources 
	allocated for the execution function operations.
-----------------------------------------------------------------------------*/
typedef void(*sched_cleanup_func_t)(void *param);  

/*-----------------------------------------------------------------------------
Description:
	The function creates a new empty scheduler.
	
Return Value:
	The function returns a pointer to a new empty scheduler.
	In case of failure, the function returns NULL.

Complexity: 
	time: O(1) / space: O(1)  
-----------------------------------------------------------------------------*/
scheduler_t *SchedCreate(void);

/*-----------------------------------------------------------------------------
Description: 
	The function releases all resources allocated for the scheduler.
	The function clears all remaining tasks in the scheduler. 
	
Parameters:
	sched- A scheduler created by this API.

Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
void SchedDestroy(scheduler_t *sched);

/*-----------------------------------------------------------------------------
Description:
	The function adds a task, according to the user's passed 
	arguments, to the scheduler.  
	
Parameters:
	sched- A scheduler created by this API.
	
	action_func - the task function to be scheduled to run
	(execute).
		Note! The function's return value has to be according to 
		these:
			return_value  = 0: Success
			return_value > 0 Success, reschedule run for the task in 			
			return_value seconds.
			return_value < 0 Failure
			
	action_param - action_func arguments for execution.
			
	cleanup_func - the clean up function to be run after the task 				
					executed function is finished.
					
	cleanup_param - cleanup_func arguments.
	
	run_time - the wanted time for the task to be executed.

	
Return Value:
	The function returns a UID of the new task added to scheduler.

Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
ilrd_uid_t SchedAddTask(scheduler_t *sched, sched_action_func_t action_func, 
						void *action_param, sched_cleanup_func_t cleanup_func, 
						void *cleanup_param, time_t run_time);

/*-----------------------------------------------------------------------------
Description:
	The function removes a task, according to the task's UID.  
	
Parameters:
	sched- A scheduler created by this API.
	
	uid- the wanted to remove task's UID.
	
Return Value:
	The function returns a status.
	If succeded removing the wanted task the function returns 1, if failed the
	function returns 0.

Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t uid);

/*-----------------------------------------------------------------------------
Description:
	The function runs all the tasks that were added to the scheduler in their 
	wanted run time.  
	
Parameters:
	sched- A scheduler created by this API.
	
Return Value:
	The function returns a status.
	If succeded running all the tasks, the function returns 0, if failed in one 
	or more tasks, the function returns the number of failed tasks.
	Note! if a task fails to execute, then the function continues to execute the
	remaining tasks. 

Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
int SchedRun(scheduler_t *sched);

/*-----------------------------------------------------------------------------
Description:
	The function stops the SchedRun function in mid action.
	You can use this in a task for a planned stop, or as a repeated task for 
	mid runtime stop. 
	
Parameters:
	sched- A scheduler created by this API.

Complexity: 
	time: O(1)  / space: O(1)  
-----------------------------------------------------------------------------*/
void SchedStop(scheduler_t *sched); 

/*-----------------------------------------------------------------------------
Description:
	The function checks whether the scheduler is empty- does not have any tasks.
	
Parameters:
	sched- A scheduler created by this API.
	
Return Value:
	The function returns 1 if the scheduler is empty, and 0 if not. 
  
Complexity: 
	time: O(1)  / space: O(1)  
-----------------------------------------------------------------------------*/
int SchedIsEmpty(const scheduler_t *sched);

/*-----------------------------------------------------------------------------
Description:
	The function counts the number of tasks that the scheduler contains.
	
Parameters:
	sched- A scheduler created by this API.
	
Return Value:
	The function returns the number of tasks in scheduler. 
  
Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
size_t SchedSize(const scheduler_t *sched);

/*-----------------------------------------------------------------------------
Description:
	The function removes all of the existing tasks from the scheduler.
	
Parameters:
	sched- A scheduler created by this API.
  
Complexity: 
	time: O(n)  / space: O(1)  
-----------------------------------------------------------------------------*/
void SchedClearTasks(scheduler_t *sched);


#endif 	/* __SCHEDULER_H__ */

