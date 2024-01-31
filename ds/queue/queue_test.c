/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Moriya               *
*	Date: 07/12/22	                 *
*	Vlagrind & gdb passed           *
*	Subject: Queue	 			  *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <string.h> /* memcmp */

#include "queue.h"  

#define CHECK_MARK ("\t\033[0;32m\xE2\x9C\x93\033[0m\n")
#define FAIL_MARK ("\t\033[0;31m\xE2\x9C\x97\033[0m\n")

typedef enum
{
	SUCCESS = 0,
	FAIL

} status_t;

/****************	test's prints functions declartion	***********************/

void PrintUnitTitle(char *func_name);
void PrintTestTitle(char *tests_name);
void PrintCheckStat(int stat);
void PrintCheckTitle(char *tests_name, size_t test_num);
void PrintUnitStat(int stat, char *func_name);
void PrintTestStat(int stat, char *tests_name);
int SlistIsEmptyTest(void);

/****************		tests functions declartion		******************/

void QueueBlackBoxTests(void);

int UnitCreateIsEmptyDestroyTest(void);
int UnitEnqueueSizeDequeueTest(void);
int UnitPeekTest(void);
int UnitAppendTest(void);
int UnitPeekTest(void);
int UnitAppendTest(void);

int TestIsEmpyty(char *test_name, size_t test_num, queue_t *queue, int num);
int TestSize(char *test_name, size_t test_num, queue_t *queue, size_t num);
int TestPeek(char *test_name, size_t test_num, queue_t *queue, void *num,
		   size_t size);

/***********************		main		***********************************/

int main()
{
	QueueBlackBoxTests();

	return 0;
}

void QueueBlackBoxTests(void)
{
	char tests_name[] = "Queue";
	status_t stat = SUCCESS;
	
	PrintTestTitle(tests_name);
	
	stat += UnitCreateIsEmptyDestroyTest();
	stat += UnitEnqueueSizeDequeueTest();
	stat += UnitPeekTest();
	stat += UnitAppendTest();
	
	PrintTestStat(stat, tests_name);
	
	if (1 == SlistIsEmptyTest())
	{
		printf("SlistIsEmpty test SUCCEDDED\n");
	}
	else
	{
		printf("SlistIsEmpty test FAILED\n");
	}
}

int UnitCreateIsEmptyDestroyTest(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Create IsEmpty Destroy";
	char *test_name = "IsEmpty Create";
	const size_t TEST_NUM = 10;
	size_t i = 0;
	queue_t *queue[10] = {0};
	
	PrintUnitTitle(unit_name);
	
	for (i = 0; i < TEST_NUM; ++i)
	{
		queue[i] = QueueCreate();
	
		stat +=  TestIsEmpyty(test_name, i + 1, queue[i], 1);
	
		QueueDestroy(queue[i]);
	}
	
	PrintUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}


int UnitEnqueueSizeDequeueTest(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Enqueue Size Dequeue";
	char *test1_name = "Enqueue Size";
	char *test2_name = "Dequeue Size";
	const size_t TEST_NUM = 10;
	size_t i = 0;
	queue_t *queue = NULL;
	size_t elements[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	PrintUnitTitle(unit_name);
	
	queue = QueueCreate();

	for (i = 0; i < TEST_NUM; ++i)
	{
		QueueEnqueue(queue, (void *)&elements[i]);
		
		stat +=  TestSize(test1_name, i + 1, queue, i + 1);
	}
	
	for (i = TEST_NUM; i > 0; --i)
	{
		QueueDequeue(queue);
		
		stat +=  TestSize(test2_name, (TEST_NUM * 2) - i + 1, queue, i - 1);
	}
	
	QueueDestroy(queue);
	
	PrintUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);	
}

int UnitPeekTest(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Peek";
	char *test1_name = "Enqueue Peek";
	char *test2_name = "Dequeue Peek";
	const size_t TEST_NUM = 10;
	size_t i = 0;
	queue_t *queue = NULL;
	size_t elements[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	size_t test_num = 0;
	
	PrintUnitTitle(unit_name);
	
	queue = QueueCreate();

	for (i = 0; i < TEST_NUM; ++i)
	{
		QueueEnqueue(queue, (void *)&elements[i]);
		
		stat +=  TestPeek(test1_name, ++test_num, queue,
					   &elements[0] ,sizeof(elements[0]));
	}
	
	for (i = 0; i < TEST_NUM; ++i)
	{
		stat +=  TestPeek(test2_name, ++test_num, queue,
					   &elements[i] ,sizeof(elements[0]));
		
		QueueDequeue(queue);
	}
	
	QueueDestroy(queue);
	
	PrintUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);	
}

int UnitAppendTest(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Append";
	char *test1_name = "Append dest is empty";
	char *test3_name = "Append dest size";
	char *test4_name = "Append src size ";
	char *test5_name = "Append dest peek";
	const size_t TEST_NUM = 10;
	size_t i = 0;
	size_t j = 0;
	queue_t *queue[10] = {0};
	size_t elements[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	size_t test_num = 0;
	
	PrintUnitTitle(unit_name);
	
	for (i = 0; i < TEST_NUM; ++i)
	{
		queue[i] = QueueCreate();
	}

	for (i = 0; i < TEST_NUM; ++i)
	{
		for (j = 0; j < TEST_NUM; ++j)
		{
			QueueEnqueue(queue[i], (void *)&elements[j]);
		}
	}
	
	for (i = 1; i < TEST_NUM; ++i)
	{
		queue[0] = QueueAppend(queue[0], queue[i]);
		
		stat +=  TestIsEmpyty(test1_name, ++test_num, queue[0], 0);
		
		stat +=  TestSize(test3_name, ++test_num, queue[0], 10 * (i + 1));
		stat +=  TestSize(test4_name, ++test_num, queue[i], 0);
		
		stat +=  TestPeek(test5_name, ++test_num, queue[0],
			    &elements[0] ,sizeof(elements[0]));
	}
	
	for (i = 0; i < TEST_NUM; ++i)
	{
		QueueDestroy(queue[i]);
	}
	
	PrintUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);	
}

int TestIsEmpyty(char *test_name, size_t test_num, queue_t *queue, int num)
{
	status_t stat = SUCCESS;
	
	assert(test_name);
	assert(queue);
	
	PrintCheckTitle(test_name, test_num);

	stat = QueueIsEmpty(queue) != num;

	PrintCheckStat(stat);
	
	return (stat != SUCCESS);			
}

int TestSize(char *test_name, size_t test_num, queue_t *queue, size_t num)
{
	status_t stat = SUCCESS;
	
	assert(test_name);
	assert(queue);
	
	PrintCheckTitle(test_name, test_num);

	stat = QueueSize(queue) != num;

	PrintCheckStat(stat);
	
	return (stat != SUCCESS);			
}

int TestPeek(char *test_name, size_t test_num,
		   queue_t *queue, void *num, size_t size)
{
	status_t stat = SUCCESS;
	
	assert(test_name);
	assert(queue);
	
	PrintCheckTitle(test_name, test_num);

	stat = memcmp(QueuePeek(queue), num, size) != 0;

	PrintCheckStat(stat);
	
	return (stat != SUCCESS);			
}

/****************** test's prints functions definitions ***********************/

void PrintTestTitle(char *tests_name)
{
	assert(tests_name);
		
	printf("\n\n\033[1;4m%s Tests:\033[m\n", tests_name);
}

void PrintTestStat(int stat, char *tests_name)
{
	assert(tests_name);
		
	if (stat)
	{
		printf("\033[0;31m");
		printf("\n\n\033[1mFail in %d tests :(\n\n", stat);
	}
	else
	{
		printf("\033[0;32m");
		printf("\n\n\033[1mPass all %s tests! :)\n\n", tests_name);
	}	
	printf("\033[0m\033[m");
}

void PrintUnitTitle(char *func_name)
{
	assert(func_name);
	
	printf("\n\n\033[1m%s tests:\033[m\n\n", func_name);
}

void PrintUnitStat(int stat, char *func_name)
{
	assert(func_name);
		
	if (stat)
	{
		printf("\033[0;31m");
		printf("Fail in %d tests :(\n", stat);
	}
	else
	{
		printf("\033[0;32m");
		printf("Pass all %s tests.\n\n", func_name);
	}	
	printf("\033[0m");
}

void PrintCheckTitle(char *tests_name, size_t test_num)
{
	assert(tests_name);
	
	printf("%s test %lu:", tests_name, test_num);
}

void PrintCheckStat(int stat)
{
	if (stat)
	{
		printf(FAIL_MARK);
	}
	else
	{
		printf(CHECK_MARK);
	}
}

/*addional tests*/

int SlistIsEmptyTest(void)
{
	queue_t *queue = NULL;
	
	queue = QueueCreate();

	if (QueueIsEmpty(queue))
	{
		QueueDestroy(queue);
		return 1;
	}
	else
	{
		QueueDestroy(queue);
		return 0;
	}
}



/**************************** test's end ***********************************/


