/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Gali                 *
*	Date: 17/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: doubly Linked List	  *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */

#include "dlist.h"	/* dlist */

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31mFAIL\033[0m" : "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

void RunTests(void);
static int DlistCreateDestroyTest(void);
static int DlistGetValAndInsertTest(void);
static int DlistSetValTest(void);
static int DlistCountAndRemoveTest(void);
static int DlistFrontPushAndPopTest(void);
static int DlistBackPushAndPopTest(void);
static int DlistFindAndIsEqualTest(void);
static int IsIntMatchTest(const void *data, void *param);
static int DlistForEachTest(void);
static int DoActionTest(void *data, void *param);
static int DlistSpliceTest(void);
static int DlistMultiFindTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n****************** TESTS BEGIN ******************\n\n");
	printf("DlistCreateDestroy: Test \t%s\n", TEST_RES(DlistCreateDestroyTest()));
	printf("DlistGetValAndInsert Test: \t%s\n", TEST_RES(DlistGetValAndInsertTest()));
	printf("DlistSetVal Test: \t\t%s\n", TEST_RES(DlistSetValTest()));
	printf("DlistCountAndRemove Test: \t%s\n", TEST_RES(DlistCountAndRemoveTest()));
	printf("DlistFrontPushAndPop Test: \t%s\n", TEST_RES(DlistFrontPushAndPopTest()));
	printf("DlistBackPushAndPop Test: \t%s\n", TEST_RES(DlistBackPushAndPopTest()));
	printf("DlistFindAndIsEqual Test: \t%s\n", TEST_RES(DlistFindAndIsEqualTest()));
	printf("DlistForEach Test: \t\t%s\n", TEST_RES(DlistForEachTest()));
	printf("DlistSplice Test: \t\t%s\n", TEST_RES(DlistSpliceTest()));
	printf("DlistMultiFind Test: \t\t%s\n", TEST_RES(DlistMultiFindTest()));
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int DlistCreateDestroyTest(void)
{
	dlist_t *list = NULL;
	
	list = DlistCreate();
	DlistDestroy(list);
	
	return 0;
}


static int DlistGetValAndInsertTest(void)
{
	int status = 1;
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    int num1 = 5;
    void *data = NULL;

    list = DlistCreate();
    iter = DlistBegin(list);
    
    iter = DlistInsertBefore(iter, &num1);
    data = DlistGetVal(iter);
    
    if (*(int *)data == num1)
    {
    	DlistDestroy(list);
    	status = 0;
    }
    else
    {
    	DlistDestroy(list);
    	status = 1;
    }

	return status;
}


static int DlistSetValTest(void)
{
	int status = 2;
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;
    void *data = NULL;

    list = DlistCreate();
    iter = DlistBegin(list);
    
    iter = DlistInsertBefore(iter, &num1);
    data = DlistGetVal(iter);
    
    if (*(int *)data == num1)
    {
    	status -= 1;
    }
    
    DlistSetVal(iter, &num2);
    
    if (*(int *)DlistGetVal(iter) == num2)
    {
    	status -= 1;
    }
    
    DlistDestroy(list);
	return status;
}

static int DlistCountAndRemoveTest(void)
{
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;
    int num3 = 12;
    int num4 = 18;
    int status = 4;

    list = DlistCreate();
    iter = DlistBegin(list);
    
    iter = DlistInsertBefore(iter, &num1);
	iter = DlistInsertBefore(iter, &num2);
   	
   	if (2 == DlistCount(list))
	{
		status -= 1;
	}
   
    iter = DlistInsertBefore(iter, &num3);
	iter = DlistInsertBefore(iter, &num4);
	
	if (4 == DlistCount(list))
	{
		status -= 1;
	}
	
	iter = DlistRemove(iter);
	iter = DlistRemove(iter);
	iter = DlistRemove(iter);
	
	if (1 == DlistCount(list))
	{
		status -= 1;
	}
	
	iter = DlistRemove(iter);
	
	if (DlistIsEmpty(list))
	{
		status -= 1;		
	}
	
	DlistDestroy(list);
	return status;
}


static int DlistFrontPushAndPopTest(void)
{
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;
    int num3 = 12;
    int num4 = 18;
    void *data = NULL;
    int status = 2;

    list = DlistCreate();
    iter = DlistBegin(list);
    
    iter = DlistInsertBefore(iter, &num1);
	iter = DlistPushFront(list, &num2);
    iter = DlistPushFront(list, &num3);
	iter = DlistPushFront(list, &num4);
	
	if (4 == DlistCount(list))
	{
		status -= 1;
	}
	
	data = DlistPopFront(list);
	data = DlistPopFront(list);
	data = DlistPopFront(list);
	data = DlistPopFront(list);
	
	if (DlistIsEmpty(list) && *(int *)data == num1)
	{
		status -= 1;		
	}
	
	DlistDestroy(list);
	return status;
}

static int DlistBackPushAndPopTest(void)
{
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;
    int num3 = 12;
    int num4 = 18;
    void *data = NULL;
    int status = 2;

    list = DlistCreate();
    iter = DlistBegin(list);
    
    iter = DlistInsertBefore(iter, &num4);
	iter = DlistPushBack(list, &num3);
    iter = DlistPushBack(list, &num2);
	iter = DlistPushBack(list, &num1);
	
	if (4 == DlistCount(list))
	{
		status -= 1;
	}
	
	data = DlistPopBack(list);
	data = DlistPopBack(list);
	data = DlistPopBack(list);
	data = DlistPopBack(list);
	
	if (DlistIsEmpty(list) && *(int *)data == num4)
	{
		status -= 1;		
	}
	
	DlistDestroy(list);
	return status;
}

static int DlistFindAndIsEqualTest(void)
{
	int status = 1;
	
	dlist_t *list = NULL;
	dlist_iter_t iter = NULL;
	dlist_iter_t iter_match = NULL;
	dlist_iter_t iter_from = NULL;
	dlist_iter_t iter_to = NULL;
	
	int data = 56;
	int *ptr = &data;
	float new_val = 100.92;
	float *ptr_new_val = &new_val;
	
	list = DlistCreate();
	iter = DlistEnd(list);
	
	iter = DlistInsertBefore(iter, ptr);
	iter = DlistInsertBefore(iter, ptr);
	iter = DlistInsertBefore(iter, ptr);
	iter = DlistInsertBefore(iter, ptr);
	iter = DlistInsertBefore(iter, ptr);
	iter_match = DlistInsertBefore(iter, ptr_new_val);
	
	iter_from = DlistBegin(list);
	iter_to = DlistEnd(list);
	
	if(iter_match == DlistFind(iter_from, iter_to, IsIntMatchTest, ptr_new_val))
	{
		status = 0;
	}
	
	DlistDestroy(list);
	
	return status;
}


static int IsIntMatchTest(const void *data, void *param)
{
	assert(data);
	assert(param);
	
	return (*(int *)data == *(int *)param);
}

static int DlistForEachTest(void)
{
	int status = 1;
	size_t tests_iter = 50;
	
	dlist_t *list = NULL;
	dlist_iter_t iter = NULL;
	dlist_iter_t iter_from = NULL;
	dlist_iter_t iter_to = NULL;
	
	int data = 5;
	int *ptr = &data;
	int param = 10;
	int *ptr_param = &param;
	
	list = DlistCreate();
	iter = DlistEnd(list);
	
	while (--tests_iter)
	{
		iter = DlistInsertBefore(iter, ptr);
	}
	
	iter_from = DlistBegin(list);
	iter_to = DlistEnd(list);
	
	status = DlistForEach(iter_from, iter_to, DoActionTest, ptr_param);
	
	DlistDestroy(list);
	
	return status;
}

static int DoActionTest(void *data, void *param)
{
	int status = 0;
	
	assert(data);
	assert(param);
	
	*(int *)data += *(int *)param;
	
	return status;
}

static int DlistSpliceTest(void)
{
	dlist_t *list = NULL;
    dlist_iter_t iter = NULL;
    dlist_iter_t iter1 = NULL;
    dlist_iter_t iter2 = NULL;
    dlist_iter_t iter3 = NULL;
    dlist_iter_t iter4 = NULL;
    dlist_iter_t iter5 = NULL;
    int num1 = 5;
    int num2 = 7;
    int num3 = 12;
    int num4 = 18;
    int num5 = 14;
    int status = 1;

    list = DlistCreate();
    iter1 = DlistBegin(list);
    
    iter1 = DlistPushBack(list, &num1);
	iter2 = DlistPushBack(list, &num2);
    iter3 = DlistPushBack(list, &num3);
	iter4 = DlistPushBack(list, &num4);
	iter5 = DlistPushBack(list, &num5);
	
	iter = DlistSplice(iter3, iter5, iter2);
	
	if (DlistIterIsEqual(iter4, iter) && DlistIterIsEqual(iter2, DlistNext(iter)) 
		&& DlistIterIsEqual(iter3, DlistPrev(iter)))
	{
		status = 0;
	}
	(void)iter1;
	DlistDestroy(list);
	return status;
}

static int DlistMultiFindTest(void)
{
	int status = 1;
	
	dlist_t *list = NULL;
	dlist_t *list2 = NULL;
	dlist_iter_t iter = NULL;
	dlist_iter_t iter1 = NULL;
	dlist_iter_t iter2 = NULL;
	dlist_iter_t iter3 = NULL;
	dlist_iter_t iter4 = NULL;
	dlist_iter_t iter5 = NULL;
	dlist_iter_t iter_from = NULL;
	dlist_iter_t iter_to = NULL;
	
	int num1 = 1;
	int num2 = 2;
	int num3 = 7;
	int num4 = 4;
	int num5 = 1;
	
	list = DlistCreate();
	iter = DlistBegin(list);
	
	iter1 = DlistInsertBefore(iter, &num1);
	iter2 = DlistInsertBefore(iter1, &num2);
	iter3 = DlistInsertBefore(iter2, &num3);
	iter4 = DlistInsertBefore(iter3, &num4);
	iter5 = DlistInsertBefore(iter4, &num5);
	(void)iter5;
	
	list2 = DlistCreate();
	
	iter_from = DlistBegin(list);
	iter_to = DlistEnd(list);
	
	if (!DlistMultiFind(list2, iter_from, iter_to, IsIntMatchTest, &status) &&
		2 == DlistCount(list2))
	{
		status = 0;
	}

	DlistDestroy(list);
	DlistDestroy(list2);
	
	return status;
}

