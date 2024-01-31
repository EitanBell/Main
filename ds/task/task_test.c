/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: task   			  *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */

#include "task.h" /* task */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m": "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

void RunTests(void);

static int TaskCreateDestroyTest(void);
static int DoAction(void *param);
static void CleanUp(void *param);
static int TaskGetRunTimeTest(void);
static int TaskSetRunTimeTest(void);
static int TaskGetUIDTest(void);
static int TaskExecuteFuncTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n******************** TESTS BEGIN *******************\n\n");
	
	printf("\tTaskCreate Test: \t%s\n", TEST_RES(TaskCreateDestroyTest()));
	printf("\tTaskDestroy Test: \t%s\n", TEST_RES(TaskCreateDestroyTest()));
	printf("\tTaskGetRunTime Test: \t%s\n", TEST_RES(TaskGetRunTimeTest()));
	printf("\tTaskSetRunTime Test: \t%s\n", TEST_RES(TaskSetRunTimeTest()));
	printf("\tTaskGetUID Test: \t%s\n", TEST_RES(TaskGetUIDTest()));
	printf("\tTaskExecuteFunc Test: \t%s\n", TEST_RES(TaskExecuteFuncTest()));


	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int TaskCreateDestroyTest(void)
{
	int status1 = 0;
	int status2 = 0;
	task_t *task = NULL;
	time_t run_time = 12;
	int action_param = 5;
	int clean_up_param = 17;
	
	task = TaskCreate(&DoAction, &action_param, &CleanUp, &clean_up_param, 
	run_time);
	
	if (NULL == task)
	{
		status1 = 1;
	}
	
	TaskDestroy(task);
	if (NULL != task)
	{
		status2 = 1;
	}
	return status1 && status2;
}

static int TaskGetRunTimeTest(void)
{
	int status = 0;
	task_t *task = NULL;
	time_t run_time = 30;
	int action_param = 21;
	int clean_up_param = 21;
	
	task = TaskCreate(&DoAction, &action_param, &CleanUp, &clean_up_param, 
	run_time);

	status = (TaskGetRunTime(task) == 30);
	
	TaskDestroy(task);
	
	return !status;
}

static int TaskSetRunTimeTest(void)
{
	int status = 0;
	task_t *task = NULL;
	time_t run_time = 30;
	int action_param = 21;
	int clean_up_param = 21;
	
	task = TaskCreate(&DoAction, &action_param, &CleanUp, &clean_up_param, 
	run_time);

	TaskSetRunTime(task, 100);
	
	status = (TaskGetRunTime(task) == 100);
	
	TaskDestroy(task);
	
	return !status;
}


static int TaskGetUIDTest(void)
{
	int status = 0;
	task_t *task = NULL;
	time_t run_time = 30;
	int action_param = 21;
	int clean_up_param = 21;
	
	task = TaskCreate(&DoAction, &action_param, &CleanUp, &clean_up_param, 
	run_time);
	
	status = UIDIsEqual(bad_uid, TaskGetUID(task));
	
	TaskDestroy(task);
	
	return status;
}

static int TaskExecuteFuncTest(void)
{
	int status = 0;
	task_t *task = NULL;
	time_t run_time = 30;
	int action_param = 700;
	int clean_up_param = 23;
	
	task = TaskCreate(&DoAction, &action_param, &CleanUp, &clean_up_param, 
	run_time);

	status = (TaskExecuteFunc(task) == 702);
	
	TaskDestroy(task);
	
	return !status;
}

/*--------------------------- HELPER FUNCTIONS -------------------------------*/

static int DoAction(void *param)
{
	return (*(int *)param) += 2;
}

static void CleanUp(void *param)
{
	*(int *)param = 0;
}


