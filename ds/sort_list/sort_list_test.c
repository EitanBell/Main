/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 29/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Sorted List   	       *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */

#include "sort_list.h"	/* sort_list */

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")

void RunTests(void);

static int IntCmp(const void *data, const void *param);
static int SortListCreateDestroyTest(void);
static int SortListInsertRemoveTest(void);
static int SortListFindTest(void);
static int SortListFindIfTest(void);
static int IsIntEqualTest(const void *data, void *param);
static int SortListForEachTest(void);
static int DoActionTest(void *data, void *param);
static int SortListGetValTest(void);
static int SortListIsEqualTest(void);
static int SortListCountTest(void);
static int SortListPopFrontTest(void);
static int SortListPopBackTest(void);
static int SortListMergeTest(void);
static int IntergrationTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n****************** TESTS BEGIN ******************\n\n");
	
	printf("SortListCreate Test: \t%s\n", TEST_RES(SortListCreateDestroyTest()));
	printf("SortListDestroy Test: \t%s\n", TEST_RES(SortListCreateDestroyTest()));
	printf("SortListInsert Test: \t%s\n", TEST_RES(SortListInsertRemoveTest()));
	printf("SortListRemove Test: \t%s\n", TEST_RES(SortListInsertRemoveTest()));
	printf("SortListFind Test: \t%s\n", TEST_RES(SortListFindTest()));
	printf("SortListFindIf Test: \t%s\n", TEST_RES(SortListFindIfTest()));
	printf("SortListForEach Test: \t%s\n", TEST_RES(SortListForEachTest()));
	printf("SortListGetVal Test: \t%s\n", TEST_RES(SortListGetValTest()));
	printf("SortListIsEqual Test: \t%s\n", TEST_RES(SortListIsEqualTest()));
	printf("SortListCount Test: \t%s\n", TEST_RES(SortListCountTest()));
	printf("SortListPopFront Test: \t%s\n", TEST_RES(SortListPopFrontTest()));
	printf("SortListPopBack Test: \t%s\n", TEST_RES(SortListPopBackTest()));
	printf("SortListMerge Test: \t%s\n", TEST_RES(SortListMergeTest()));
	printf("Intergration Test: \t%s\n", TEST_RES(IntergrationTest()));
	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int SortListCreateDestroyTest(void)
{
	int status = 1;
	sort_list_t *list = NULL;
	sort_list_cmp_t cmp_ptr = NULL;
	
	cmp_ptr = &IntCmp;
	list = SortListCreate(cmp_ptr);
	
	if (SortListIsEmpty(list))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int SortListInsertRemoveTest(void)
{
	int status = 1;
	sort_list_t *list = {NULL};
	sort_list_iter_t iter = {NULL};
	int data1 = 3;
	int *ptr1 = &data1;
	int data2 = 7;
	int *ptr2 = &data2;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	
	iter = SortListInsert(list, ptr1);
	iter = SortListInsert(list, ptr2);
	
	iter = SortListBegin(list);
	
	iter = SortListRemove(iter);
	iter = SortListRemove(iter);
	
	if (SortListIsEmpty(list))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int SortListFindTest(void)
{
	int status = 1;
	
	sort_list_t *list = {NULL};
	sort_list_iter_t iter = {0};
	sort_list_iter_t iter_from = {0};
	sort_list_iter_t iter_to = {0};
	
	int data = 25;
	int *ptr = &data;
	int new_val = 12;
	int *ptr_new_val = &new_val;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	
	SortListInsert(list, ptr);
	SortListInsert(list, ptr);
	SortListInsert(list, ptr);
	SortListInsert(list, ptr);
	SortListInsert(list, ptr_new_val);
	SortListInsert(list, ptr);
	SortListInsert(list, ptr);
	(void)iter;

	iter_from = SortListBegin(list);
	iter_to = SortListEnd(list);
	
	if(12 == *(int *)SortListGetVal(SortListFind(list, iter_from, iter_to, ptr_new_val)))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int SortListFindIfTest(void)
{
	int status = 1;
	
	sort_list_t *list = {NULL};
	sort_list_iter_t iter = {0};
	sort_list_iter_t iter_match = {0};
	sort_list_iter_t iter_from = {0};
	sort_list_iter_t iter_to = {0};
	
	int data = 22;
	int *ptr = &data;
	float new_val = 7.75;
	float *ptr_new_val = &new_val;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	
	iter = SortListInsert(list, ptr);
	iter = SortListInsert(list, ptr);
	iter = SortListInsert(list, ptr);
	iter = SortListInsert(list, ptr);
	iter = SortListInsert(list, ptr);
	iter_match = SortListInsert(list, ptr_new_val);
	(void)iter;

	iter_from = SortListBegin(list);
	iter_to = SortListEnd(list);
	
	if (SortListIterIsEqual(iter_match, SortListFindIf(iter_from, iter_to, IsIntEqualTest, ptr_new_val)))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int IsIntEqualTest(const void *data, void *param)
{
	assert(data);
	assert(param);
	
	return (*(int *)data == *(int *)param);
}

static int SortListForEachTest(void)
{
	int status = 1;
	size_t tests_iter = 60;
	int data = 3;
	int *ptr = &data;
	int param = 4;
	int *ptr_param = &param;
	
	sort_list_t *list = {NULL};
	sort_list_iter_t iter = {0};
	sort_list_iter_t iter_from = {0};
	sort_list_iter_t iter_to = {0};
	(void)iter;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	
	while (0 != tests_iter)
	{
		iter = SortListInsert(list, ptr);
		tests_iter -= 1;
	}
	
	iter_from = SortListBegin(list);
	iter_to = SortListEnd(list);
	
	status = SortListForEach(iter_from, iter_to, DoActionTest, ptr_param);
	
	SortListDestroy(list);
	
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

static int IntCmp(const void *data, const void *param)
{
	assert(data);
	assert(param);
	
	return (*(int *)data - *(int *)param);
}

static int SortListGetValTest(void)
{
	int status = 1;
	sort_list_t *list = {NULL};
	sort_list_iter_t iter = {NULL};
	float data = 10.2;
	float *ptr = &data;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	
	iter = SortListInsert(list, ptr);
	
	if ((float)10.2 == *(float *)SortListGetVal(iter))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}
static int SortListCountTest(void)
{
	int status = 1;
	size_t i = 0;
	size_t tests_iter = 44;
	
	sort_list_iter_t iter = {0};
	sort_list_t *list = {NULL};
	
	int data = 2;
	int *ptr = &data;
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	(void)iter;
	
	for (i = 0; i < tests_iter; ++i)
	{
		iter = SortListInsert(list, ptr);
	}

	if (44 == SortListCount(list))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}


static int SortListIsEqualTest(void)
{
	int status = 1;
	int is_equal = 0;
	int not_equal = 0;
	int data = 20;
	sort_list_iter_t iter = {0};
	sort_list_t *list = {NULL};
	
	list = SortListCreate(IntCmp);
	iter = SortListBegin(list);
	iter = SortListInsert(list, &data);
	
	is_equal = SortListIterIsEqual(SortListBegin(list), iter);
	not_equal = SortListIterIsEqual(SortListEnd(list), iter);
	
	if (is_equal && !not_equal)
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int SortListPopFrontTest(void)
{
	sort_list_t *list = {NULL};
	
	int data = 13;
	int *ptr = &data;
	int status = 1;
	
	list = SortListCreate(IntCmp);
	SortListInsert(list, ptr);

	if (13 == *(int *)SortListPopFront(list))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}


static int SortListPopBackTest(void)
{
	sort_list_t *list = {NULL};
	
	int data = 154;
	int *ptr = &data;
	int status = 1;
	
	list = SortListCreate(IntCmp);
	SortListInsert(list, ptr);
	
	if (154 == *(int *)SortListPopBack(list))
	{
		status = 0;
	}
	
	SortListDestroy(list);
	
	return status;
}

static int SortListMergeTest(void)
{
	int status = 0;
	size_t i = 1;
	
	sort_list_t *list = {NULL};
	sort_list_t *list2 = {NULL};
	sort_list_iter_t iter = {NULL};
	
	
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 20;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	int num11 = 6;
	
	list = SortListCreate(&IntCmp);
	iter = SortListBegin(list);
	
	SortListInsert(list, &num1);
	SortListInsert(list, &num5);
	SortListInsert(list, &num3);
	SortListInsert(list, &num2);
	SortListInsert(list, &num4);
	
	list2 = SortListCreate(&IntCmp);
	
	SortListMerge(list2, list);
	
	iter = SortListBegin(list2);

	while (!SortListIterIsEqual(SortListEnd(list2), iter) && !status)
	{
		status = !((int)i == (*(int *)SortListGetVal(iter)));
		iter = SortListNext(iter);
		++i;
	}
	
	SortListInsert(list, &num6);
	SortListInsert(list, &num7);
	SortListInsert(list, &num10);
	SortListInsert(list, &num8);
	SortListInsert(list, &num9);
	SortListInsert(list, &num11);
	
	SortListMerge(list, list2);
	
	iter = SortListBegin(list);
	
	i = 1;
	while (!SortListIterIsEqual(SortListEnd(list), iter) && !status)
	{
		if (SortListIterIsEqual(SortListPrev(SortListEnd(list)), iter))
		{
				status = !(20 == (*(int *)SortListGetVal(iter)));
		}
		else
		{
			status = !((int)i == (*(int *)SortListGetVal(iter)));
		}
		
		iter = SortListNext(iter);
		++i;
	}
	
	SortListDestroy(list);
	SortListDestroy(list2);
	
	return status;
}

static int IntergrationTest(void)
{
	int status = 0;
	size_t i = 0;
	
	sort_list_t *list = {0};
	sort_list_t *list2 = {0};
	sort_list_iter_t iter = {0};
	
	int input1[] = {-1, 5, 3, -596};
	int input2[] = {-10, 6, 2, 4, -20};
	int input3[] = {6, 8, 9, 7};
	
	int arr_res1[] = {-596, -20, -10, -1, 2, 3, 4, 5, 6};
	int arr_res2[] = {-596, -20, -10, -1, 2, 3, 4, 5, 6, 6, 7, 8, 9};
	
	list = SortListCreate(&IntCmp);
	
	/* -596, -1, 3, 5 */
	SortListInsert(list, &input1[0]);
	SortListInsert(list, &input1[1]);
	SortListInsert(list, &input1[2]);
	SortListInsert(list, &input1[3]);
	
	list2 = SortListCreate(&IntCmp);
	/* -20, -10, 2, 4, 6 */
	SortListInsert(list2, &input2[0]);
	SortListInsert(list2, &input2[1]);
	SortListInsert(list2, &input2[2]);
	SortListInsert(list2, &input2[3]);
	SortListInsert(list2, &input2[4]);
	
	SortListMerge(list2, list);
	
	iter = SortListBegin(list2);
	
	if (9 == SortListCount(list2) && 
		0 == SortListCount(list) &&
		SortListIsEmpty(list) &&
		!SortListIsEmpty(list2))
	{
		while (!SortListIterIsEqual(SortListEnd(list2), iter) &&
				!status)
		{
			status = !(arr_res1[i] == (*(int *)SortListGetVal(iter)));
			iter = SortListNext(iter);
			++i;
		}
	}
	else
	{
		status = 1;
	}
	
	SortListInsert(list, &input3[0]);
	SortListInsert(list, &input3[1]);
	SortListInsert(list, &input3[2]);
	SortListInsert(list, &input3[3]);
	
	if (9 == SortListCount(list2) && 
		4 == SortListCount(list) &&
		!SortListIsEmpty(list) &&
		!SortListIsEmpty(list2))
	{
		status = 0;
	}
	else
	{
		status = 1;
	}
	
	SortListMerge(list, list2);
	
	if (0 == SortListCount(list2) && 
		13 == SortListCount(list) &&
		!SortListIsEmpty(list) &&
		SortListIsEmpty(list2))
	{
		i = 0;
		iter = SortListBegin(list);
		while (!SortListIterIsEqual(SortListEnd(list), iter) && !status)
		{
			status = !(arr_res2[i] == (*(int *)SortListGetVal(iter)));
			iter = SortListNext(iter);
			++i;
		}
	}
	else
	{
		status = 1;
	}
	
	SortListPopBack(list);
	SortListPopFront(list);
	SortListPopBack(list);
	SortListPopFront(list);
	
	if (!status && 9 == SortListCount(list))
	{
		status = 0;
	}
	else
	{
		status = 1;
	}
	
	SortListDestroy(list);
	SortListDestroy(list2);
	
	return status;
}









