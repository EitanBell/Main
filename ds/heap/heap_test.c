/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Matan	              *
*	Date: 06/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Heap  	  				  *
*				                	  *
**************************************/

#include <stdio.h>			/* printf */
#include <assert.h>			/* assert */
#include <stdlib.h>			/* malloc, free */
#include <stdio.h>

#include "heap.h"		/* HEAP API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) (bool ^ 1)

enum status
{
	SUCCESS = 0,
	FAIL
} status_t;

/*------------- FUNC DECLARATIONS -------------*/
void RunTests(void);
int CmpIntFunc(const void *heap_data, const void *new_data, void *param);
static int CreatePushDestroyHeapTest(void);
static int SizeIsEmptyPeekHeapTest(void);
static int HeapPushPopTest(void);

/*------------- MAIN FUNC -------------*/
int main(void)
{
	RunTests();
	
	return 0;
}

/*------------- FUNC INPLEMENTATIONS -------------*/
void RunTests(void)
{
	printf("\n\t******************** TESTS BEGIN *******************\n\n");
	
	printf("\t\tHeapCreate  \tTest: \t%s\n", TEST_RES(CreatePushDestroyHeapTest()));
	printf("\t\tHeapPush    \tTest: \t%s\n", TEST_RES(CreatePushDestroyHeapTest()));
	printf("\t\tHeapDestroy \tTest: \t%s\n", TEST_RES(CreatePushDestroyHeapTest()));
	printf("\t\tHeapIsEmpty \tTest: \t%s\n", TEST_RES(SizeIsEmptyPeekHeapTest()));
	printf("\t\tHeapSize    \tTest: \t%s\n", TEST_RES(SizeIsEmptyPeekHeapTest()));
	printf("\t\tHeapPeek    \tTest: \t%s\n", TEST_RES(SizeIsEmptyPeekHeapTest()));
	printf("\t\tHeapPop     \tTest: \t%s\n", TEST_RES(HeapPushPopTest()));
	printf("\t\tHeapifyUp   \tTest: \t%s\n", TEST_RES(HeapPushPopTest()));
	printf("\t\tHeapifyDownUp   Test: \t%s\n", TEST_RES(HeapPushPopTest()));
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
	
}

int CmpIntFunc(const void *heap_data, const void *new_data, void *param)
{
	(void)param;
	
	return (*(int *)heap_data - *(int *)new_data);
}

static int CreatePushDestroyHeapTest(void)
{
	heap_t *heap = HeapCreate(&CmpIntFunc, NULL);
	int arr[] = {5, -2, 0, 7, 4};
	int status = 0;
	size_t i = 0;
	size_t arr_len = sizeof(arr) / sizeof(*arr);
	
	for (i = 0; i < arr_len; ++i)
	{
		status += HeapPush(heap, arr + i);
	}

	HeapDestroy(heap);
	heap = NULL;

	return ((0 == status) ? SUCCESS : FAIL);
}

static int SizeIsEmptyPeekHeapTest(void)
{
	heap_t *heap = HeapCreate(&CmpIntFunc, NULL);
	int arr[] = {-2, 5, 1, 7, 4};
	int status = 0;
	size_t i = 0;
	size_t arr_len = sizeof(arr) / sizeof(*arr);
	
	status += BOOL_TO_STATUS(HeapIsEmpty(heap));
	
	for (i = 0; i < arr_len; ++i)
	{
		HeapPush(heap, arr + i);
	}
	
	status += BOOL_TO_STATUS((5 == HeapSize(heap)));
	status += BOOL_TO_STATUS((-2 == *(int *)HeapPeek(heap)));
	status += BOOL_TO_STATUS(!HeapIsEmpty(heap));

	HeapDestroy(heap);
	heap = NULL;

	return ((0 == status) ? SUCCESS : FAIL);
}

static int HeapPushPopTest(void)
{
	heap_t *heap = NULL;
	size_t i = 0;
	int test_arr[] = {7, 3, 8, 4, 12, 14, 28, 2, 1, 24};
	int status = SUCCESS;

	heap = HeapCreate(&CmpIntFunc, NULL);
	if (NULL == heap)
	{
		status = FAIL;
	}

	for (i = 0; 10 > i; ++i)
	{
		status += HeapPush(heap, (test_arr + i));
	}

	status += !(1 == *(int *)HeapPeek(heap));
	status += !(10 == HeapSize(heap));

	HeapPop(heap);
	status += !(2 == *(int *)HeapPeek(heap));
	status += !(9 == HeapSize(heap));

	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);

	status += !(8 == *(int *)HeapPeek(heap));
	status += !(5 == HeapSize(heap));

	for (i = 0; 5 > i; ++i)
	{
		status += !((5 - i) == HeapSize(heap));
		HeapPop(heap);
	}

	status += !HeapIsEmpty(heap);
	HeapDestroy(heap);

	return status;
}




