/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 18/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Binary Search Tree   	  *
*				                	  *
**************************************/

#include <stdio.h>			/* printf */
#include <assert.h>			/* assert */
#include <stdlib.h>			/* malloc, free */

#include "ilrd_bst.h"		/* functions declarations */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

void RunTests(void);

static int IntCmpFunc(const void *data1, const void *data2);
static int BSTCreateAndDestroyTest(void);
static int BSTInsertAndRemoveTest(void);
static int BTSFindTest(void);
static int BSTNextAndPrevTest(void);
static int BSTCountTest(void);
static int BSTForEachTest(void);
static int CallBack(void *data, void *param);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n\t******************** TESTS BEGIN *******************\n\n");
	
	printf("\t\tBSTCreate Test: \t%s\n", TEST_RES(BSTCreateAndDestroyTest()));
	printf("\t\tBSTDestroy Test: \t%s\n", TEST_RES(BSTCreateAndDestroyTest()));
	printf("\t\tBSTInsert Test: \t%s\n", TEST_RES(BSTInsertAndRemoveTest()));
	printf("\t\tBSTRemove Test: \t%s\n", TEST_RES(BSTInsertAndRemoveTest()));
	printf("\t\tBTSFind Test:   \t%s\n", TEST_RES(BTSFindTest()));
	printf("\t\tBSTNext Test:   \t%s\n", TEST_RES(BSTNextAndPrevTest()));
	printf("\t\tBSTPrev Test:   \t%s\n", TEST_RES(BSTNextAndPrevTest()));
	printf("\t\tBSTCount Test:   \t%s\n", TEST_RES(BSTCountTest()));	
	printf("\t\tBSTForEach Test:   \t%s\n", TEST_RES(BSTForEachTest()));
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
	
}

static int IntCmpFunc(const void *data1, const void *data2)
{
	assert(data1);
	assert(data2);

	return (*(int *)data1 - *(int *)data2);
}

static int BSTCreateAndDestroyTest(void)
{
	int status = 0;
	
	bst_t *bst_1 = NULL;
	
	bst_1 = BSTCreate(&IntCmpFunc);
	BSTDestroy(bst_1);
	
	return status;	
}

static int BSTInsertAndRemoveTest(void)
{
int status = 3;
	int num1 = 4;
	int num2 = 59;
	bst_iter_t iter1 = NULL;
	bst_iter_t iter2 = NULL;
	bst_t *bst_2 = NULL;
	
	bst_2 = BSTCreate(&IntCmpFunc);
	
	iter1 = BSTInsert(bst_2, &num1);
	iter2 = BSTInsert(bst_2, &num2);
	if (num1 == *(int *)BSTGetVal(iter1))
	{
		--status; 
	}
	
	if (num2 == *(int *)BSTGetVal(iter2))
	{
		--status; 
	}
	BSTRemove(iter1);	
	BSTRemove(iter2);
	
	
	if (BSTIsEmpty(bst_2))
	{
		--status;
	}

	BSTDestroy(bst_2);
	
	return status;
}

static int BTSFindTest(void)
{
	int status = 1;
	int num1 = 7;
	int num2 = 33;
	int num3 = 8;
	int num4 = 4;
	int num5 = 1;
	int num6 = 22;
	int *val = &num2;
	
	bst_iter_t iter1 = NULL;
	bst_iter_t iter2 = NULL;
	bst_iter_t iter3 = NULL;
	bst_iter_t iter4 = NULL;
	bst_iter_t iter5 = NULL;
	bst_t *mng_bst = NULL;
	
	mng_bst = BSTCreate(&IntCmpFunc);
	
	iter1 = BSTInsert(mng_bst, &num1);
	iter2 = BSTInsert(mng_bst, &num2);
	iter3 = BSTInsert(mng_bst, &num3);
	iter4 = BSTInsert(mng_bst, &num4);
	iter5 = BSTInsert(mng_bst, &num5);
	
	if (BSTFind(mng_bst, &num6) == BSTEnd(mng_bst)	
    	&& BSTIsEqual(BSTFind(mng_bst, val), iter2))
	{
		status = 0;
	}
	
	BSTRemove(BSTFind(mng_bst, &num1));
    BSTRemove(BSTFind(mng_bst, &num4));
    BSTRemove(BSTFind(mng_bst, &num3));
    BSTRemove(BSTFind(mng_bst, &num2));
    BSTRemove(BSTFind(mng_bst, &num5));
	
	(void)iter1;
	(void)iter2;
	(void)iter3;
	(void)iter4;
	(void)iter5;
	
	BSTDestroy(mng_bst);
	return status;	
}


static int BSTNextAndPrevTest(void)
{
	int status = 0;
	int num1 = 8;
	int num2 = 12;
	int num3 = 7;
	int num4 = 4;
	int num5 = 2;
	
	bst_iter_t iter1 = NULL;
	bst_iter_t iter2 = NULL;
	bst_iter_t iter3 = NULL;
	bst_iter_t iter4 = NULL;
	bst_iter_t iter5 = NULL;
	bst_t *mng_bst = NULL;
	
	mng_bst = BSTCreate(&IntCmpFunc);
	
	iter1 = BSTInsert(mng_bst, &num1);
	iter2 = BSTInsert(mng_bst, &num2);
	iter3 = BSTInsert(mng_bst, &num3);
	iter4 = BSTInsert(mng_bst, &num4);
	iter5 = BSTInsert(mng_bst, &num5);
	
	status += !(BSTIsEqual(iter5, BSTBegin(mng_bst)));
	
	status += !(BSTIsEqual(iter4, BSTNext(iter5)));
	status += !(BSTIsEqual(iter3, BSTNext(iter4)));
	status += !(BSTIsEqual(iter1, BSTNext(iter3)));
	status += !(BSTIsEqual(iter2, BSTNext(iter1)));
	status += !(BSTIsEqual(BSTEnd(mng_bst), BSTNext(iter2)));
	
	status += !(BSTIsEqual(iter4, BSTPrev(iter3)));
	status += !(BSTIsEqual(iter3, BSTPrev(iter1)));
	status += !(BSTIsEqual(iter1, BSTPrev(iter2)));
	
	BSTRemove(BSTFind(mng_bst, &num1));
    BSTRemove(BSTFind(mng_bst, &num4));
    BSTRemove(BSTFind(mng_bst, &num2));
    BSTRemove(BSTFind(mng_bst, &num3));
    BSTRemove(BSTFind(mng_bst, &num5));

	BSTDestroy(mng_bst);
	return status;	
}

static int BSTCountTest(void)
{
	int status = 1;
	int num1 = 8;
	int num2 = 12;
	int num3 = 7;
	int num4 = 4;
	int num5 = 2;
	
	bst_t *mng_bst = NULL;
	
	mng_bst = BSTCreate(&IntCmpFunc);
	
	BSTInsert(mng_bst, &num1);
	BSTInsert(mng_bst, &num2);
	BSTInsert(mng_bst, &num3);
	BSTInsert(mng_bst, &num4);
	BSTInsert(mng_bst, &num5);
	
	if (5 == BSTCount(mng_bst))
	{
		status = 0;
	}
	
	BSTDestroy(mng_bst);
	return status;
}


static int BSTForEachTest(void)
{
	int status = 1;
	int data1 = 8;
	int data2 = 12;
	int data3 = 7;
	int data4 = 4;
	int data5 = 2;
	
	bst_t *bst = NULL;
	
	bst = BSTCreate(&IntCmpFunc);
	
	BSTInsert(bst, &data1);
	BSTInsert(bst, &data2);
	BSTInsert(bst, &data3);
	BSTInsert(bst, &data4);
	BSTInsert(bst, &data5);
	
	if (0 == BSTForEach(BSTBegin(bst),(BSTEnd(bst)), CallBack, NULL))
	{
		status = 0;
	}
	
	BSTDestroy(bst);
	
	return status;	
}

static int CallBack(void *data, void *param)
{
	int status = 0;

	(void)param;
	
	++(*(int *)data);	
	
	return status;
}


































