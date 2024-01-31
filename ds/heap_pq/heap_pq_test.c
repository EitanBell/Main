/*************************************
*				   			  		 *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:                        *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: priority queue   	  	 *
*				                	 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */

#include "heap_pq.h"	/* Heap_pq API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

void RunTests(void);

static int IntCmpFunc(const void *data1, const void *data2);
static int PQCreateAndDestroyTest(void);
static int PQEnqueueDequeueTest(void);
static int PQSizeAndIsEmptyTest(void);
static int PQClearTest(void);
static int IsMatchEqualInts(const void *data, void *param);
static int PQEraseTest(void);
static int IntegrationTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n******************** TESTS BEGIN *******************\n\n");
	
	printf("\tPQCreate Test: \t\t%s\n", TEST_RES(PQCreateAndDestroyTest()));
	printf("\tPQDestroy Test: \t%s\n", TEST_RES(PQCreateAndDestroyTest()));
	printf("\tPQEnqueue Test: \t%s\n", TEST_RES(PQEnqueueDequeueTest()));
	printf("\tPQDequeue Test: \t%s\n", TEST_RES(PQEnqueueDequeueTest()));
	printf("\tPQSize Test: \t\t%s\n", TEST_RES(PQSizeAndIsEmptyTest()));
	printf("\tPQIsEmpty Test: \t%s\n", TEST_RES(PQSizeAndIsEmptyTest()));
	printf("\tPQClear Test: \t\t%s\n", TEST_RES(PQClearTest()));
	printf("\tPQErase Test: \t\t%s\n", TEST_RES(PQEraseTest()));
	printf("\tIntegration Test: \t%s\n", TEST_RES(IntegrationTest()));
	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int PQCreateAndDestroyTest(void)
{
	ilrd_pq_t *pq = NULL;
	int status = 1;
	
	pq = PQCreate(&IntCmpFunc);
	
	if (PQIsEmpty(pq))
	{
		status = 0;
	}
	
	PQDestroy(pq);
	return status;
}


static int IntCmpFunc(const void *data1, const void *data2)
{
	assert(data1);
	assert(data2);

	return (*(int *)data1 - *(int *)data2);
}

static int PQEnqueueDequeueTest(void)
{
	size_t i = 0;
	int status = 0;
	int arr[] = {54, 11, 1, 5, 55, 22, 2};
	int sort_arr[] = {55, 54, 22, 11, 5, 2, 1};
	ilrd_pq_t *pq = NULL;
	
	pq = PQCreate(&IntCmpFunc);
	
	for (i = 0; 7 > i; ++i)
	{
		status += PQEnqueue(pq, &arr[i]);
	}
	
	for (i = 6; 0 < i; --i)
	{
		status += !(*(int *)PQPeek(pq) == sort_arr[i]);
		PQDequeue(pq);
	}
	
	PQDestroy(pq);
	return status;
}

static int PQClearTest(void)
{
	size_t i = 0;
	int status = 0;
	int arr[] = {55, 33, 22, 77, 100, 6, 1};
	ilrd_pq_t *pq = NULL;
	
	pq = PQCreate(&IntCmpFunc);
	
	for (i = 0; 7 > i; ++i)
	{
		status += PQEnqueue(pq, &arr[i]);
	}
	
	status += !(7 == PQSize(pq));
	
	PQClear(pq);
	status += !PQIsEmpty(pq);
	
	PQDestroy(pq);
	return status;
}


static int PQSizeAndIsEmptyTest(void)
{
	size_t i = 0;
	int status = 0;
	int arr[] = {55, 33, 22, 77, 100, 6, 1};
	ilrd_pq_t *pq = NULL;
	
	pq = PQCreate(&IntCmpFunc);
	
	for (i = 0; 7 > i; ++i)
	{
		status += PQEnqueue(pq, &arr[i]);
	}
	
	status += (7 == PQSize(pq));
	
	for (i = 7; 0 < i; --i)
	{
		PQDequeue(pq);
	}
	
	status = !PQIsEmpty(pq);
	
	PQDestroy(pq);
	return status;
}

static int PQEraseTest(void)
{
	size_t i = 0;
	int status = 1;
	int val = 4;
	int arr[] = {55, 33, 22, 77, 100, 6, 1};
	int *data = arr + 2; 
	ilrd_pq_t *pq = NULL;
	pq_is_match_t match_func = NULL;
	
	pq = PQCreate(&IntCmpFunc);
	match_func = &IsMatchEqualInts;
	
	for (i = 0; 7 > i; ++i)
	{
		status += PQEnqueue(pq, &arr[i]);
	}
	
	if (*data == *(int *)PQErase(pq, data, match_func)
		&& NULL == PQErase(pq, &val, match_func)
		&& 6 == PQSize(pq))
		{
			status -= 1;
		} 
		
	PQDestroy(pq);
	return status;
}

static int IsMatchEqualInts(const void *data, void *param)
{
	assert(data);
	assert(param);
	
	return (*(int *)data == *(int *)param);
}


static int IntegrationTest(void)
{
	ilrd_pq_t *pq = NULL;
	int first_tester[20] = {0};
	int second_tester[20] = {0};
	size_t i = 0;
	int status = 0;
	void *ereaser = NULL;
	int erease = 0;
	
	for (i = 0; i < 20; ++i)
	{
		first_tester[i] = i * 2;
	}
	
	for (i = 0; i < 20; ++i)
	{
		second_tester[i] = i * 3;
	}
	
	
	pq = PQCreate(&IntCmpFunc);
	
	for (i = 0; i < 20; ++i)
	{
		status += PQEnqueue(pq, &first_tester[i]);
	}

	status += (20 != PQSize(pq));
	
	for (i = 0; i < 20; ++i)
	{
		status += PQEnqueue(pq, &second_tester[i]);
	}
	
	status += (40 != PQSize(pq));
	
	erease = 6;
	ereaser = PQErase(pq, &erease, &IsMatchEqualInts);
	
	status += (6 != *(int *)ereaser);						  		
	status += (39 != PQSize(pq));
	
	erease = 5;					
	ereaser = PQErase(pq, &erease, &IsMatchEqualInts);
	
	status += (NULL != ereaser);						  		
	status += (39 != PQSize(pq));
	status += (0 != *(int *)PQPeek(pq));
	
	for (i = 0; i < 9; ++i)
	{
		PQDequeue(pq);
		status += (39 - i - 1 != PQSize(pq)); 
	}
	
	PQClear(pq); 

	status += (!PQIsEmpty(pq));
	
	PQDestroy(pq);	
	
	return status;
}








