/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 03/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Schedualer   		  *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */
#include <assert.h> /* assert */

#include "scheduler.h" /* scheduler */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
							     "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
enum run_status
{
	STOP = 0,
	RUN = 1
};

FILE *fp = NULL;
int flag_status = 0;

typedef struct test
{
	scheduler_t *my_sched;
	ilrd_uid_t a_uid;
		
}test_t;

test_t sched = {0};

typedef struct stop_test
{
	scheduler_t *my_sched;
	FILE *file;
		
}stop_test_t;

stop_test_t stop_test;

void RunTests(void);
static int SchedCreateAndDestroyTest(void);
static int SchedAddTaskAndRemoveTest(void);
static int SchedClearTasksTest(void);
static int SchedRunTest(void);
static int SchedStopTest(void);
static int SchedTaskRemoveItselfTest(void);

static int DoActionFuncStop(void *param);
static int DoActionFuncRemoveItself(void *param);
static void CleanUpFunc(void *param);
static void CleanUpFuncTaskRemoveItself(void *param);
static int DoActionFunc(void *param);
static int DoActionFunc2(void *param);
static int DoActionFunc3(void *param);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n******************** TESTS BEGIN *******************\n\n");
	
	printf("\t SchedCreateAndDestroy Test: \t%s\n", TEST_RES(SchedCreateAndDestroyTest()));
	printf("\t SchedAddTaskAndRemove Test: \t%s\n", TEST_RES(SchedAddTaskAndRemoveTest()));
	printf("\t SchedClearTasks Test: \t\t%s\n", TEST_RES(SchedClearTasksTest()));
	printf("\t SchedRun Test: \t\t%s\n", TEST_RES(SchedRunTest()));
	printf("\t SchedStop Test: \t\t%s\n", TEST_RES(SchedStopTest()));
	printf("\t SchedTaskRemoveItself Test: \t%s\n", TEST_RES(SchedTaskRemoveItselfTest()));
	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int SchedCreateAndDestroyTest(void)
{
	int status = 0;
	scheduler_t *my_sched = NULL;
	
	my_sched = SchedCreate();
	if (NULL == my_sched)
	{
		status = 1;
	}
	
	SchedDestroy(my_sched);
	return status;
}

static int SchedAddTaskAndRemoveTest(void)
{
	int status = 0;
	time_t run1 = 1;
	time_t run2 = 2;
	char *filename = "test.txt";
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	scheduler_t *my_sched = NULL;
	
	my_sched = SchedCreate();
	if (NULL == my_sched)
	{
		return status;
	}
	
	uid1 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
						(void *)filename, run1);
		   
	uid2 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run2);
				 
	status += (UIDIsEqual(uid1, uid2));
	status += !((size_t)2 == SchedSize(my_sched));
	
	SchedRemoveTask(my_sched, uid1);
	status += !(1 == SchedSize(my_sched));
	
	SchedRemoveTask(my_sched, uid2);
	status += !SchedIsEmpty(my_sched);
	
	SchedDestroy(my_sched);
	return status;

}

static int SchedClearTasksTest(void)
{
	int status = 0;
	time_t run1 = 1;
	time_t run2 = 2;
	char *filename = "test.txt";
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	scheduler_t *my_sched = NULL;
	
	my_sched = SchedCreate();
	if (NULL == my_sched)
	{
		return status;
	}
	
	uid1 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
						(void *)filename, run1);
		   
	uid2 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run2);
				 
	status += (UIDIsEqual(uid1, uid2));
	status += !((size_t)2 == SchedSize(my_sched));
	
	SchedClearTasks(my_sched);
	status += !SchedIsEmpty(my_sched);
	
	SchedDestroy(my_sched);
	return status;
}


static int SchedTaskRemoveItselfTest(void)
{
	int status = 0;
	time_t run1 = 1;
	ilrd_uid_t uid1 = {0};
	
	sched.my_sched = SchedCreate();
	if (NULL == sched.my_sched)
	{
		return status;
	}
	
	sched.a_uid = SchedAddTask(sched.my_sched, &DoActionFuncRemoveItself, &sched,
								&CleanUpFuncTaskRemoveItself, &sched, run1);
						
	status += SchedRun(sched.my_sched);
	status += !SchedIsEmpty(sched.my_sched);
	
	SchedDestroy(sched.my_sched);
	(void)uid1;
	
	return status;
}


static int SchedRunTest(void)
{
	int status = 0;
	time_t run1 = 1;
	time_t run2 = 2;
	time_t run3 = 3;
	char *filename = "test.txt";
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	ilrd_uid_t uid3 = {0};
	scheduler_t *my_sched = NULL;
	
	my_sched = SchedCreate();
	if (NULL == my_sched)
	{
		return status;
	}
	
	uid1 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
						(void *)filename, run1);
		   
	uid2 = SchedAddTask(my_sched, &DoActionFunc2, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run2);
				 		
	uid3 = SchedAddTask(my_sched, &DoActionFunc3, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run3);
	
	status += (UIDIsEqual(uid1, uid2));			 
	status += (UIDIsEqual(uid2, uid3));
	status += (UIDIsEqual(uid1, uid3));
	
	status += SchedRun(my_sched);
	status += !SchedIsEmpty(my_sched);
	
	SchedDestroy(my_sched);
	return status;
}


static int SchedStopTest(void)
{
	int status = 0;
	time_t run1 = 1;
	time_t run2 = 2;
	time_t run3 = 3;
	time_t run4 = 4;
	char *filename = "test.txt";
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	ilrd_uid_t uid3 = {0};
	ilrd_uid_t uid4 = {0};
	scheduler_t *my_sched = NULL;
	
	my_sched = SchedCreate();
	if (NULL == my_sched)
	{
		return status;
	}
	
	uid1 = SchedAddTask(my_sched, &DoActionFunc, (void *)filename, &CleanUpFunc, 
						(void *)filename, run1);
		   
	uid2 = SchedAddTask(my_sched, &DoActionFunc2, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run2);
	
	uid3 = SchedAddTask(my_sched, &DoActionFuncStop, (void *)my_sched
						 , &CleanUpFunc, (void *)my_sched, run3);			 		
	
	uid4 = SchedAddTask(my_sched, &DoActionFunc3, (void *)filename, &CleanUpFunc, 
				 		(void *)filename, run4);
	
	status += (UIDIsEqual(uid1, uid2));			 
	status += (UIDIsEqual(uid2, uid3));
	status += (UIDIsEqual(uid1, uid3));
	status += (UIDIsEqual(uid1, uid4));
	
	status += SchedRun(my_sched);
	status += !(1 == SchedSize(my_sched));
	
	SchedDestroy(my_sched);
	return status;
}



static int DoActionFuncStop(void *param)
{
	int status = 0;
	assert(param);

	SchedStop((scheduler_t *)param);
	return status;
}

static int DoActionFuncRemoveItself(void *param)
{
	int status = -1;
	test_t *sched = NULL;
	
	assert(param);
	
	sched = (test_t *)param;
	status += SchedRemoveTask(sched->my_sched, sched->a_uid);
	
	return status;
}

static void CleanUpFunc(void *param)
{	
	assert(NULL != param);
	
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
	(void)param;
}


static void CleanUpFuncTaskRemoveItself(void *param)
{	
	(void)param;
}

static int DoActionFunc(void *param)
{
	int status = 1;
	const char *str = "New Task Execute\n";
	
	assert(NULL != param);

	fp = fopen((char *)param, "a+");
	if (NULL == fp)
	{
		return -1;
	}
	if (fwrite(str, sizeof(char) , strlen(str), fp))
	{
		status = 0;
	}
	
	return status;
}


static int DoActionFunc2(void *param)
{
	int status = 1;
	const char *str = "New Task Execute2\n";
	
	assert(NULL != param);

	fp = fopen((char *)param, "a+");
	if (NULL == fp)
	{
		return -1;
	}

	if (fwrite(str, sizeof(char) , strlen(str), fp))
	{
		status = 0;
	}
	
	return status;
}


static int DoActionFunc3(void *param)
{
	int status = 1;
	const char *str = "New Task Execute3\n";
	
	assert(NULL != param);

	fp = fopen((char *)param, "a+");
	if (NULL == fp)
	{
		return -1;
	}
	if (fwrite(str, sizeof(char) , strlen(str), fp))
	{
		status = 0;
	}
	
	if (0 == flag_status)
	{
		flag_status = 1;
		status = 4;
		fclose(fp);
		fp = NULL;
		
	}
	return status;
}

