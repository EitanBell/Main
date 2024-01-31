/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 29/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: AVL  	  				  *
*				                	  *
**************************************/

#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h>	/* printf */

#include "avl.h"  /* AVL API */

/*******************	special test's includes			***********************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h>	/* printf */

#include "avl.h"  /* the header to test */

/****************	test's enums, typedefs & macros		***********************/
#define CHECK_MARK ("\t\033[0;32m\xE2\x9C\x93\033[0m\n")
#define FAIL_MARK ("\t\033[0;31m\xE2\x9C\x97\033[0m\n")

typedef enum
{
	SUCCESS = 0,
	FAIL

} status_t;

typedef struct int_cmp
{
	int cmp;
	size_t data;
} int_cmp_t;

/****************		tests functions declartion		***********************/
void ProjectTests(void);
/* add more as you need */

int Unit1Test(void);
int Unit2Test(void);
int Unit3Test(void);
int Unit4Test(void);
int Unit5Test(void);
/* add more as you need */

int TestSizetCmp(char *test_name, size_t test_num, size_t test, size_t res);
int TestSSizetCmp(char *test_name, size_t test_num, ssize_t test, ssize_t res);
int TestIntCmp(char *test_name, size_t test_num, int test, int res);
int TestFlow(char *test_name, size_t test_num);
int AVLIntCmp(const void *data1, const void *data2);
int AVLStructIntCmp(const void *data1, const void *data2);
int SetData(void *data, void *param);

void TestMsgTestTitle(char *tests_name);
void TestMsgTestStat(int stat, char *tests_name);
void TestMsgUnitTitle(char *func_name);
void TestMsgUnitStat(int stat, char *func_name);
void TestMsgCheckTitle(char *tests_name, size_t test_num);
void TestMsgCheckStat(int stat);

/***********************		main		***********************************/
int main()
{
	ProjectTests();
	/* add more as you need */
	return 0;
}

void ProjectTests(void)
{
	char tests_name[] = "AVL";
	status_t stat = SUCCESS;
	
	TestMsgTestTitle(tests_name);
	
	stat += Unit1Test();
	stat += Unit2Test();
	stat += Unit3Test();
	stat += Unit4Test();
	stat += Unit5Test();
	
	TestMsgTestStat(stat, tests_name);
}

int Unit1Test(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Create & Destroy";
	size_t test_num = 0;
	avl_t *avl = NULL;
	TestMsgUnitTitle(unit_name);

	avl = AVLCreate(AVLIntCmp);
	
	stat += TestSizetCmp("AVL Create", ++test_num, (avl == NULL), 0);
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);

	AVLDestroy(avl);
	stat += TestFlow("AVL Destroy", ++test_num);
	
	TestMsgUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}

int Unit2Test(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Insert & Find";
	size_t test_num = 0;
	size_t i = 0;
	avl_t *avl = NULL;
	int test[] = {6, 3, 8, 2, 0, 7, -17, 200, 1, 5, 67, -300, 4};
	int *res[13] = {0};

	TestMsgUnitTitle(unit_name);

	avl = AVLCreate(AVLIntCmp);
	
	stat += TestSizetCmp("AVL Create", ++test_num, (avl == NULL), 0);
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);

	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("Find Stat before", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 0);
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("Find Stat after", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 1);
	}
	
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 0);	
	
	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("Find Stat", ++test_num, AVLFind(avl, (void *)(test + i), (void **)&(res[i])) , 1);
		stat += TestIntCmp("AVL Find", ++test_num, test[i], *res[i]);
	}
	
	stat += TestSizetCmp("AVL Find", ++test_num, AVLFind(avl, (void *)(test + 10), (void **)(res + i)) != 0 , 0);
	stat += TestSizetCmp("AVL Find", ++test_num, AVLFind(avl, (void *)(test + 11), (void **)(res + i)) != 0 , 0);
	stat += TestSizetCmp("AVL Find", ++test_num, AVLFind(avl, (void *)(test + 12), (void **)(res + i)) != 0 , 0);
	
	AVLDestroy(avl);
	
	stat += TestFlow("AVL Destroy", ++test_num);
	
	TestMsgUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}

int Unit3Test(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Remove & Count";
	size_t test_num = 0;
	size_t i = 0;
	avl_t *avl = NULL;
	int test[] = {6, 3, 8, 2, 0, 7, -17, 200, 1, 5, 67, -300, 4};
	int res[13] = {0};

	TestMsgUnitTitle(unit_name);

	avl = AVLCreate(AVLIntCmp);
	
	stat += TestSizetCmp("AVL Create", ++test_num, (avl == NULL), 0);
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);
	stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), 0);

	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), i + 1);
	}
	
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 0);	
	
	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), 10 - i);
		stat += TestSizetCmp("Find Exist", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 1);
		AVLRemove(avl, (void *)(test + i));
		stat += TestFlow("AVL Remove", ++test_num);
		stat += TestSizetCmp("Find Absent", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 0);
	}
	
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);	
	
	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
	}
	
	for (i = 9; i > 0; --i)
	{
		stat += TestSizetCmp("Find Exist", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 1);
		AVLRemove(avl, (void *)(test + i));
		stat += TestFlow("AVL Remove", ++test_num);
		stat += TestSizetCmp("Find Absent", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 0);
	}
	
	AVLDestroy(avl);
	stat += TestFlow("AVL Destroy", ++test_num);
	
	TestMsgUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}

int Unit4Test(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Get Height";
	size_t test_num = 0;
	size_t i = 0;
	avl_t *avl = NULL;
	int test[] = {6, 3, 8, 2, 0, 7, -17, 200, 1, 5, 67, -300, 4};
	int res[13] = {0};

	TestMsgUnitTitle(unit_name);

	avl = AVLCreate(AVLIntCmp);
	
	stat += TestSizetCmp("AVL Create", ++test_num, (avl == NULL), 0);
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);
	stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), 0);
	stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), -1);

	for (i = 0; i < 1; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), i + 1);
		stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), 0);
	}
	
	
	for (i = 1; i < 3; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), i + 1);
		stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), 1);
	}

	
	for (i = 3; i < 6; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), i + 1);
		stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), 2);
	}
	
	for (i = 6; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), i + 1);
		stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), 3);
	}
		
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 0);	
	
	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Count", ++test_num, AVLCount(avl), 10 - i);
		stat += TestSizetCmp("Find Exist", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 1);
		AVLRemove(avl, (void *)(test + i));
		stat += TestFlow("AVL Remove", ++test_num);
		stat += TestSizetCmp("Find Absent", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 0);
	}
	
	stat += TestSizetCmp("AVL Is Empty", ++test_num, AVLIsEmpty(avl) , 1);	
	
	for (i = 0; i < 10; ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
	}
	
	for (i = 9; i > 0; --i)
	{
		stat += TestSizetCmp("Find Exist", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 1);
		AVLRemove(avl, (void *)(test + i));
		stat += TestFlow("AVL Remove", ++test_num);
		stat += TestSizetCmp("Find Absent", ++test_num, AVLFind(avl, (void *)(test + i), (void **)(res + i)) , 0);
	}
	
	AVLDestroy(avl);
	stat += TestFlow("AVL Destroy", ++test_num);
	
	TestMsgUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}

int Unit5Test(void)
{
	status_t stat = SUCCESS;
	char *unit_name = "Bug detected";
	size_t test_num = 0;
	size_t i = 0;
	avl_t *avl = NULL;
	int test[] = {65, 91, 37, 7, 4, 67, 50, 95, 66, 13, 69, 14};

	TestMsgUnitTitle(unit_name);

	avl = AVLCreate(AVLIntCmp);
	
	for (i = 0; i < (sizeof(test) / sizeof(test[0])); ++i)
	{
		stat += TestSizetCmp("AVL Insert", ++test_num, AVLInsert(avl, &test[i]), 0);
	}

	stat += TestSSizetCmp("Get Height", ++test_num, AVLGetHeight(avl), 3);
	
	AVLDestroy(avl);
	
	stat += TestFlow("AVL Destroy", ++test_num);
	
	TestMsgUnitStat(stat, unit_name);
	
	return (stat != SUCCESS);
}

int TestSizetCmp(char *test_name, size_t test_num, size_t test, size_t res)
{
	status_t stat = SUCCESS;
	
	TestMsgCheckTitle(test_name, test_num);
	
	/* the test */
	stat = (test != res);
	
	TestMsgCheckStat(stat);
	
	return (stat != SUCCESS);			
}

int TestSSizetCmp(char *test_name, size_t test_num, ssize_t test, ssize_t res)
{
	status_t stat = SUCCESS;
	
	TestMsgCheckTitle(test_name, test_num);
	
	/* the test */
	stat = (test != res);
	
	TestMsgCheckStat(stat);
	
	return (stat != SUCCESS);			
}

int TestIntCmp(char *test_name, size_t test_num, int test, int res)
{
	status_t stat = SUCCESS;
	
	TestMsgCheckTitle(test_name, test_num);
	
	/* the test */
	stat = (test != res);
	
	TestMsgCheckStat(stat);
	
	return (stat != SUCCESS);			
}

int TestFlow(char *test_name, size_t test_num)
{	
	TestMsgCheckTitle(test_name, test_num);
	
	TestMsgCheckStat(SUCCESS);
	
	return SUCCESS;			
}

int AVLIntCmp(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

int AVLStructIntCmp(const void *data1, const void *data2)
{
	return (((int_cmp_t *)data1)->cmp - ((int_cmp_t *)data2)->cmp);
}

int SetData(void *data, void *param)
{
	((int_cmp_t *)data)->data = (size_t)param;
	
	return 0;
}

/****************	test's msg functions definitions	***********************/
void TestMsgTestTitle(char *tests_name)
{
	assert(tests_name);
		
	printf("\n\n\033[1;4m%s Tests:\033[m\n", tests_name);
}

void TestMsgTestStat(int stat, char *tests_name)
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

void TestMsgUnitTitle(char *func_name)
{
	assert(func_name);
	
	printf("\n\n\033[1m%s tests:\033[m\n\n", func_name);
}

void TestMsgUnitStat(int stat, char *func_name)
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

void TestMsgCheckTitle(char *tests_name, size_t test_num)
{
	assert(tests_name);
	
	printf("%s test %lu:", tests_name, test_num);
}

void TestMsgCheckStat(int stat)
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



