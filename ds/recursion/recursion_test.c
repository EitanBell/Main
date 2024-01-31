/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah                  *
*	Date: 26/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Recursion  			  *
*				                	  *
**************************************/
					   			        
#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* calloc, free*/
#include <stddef.h>  /* size_t */
#include <assert.h>  /* assert */
#include <string.h> /* strings func check */

#include "stack.h" /* stack API */
#include "recursion.h" /* recursion API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
enum status
{
	SUCCESS,
	FAIL
};

/*------------- Helper Funcs -------------*/
static void RunTests(void);
static int IterFibonacciTest(void);
static int RecFibonacciTest(void);
static int RecStrLenTest(void);
static int RecStrCmpTest(void);
static int RecStrCpyTest(void);
static int RecStrCatTest(void);
static int RecStrStrTest(void);
static int RecFlipListTest(void);
static int RecStackSortTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	printf("\n\t****************** TESTS BEGIN ******************\n\n");
	
	printf("\t\tIterFibonacci Test: \t%s\n", TEST_RES(IterFibonacciTest()));
	printf("\t\tRecFibonacci  Test: \t%s\n", TEST_RES(RecFibonacciTest()));
	printf("\t\tRecStrLen     Test: \t%s\n", TEST_RES(RecStrLenTest()));
	printf("\t\tRecStrCmp     Test: \t%s\n", TEST_RES(RecStrCmpTest()));
	printf("\t\tRecStrCpy     Test: \t%s\n", TEST_RES(RecStrCpyTest()));
	printf("\t\tRecStrCat     Test: \t%s\n", TEST_RES(RecStrCatTest()));
	printf("\t\tRecStrStr     Test: \t%s\n", TEST_RES(RecStrStrTest()));
	printf("\t\tRecFlipList   Test: \t%s\n", TEST_RES(RecFlipListTest()));
	printf("\t\tRecStackSort  Test: \t%s\n", TEST_RES(RecStackSortTest()));
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
}

/*----------------------------- IterFibonacciTest ----------------------------*/

static int IterFibonacciTest(void)
{
	int res1 = 1;
	int res2 = 5;
	int res3 = 21;
	int res4 = 89;
	int is_equal = 0;
	
	is_equal += (res1 == IterFibonacci(1));
	is_equal += (res2 == IterFibonacci(5));
	is_equal += (res3 == IterFibonacci(8));
	is_equal += (res4 == IterFibonacci(11));

	return (is_equal == 4)? SUCCESS: FAIL;
}

/*---------------------------- RecFibonacciTest -----------------------------*/

static int RecFibonacciTest(void)
{
	int res1 = 1;
	int res2 = 5;
	int res3 = 21;
	int res4 = 89;
	int is_equal = 0;
	
	is_equal += (res1 == RecFibonacci(1));
	is_equal += (res2 == RecFibonacci(5));
	is_equal += (res3 == RecFibonacci(8));
	is_equal += (res4 == RecFibonacci(11));

	return (is_equal == 4)? SUCCESS: FAIL;
}

/*------------------------------- RecStrLenTest ------------------------------*/

static int RecStrLenTest(void)
{
	char *str = "Eitan_Bellaiche_AYA_PO";
	
	if (strlen(str) == RecStrLen(str))
	{
		return SUCCESS;
	}
	
	return FAIL;	
}

/*------------------------------- RecStrCmpTest ------------------------------*/

static int RecStrCmpTest(void)
{
	char *str1 = "I want to be HERE";
	char *str2 = "I want to be HERE";
	char *str3 = "I want to be THERE";
	
	if ((0 == RecStrCmp(str1, str2)) && 0 > RecStrCmp(str2, str3))
	{
		return SUCCESS;
	}
	
	return FAIL;
} 

/*------------------------------- RecStrCpyTest ------------------------------*/

static int RecStrCpyTest(void)
{
	char src[30] = "N NA NAH NAHM NAHMAM MEUMAN";
	char dest[30] = "";
	
	char src2[30] = "N NA NAH NAHM NAHMAM MEUMAN";
	char dest2[30] = "";
	
	if (0 == strcmp(strcpy(dest, src), RecStrCpy(dest2, src2)))
	{
		return SUCCESS;
	}

	return FAIL;
}
/*------------------------------- RecStrCatTest ------------------------------*/

static int RecStrCatTest(void)
{
	char dest[50] = "Yonatan akatan";
	char src[25] = "ratz baboker el agan";
	
	char dest2[50] = "Yonatan akatan";
	char src2[25] = "ratz baboker el agan";
	
	if (0 == strcmp(strcat(dest, src), RecStrCat(dest2, src2)))
	{
		return SUCCESS;
	}

	return FAIL;
}

/*------------------------------- RecStrStrTest ------------------------------*/

static int RecStrStrTest(void)
{
	char dest[70] = "sub1sub2sub3SubTextIsMatchingFromSubText";
	char needle[10] = "SubText";
	
	char dest2[70] = "sub1sub2sub3SubTextIsMatchingFromSubText";
	char needle2[10] = "SubText";
	
	if (0 == strcmp(strstr(dest, needle), RecStrStr(dest2, needle2)))
	{
		return SUCCESS;
	}

	return FAIL;
}

/*----------------------------- RecFlipListTest ------------------------------*/

static int RecFlipListTest(void)
{
	int val[4] = {1, 2, 3, 4};
	node_t node_1 = {0};
	node_t node_2 = {0};
	node_t node_3 = {0};
	node_t node_4 = {0};
	node_t *p_node_1 = &node_1;
	node_t *p_node_2 = &node_2;
	node_t *p_node_3 = &node_3;
	node_t *p_node_4 = &node_4;
	
	p_node_1->next = p_node_2;
	p_node_2->next = p_node_3;
	p_node_3->next = p_node_4;
	p_node_4->next = NULL;
	
	p_node_1->data = &val[0];
	p_node_2->data = &val[1];
	p_node_3->data = &val[2];
	p_node_4->data = &val[3];
	
	p_node_1 = RecFlipList(p_node_1);
	
	if ((val[3] == *(int *)p_node_1->data) && p_node_3->next == p_node_2)
	{
		return SUCCESS;
	}
	
	return FAIL;
}

/*------------------------------- RecStackSort ------------------------------*/

static int RecStackSortTest(void)
{
	int not_sorted[10] = {21, 704, 1, -89, 126, 32, 75, 4, 999, 998};
	int sorted[10] = {-89, 1, 4, 21, 32, 75, 126, 704, 998, 999};
	stack_t *stack = NULL;
	int i = 0;
	int status = SUCCESS;
	
	stack = StackCreate(10 * sizeof(int));
	if (NULL == stack)
	{
		return FAIL;
	}

	for (i = 0; i < 10; ++i)
	{
		StackPush(stack, (void *)&not_sorted[i]);
	}
	
	RecStackSort(stack);
	
	for (i = 9; i >= 0; --i)
	{
		if (*(int *)StackPop(stack) != sorted[i])
		{
			status = FAIL;
		}
	}
	
	StackDestroy(stack);
	
	return status;
}

