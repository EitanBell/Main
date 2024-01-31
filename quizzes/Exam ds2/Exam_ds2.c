/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	             
*	Subject: DS2 Exam 			   	  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <string.h>		/* strlen */
#include <stdlib.h>		/* malloc, free */

#include "stack.h"		/* stack API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

typedef struct slist_node
{
	void *data;
	struct slist_node *next;
}slist_node_t;

typedef struct slist
{
	slist_node_t *tail;
	slist_node_t *head;
}slist_t;

enum status	
{
	SUCCESS = 0,
	FAIL
};

enum child_dir 
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
};

typedef struct bst_node
{
	int data;
	struct bst_node *parent;
	struct bst_node *child[NUM_OF_CHILDREN];
}bst_node_t;

typedef struct bst
{
	bst_node_t *root;
}bst_t;

					   			        
/*------------- Funcs -------------*/
static void RunTests(void);
static int LargestSubArrSum(int *arr, size_t size, size_t *left, size_t *right);
static int SortStack(stack_t *stack);
static int SortCharsInFile(const char *file_name, char *result);
static int CmpFunc(const void *a, const void *b);
static bst_node_t *BSTInsert(bst_t *tree, int data);
static bst_node_t *BSTRecInsert(bst_node_t *root, int data);
static void BSTFixedArrInsert(int **tree, int data);
static void ReverseStr(char *str);
static void RecReverseStr(char *str, size_t start, size_t end);
static void Swap(char *a, char *b);
static void StringPermutations(char *str);
static void RecStringPerm(char *str, size_t start, size_t end);
static void StackSortInsert(stack_t *stack, int *data);
static void RecStackSort(stack_t *stack);
static void RecStackSortInsert(stack_t *stack, int *data);

/*------------- Tests -------------*/
static int FlipTest(void);
static int LargestSumTest(void);
static int SortStackTest(void);
static int SortCharsInFileTest(void);
static int BSTInsertTest(void);
static int BSTRecInsertTest(void);
static int ReverseStrTest(void);
static void StringPermutationsTest(void);
static int StackSortInsertTest(void);

/*------------- main -------------*/
int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	printf("\n\t********************* TESTS BEGIN **********************\n\n");
	
	printf("\t\tQue 1 - Flip Test\t\t%s\n", TEST_RES(FlipTest()));
	printf("\t\tQue 2 - MaxSubArrSum Test\t%s\n", TEST_RES(LargestSumTest()));
	printf("\t\tQue 3 - SortStack Test\t\t%s\n", TEST_RES(SortStackTest()));
	printf("\t\tQue 4 - SortCharsInFile Test\t%s\n", TEST_RES(SortCharsInFileTest()));
	printf("\t\tQue 5 - BSTInsert Test\t\t%s\n", TEST_RES(BSTInsertTest()));
	printf("\t\tQue 5 - BSTRecInsert Test\t%s\n", TEST_RES(BSTRecInsertTest()));	
	printf("\t\tQue 7 - ReverseString Test\t%s\n", TEST_RES(ReverseStrTest()));
	printf("\t\tQue 8 - StringPermutations Test\n");
	StringPermutationsTest();
	printf("\t\tQue 10 - StackSortInsert Test\t%s\n", TEST_RES(StackSortInsertTest()));
		
	printf("\n\t******************** TESTS FINISHED ********************\n\n");
}
					   			        
/*------------------- Question 1 - Flip singly linked list -------------------*/

static slist_node_t *Flip(slist_node_t *head)
{
	slist_node_t *cur = NULL;
	slist_node_t *prev = NULL;
	slist_node_t *next = NULL;
	
	assert(NULL != head);
	
	cur = head;
	
	while (NULL != cur)
	{
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	
	return prev;
}

static slist_node_t *RecFlip(slist_node_t *node)
{
	slist_node_t *next_node = NULL;
	
	if (NULL == node || NULL == node->next)
	{
		return node;
	}
	
	next_node = RecFlip(node->next);
	node->next->next = node;
	node->next = NULL;
	
	return next_node;
}

/*------------------- Question 2 - Largest sum of sub array ------------------*/

static int LargestSubArrSum(int *arr, size_t size, size_t *left, size_t *right)
{
	int sum = 0;
	int tmp_sum = 0;
	size_t i = 0;
	size_t left_indx = 0;
	
	assert(NULL != arr);
	assert(0 < size);
	
	for (i = 0; i < size; ++i)
	{
		tmp_sum += arr[i];
	}
	
	for (i = 0; size > i; ++i)
	{
		tmp_sum += arr[i];
		
		if (sum < tmp_sum)
		{
			sum = tmp_sum;
			*left = left_indx;
			*right = i;
		}	
		
		if (0 > tmp_sum)
		{
			tmp_sum = 0;
			left_indx = i + 1;
		}
	}
	return sum;
}

/*------------------------ Question 3 - Sort a stack -------------------------*/

static int SortStack(stack_t *stack)
{
	stack_t *tmp_stack = NULL;
	int *poped_elem = NULL;
	int status = SUCCESS;
	
	assert(NULL != stack);
	
	tmp_stack = StackCreate(StackCapacity(stack));
	if (NULL == tmp_stack)
	{
		return FAIL;
	}
	
	while (!StackIsEmpty(stack))
	{
		poped_elem = (int *)StackPop(stack);
		
		while (!StackIsEmpty(tmp_stack) && 
			   *poped_elem < *(int *)StackPeek(tmp_stack))
		{
			StackPush(stack, StackPop(tmp_stack));
		}
		
		StackPush(tmp_stack, poped_elem);
	}
	
	while (!StackIsEmpty(tmp_stack))
	{
		StackPush(stack, (int *)StackPop(tmp_stack));
	}
	
	StackDestroy(tmp_stack);
	
	return status;
}

/*---------------------- Question 4 - Sort char in file ----------------------*/

static int SortCharsInFile(const char *file_name, char *result)
{
	FILE *fp = NULL;
	size_t res_size = 0;
	size_t status = SUCCESS;
	
	assert(NULL != file_name);
	assert(NULL != result);
	
	res_size = strlen(result);
	
	fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		return FAIL;
	}
	
	status = fread(result, sizeof(char), res_size, fp);
	if (res_size != status)
	{
		fclose(fp);
		fp = NULL;
		
		return FAIL;
	}
	
	qsort(result, res_size, sizeof(char), &CmpFunc);
	
	fclose(fp);
	fp = NULL;
	
	return SUCCESS;
}

static int CmpFunc(const void *a, const void *b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	return (*(char *)a - *(char *)b);
}

/*------------------- Question 5 - BSTInsert Iter & Rec ----------------------*/

static bst_node_t *BSTInsert(bst_t *tree, int data)
{
	bst_node_t *root = NULL;
	bst_node_t *new_node = NULL;
	bst_node_t *parent = NULL;
	size_t direction = 0;
	
	assert(NULL != tree);
	
	root = tree->root;
	new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	while (NULL != root)
	{
		parent = root;
		direction = root->data < data;
		root = root->child[direction];
	}
	
	if (NULL != parent)
	{
		parent->child[parent->data < data] = new_node;
		new_node->parent = parent;
		new_node->data = data;
		new_node->child[LEFT] = NULL;
		new_node->child[RIGHT] = NULL;
	}
	
	return new_node;
}

static bst_node_t *BSTRecInsert(bst_node_t *root, int data)
{
	int cmp_res = 0;
	
	if (NULL == root)
	{
		bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
		if (NULL == new_node)
		{
			return NULL;
		}
		new_node->data = data;
		new_node->child[LEFT] = NULL;
		new_node->child[RIGHT] = NULL;
		
		return new_node;
	}
	
	cmp_res = (root->data < data);
	root->child[cmp_res] = BSTRecInsert(root->child[cmp_res], data);
			
	return root;
}

static void BSTFixedArrInsert(int **tree, int data)
{
	size_t i = 0;
	int res = 0;
	
	assert(NULL != tree);
	
	while (NULL != tree[i])
	{
		res = **(tree + i) - data;
		
		if (0 < res)
		{
			i = 2 * i + 1;	/* left child */
		}
		else
		{
			i = 2 * i + 2;	/* right child */
		}
	}
	
	**(tree + i) = data;
}

/*------------------ Question 7 - Recursive Reverse string -------------------*/

static void ReverseStr(char *str)
{
	assert(NULL != str);
	
	RecReverseStr(str, 0, strlen(str) - 1);
}

static void RecReverseStr(char *str, size_t start, size_t end)
{
	assert(NULL != str);
	
	if (start >= end)
	{
		return;
	}
	
	Swap(&str[start], &str[end]);
	RecReverseStr(str, start + 1, end - 1);
}

static void Swap(char *a, char *b)
{
	char tmp = 0;
	
	assert(NULL != a);
	assert(NULL != b);
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*--------------------- Question 8 - StringPermutations ----------------------*/

static void StringPermutations(char *str)
{
	assert(NULL != str);
	
	RecStringPerm(str, 0, (strlen(str) - 1));
}

static void RecStringPerm(char *str, size_t start, size_t end)
{
	size_t i = 0;
	
	assert(NULL != str);
	
	if (end == start)
	{
		printf("%s, ", str);
	}
	else
	{
		for (i = start; end >= i; ++i)
		{
			Swap((str + start), (str + i));
			RecStringPerm(str, (start + 1), end);
			Swap((str + start), (str + i));
		}
	}
}

/*----------------------- Question 10 - Rec Sort Stack -----------------------*/

static void StackSortInsert(stack_t *stack, int *data)
{
	assert(NULL != stack);
	
	StackPush(stack, data);
	
	RecStackSort(stack);
}

static void RecStackSort(stack_t *stack)
{
	void *peek_elem = NULL;

	assert(NULL != stack);
    
	if (!StackIsEmpty(stack))
    {
        peek_elem = StackPop(stack);
        
        RecStackSort(stack);
        RecStackSortInsert(stack, peek_elem);
    }
}

static void RecStackSortInsert(stack_t *stack, int *data)
{
	void *peek_temp = NULL;
    
    assert(NULL != stack);
    assert(NULL != data);

    if (StackIsEmpty(stack) || *data > *(int *)StackPeek(stack))
    {
        StackPush(stack, data);
        return;
    }
    
    peek_temp = StackPop(stack);
    RecStackSortInsert(stack, data);
    
    StackPush(stack, peek_temp);
}



/*--------------------------------- TESTS ------------------------------------*/

/*----------------- Test Question 1 - Flip singly linked list ----------------*/

static int FlipTest(void)
{
	int a = 1;
	int b = 4;
	int c = 7;
	int d = 8;
	
	slist_node_t node1 = {0};
	slist_node_t node2 = {0};
	slist_node_t node3 = {0};
	slist_node_t node4 = {0};
	
	slist_node_t *p_node1 = &node1;
	slist_node_t *p_node2 = &node2;
	slist_node_t *p_node3 = &node3;
	slist_node_t *p_node4 = &node4;
	
	p_node1->data = &a;
	p_node2->data = &b;
	p_node3->data = &c;
	p_node4->data = &d;
	
	p_node1 = RecFlip(Flip(p_node1));

	if (a == *(int *)(p_node1->data))
	{
		return SUCCESS;
	}
	
	return FAIL;
}

/*---------------- Test Question 2 - Largest sum of sub array ----------------*/

static int LargestSumTest(void)
{
	int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
	size_t size = sizeof(arr) / sizeof(int);
	size_t left = 0;
	size_t right = 0;
	
	if (6 == LargestSubArrSum(arr, size, &left, &right))
	{
		return SUCCESS;
	}
	
	return FAIL;
}

/*------------------------ Test Question 3 - Sort Stack ----------------------*/

static int SortStackTest(void)
{
	stack_t *stack = NULL;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int i = 1;
	size_t status = SUCCESS;
	
	
	stack = StackCreate(5);
	
	StackPush(stack, &a);
	StackPush(stack, &d);
	StackPush(stack, &b);
	StackPush(stack, &c);
	StackPush(stack, &e);
	 
	SortStack(stack);
	
	while (!StackIsEmpty(stack))
	{
		if (i != *(int *)StackPop(stack))
		{
			status = FAIL;
		}
		
		++i;
	}
	
	StackDestroy(stack);
	
	return status;
}

/*------------------- Test Question 4 - Sort chars in a file -----------------*/

static int SortCharsInFileTest(void)
{
	size_t i = 0;
	char *result = NULL;
	
	result = (char *)malloc(sizeof(char) * 74);
	if (NULL == result)
	{
		return FAIL;
	}
	
	if (FAIL == SortCharsInFile("CharsFile.txt", result))
	{
		return FAIL;
	}
	
	for (i = 0; i < 7; ++i)
	{
		printf("%c", result[i]);
	}
	
	free(result);
	result = NULL;
	
	return SUCCESS;
}

/*------------------------ Test Question 5 - BSTInsert -----------------------*/

static int BSTInsertTest(void)
{
	int status = 1;
	bst_t tree = {0};
	bst_node_t root = {0};
	bst_node_t *res_left = NULL;
	bst_node_t *res_right = NULL;
	bst_node_t *grand_child = NULL;
	
	tree.root = &root;
	root.data = 5;
	
	res_left = BSTInsert(&tree, 2);
	res_right = BSTInsert(&tree, 10);
	
	grand_child = BSTInsert(&tree, 11);
	
	if (NULL != res_left && 
		NULL != res_right &&
		NULL != grand_child &&
		5 == res_left->parent->data &&
		5 == res_right->parent->data &&
		10 == grand_child->parent->data)
	{
		status = 0;
	}
	
	free(res_left);
	res_left = NULL;
	
	free(res_right);
	res_right = NULL;
	
	free(grand_child);
	grand_child = NULL;
	
	return status;
}

static int BSTRecInsertTest(void)
{
	int status = FAIL;
	bst_node_t root = {0};
	bst_node_t *root_ptr = &root;
	bst_node_t *res_left = NULL;
	bst_node_t *res_right = NULL;
	bst_node_t *grand_child = NULL;
	
	root.data = 5;
	
	root_ptr = BSTRecInsert(root_ptr, 2);
	root_ptr = BSTRecInsert(root_ptr, 10);
	
	root_ptr = BSTRecInsert(root_ptr, 11);
	
	res_left = root_ptr->child[LEFT];
	res_right = root_ptr->child[RIGHT];
	grand_child = root_ptr->child[RIGHT]->child[RIGHT];
	
	if (NULL != res_left && 2 == res_left->data &&
		NULL != res_right && 10 == res_right->data &&
		NULL != grand_child && 11 == grand_child->data)
	{
		status = SUCCESS;
	}
	
	free(res_left);
	res_left = NULL;
	
	free(res_right);
	res_right = NULL;
		
	free(grand_child);
	grand_child = NULL;
	
	return status;
}

/*---------------- Question 7 Test- Recursive Reverse string -----------------*/

static int ReverseStrTest(void)
{
	int status = SUCCESS;
	char str[] = {'E', 'i', 't', 'a', 'n', 'B', 'e', 'l', 'l'};
	
	ReverseStr(str);
	
	if ('l' != str[0] || 'l' != str[1] || 'E' != str[8])
	{
		status = FAIL;
	}
	
	return status;
}

/*------------------- Question 8 Test- StringPermutations --------------------*/

static void StringPermutationsTest(void)
{
	char str[] = {'t', 'e', 's', 't'};
	
	StringPermutations(str);
	printf("\n");
}

/*-------------------- Question 10 Test- Rec Sort Stack ----------------------*/

static int StackSortInsertTest(void)
{
	stack_t *stack = NULL;
	int status = SUCCESS;
	int i = 1;
	int a = 7;
	int b = 3;
	int c = 1;
	int d = 9;
	int e = 5;
	
	stack = StackCreate(5);
	
	StackSortInsert(stack, &c);
	StackSortInsert(stack, &e);
	StackSortInsert(stack, &d);
	StackSortInsert(stack, &b);
	StackSortInsert(stack, &a);
	 
	SortStack(stack);
	
	while (!StackIsEmpty(stack))
	{
		if (i != *(int *)StackPop(stack))
		{
			status = FAIL;
		}
		
		i += 2;
	}
	
	StackDestroy(stack);
	
	return status;
}



