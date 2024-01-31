/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	             
*	Subject: DS3 Exam 			   	  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <string.h>		/* strlen */
#include <stdlib.h>		/* malloc, free */

#include "stack.h"		/* stack API */
#include "dlist.h"		/* dlist API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) ((res) ^ 1)

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

enum bool_status	
{
	FALSE = 0,
	TRUE
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

typedef struct int_array
{
    int *data;
    size_t *data_vers;
    size_t size;
    int all_data;
    size_t all_vers;
}int_array_t;

typedef struct min_stack
{
	stack_t *data;
	stack_t *min_stack;
}min_stack_t;
					   			        
/*------------- Funcs -------------*/
static void RunTests(void);
static int_array_t *ArrCreate(size_t size);
static void ArrDestroy(int_array_t *arr);
static void ArrSetVal(int_array_t *arr, int indx, int val);
static int ArrGetVal(int_array_t *arr, int indx);
static void ArrSetAll(int_array_t *arr, int val);
static int FindSumOfTwo(unsigned int *arr, size_t size, unsigned int sum, 
				 					  unsigned int *first, unsigned int *second);
static int IntCmpFunc(const void *a, const void *b);
static int IsCharInArr(char *arr, size_t size, char ch);
static void SwapChars(char *one, char *other);
static void CircShiftArr(char *arr, size_t size, size_t n);
static void ReverseArr(char *left, char *right);
static size_t CountIslands(int *map, int row_size, int column_size);
static int IsInMap(int row_size, int column_size, int row, int column);
static int DrownIslandIfExist(int *map, int row_size, int column_size, int row,
                         											int column);
static min_stack_t *MinStackCreate(size_t capacity);
static void MinStackDestroy(min_stack_t *stack);
static int MinStackPush(min_stack_t *stack, void *data);
static void MinStackPop(min_stack_t *stack);
static void *MinStackMinPeek(min_stack_t *stack);
static int CheckParenthesis(const char *str);
static size_t RecFibonacci(size_t index);
static unsigned int RecMulti(unsigned int one, unsigned int other);
static int AddOne(int num);
static int RecAddOne(int num, int flip_bit);
static void ArrangeListByPivot(dlist_t *list, void *pivot);
static int PivCmpFunc(void *one, void * other);

static bst_node_t *BSTInsert(bst_t *tree, int data);
static bst_node_t *BSTRecInsert(bst_node_t *root, int data);

/*------------- Tests -------------*/
static int ArrayIntTest(void);
static int SumOfTwoTest(void);
static int IsCharInArrTest(void);
static int CircShiftArrTest(void);
static int BitmapSeaShoreTest(void);
static int MinStackTest(void);
static int CheckParenthesisTest(void);
static int RecMultiAndAddTest(void);
static int PivotTest(void);
static int BSTInsertTest(void);
static int BSTRecInsertTest(void);
static int BSTInsertTests(void);

/*------------- main -------------*/
int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	printf("\n\t********************* TESTS BEGIN **********************\n\n");
	
	printf("\t\tQue 1 - Array in O(1) Test\t%s\n", TEST_RES(ArrayIntTest()));
	printf("\t\tQue 2 - SumOfTwo Test\t\t%s\n", TEST_RES(SumOfTwoTest()));
	printf("\t\tQue 3 - IsCharInArr Test\t%s\n", TEST_RES(IsCharInArrTest()));
	printf("\t\tQue 4 - CircShiftArr Test\t%s\n", TEST_RES(CircShiftArrTest()));
	printf("\t\tQue 5 - BitmapSeaShore Test\t%s\n", TEST_RES(BitmapSeaShoreTest()));
	printf("\t\tQue 6 - MinStack Test\t\t%s\n", TEST_RES(MinStackTest()));
	printf("\t\tQue 7 - CheckParenthesis Test\t%s\n", TEST_RES(CheckParenthesisTest()));
	printf("\t\tQue 8 - RecMultiAndAdd Test\t%s\n", TEST_RES(RecMultiAndAddTest()));
	printf("\t\tQue 9 - ArrangeListByPivot Test\t%s\n", TEST_RES(PivotTest()));
	printf("\t\tQue 13 -BSTInsert Iter&Rec Test %s\n", TEST_RES(BSTInsertTests()));
	
	printf("\n\t******************** TESTS FINISHED ********************\n\n");
}
					   			        
/*-------------------- Question 1 - Array implementation ---------------------*/


static int_array_t *ArrCreate(size_t size)
{
	int_array_t *new_arr = NULL;

	assert(0 < size);
	
    new_arr = (int_array_t *)malloc(sizeof(int_array_t));
    if (NULL == new_arr)
	{
		return NULL;
	}
    
    new_arr->data = (int *)calloc(size, sizeof(int));
    if (NULL == new_arr->data)
    {
    	free(new_arr);
    	new_arr = NULL;
    	
        return NULL;
    }
    
    new_arr->data_vers = (size_t *)calloc(size, sizeof(size_t));
    if (NULL == new_arr->data_vers)
    {
    	free(new_arr);
    	new_arr = NULL;
        free(new_arr->data);
        new_arr->data = NULL;
        
        return NULL;
    }
    
    new_arr->size = size;
    new_arr->all_data = 0;
    new_arr->all_vers = 0;
    
    return new_arr;
}

static void ArrDestroy(int_array_t *arr)
{
    free(arr->data);
    arr->data = NULL;
    
    free(arr->data_vers);
    arr->data_vers = NULL;
    
    free(arr);
    arr = NULL;
}

static void ArrSetVal(int_array_t *arr, int indx, int val)
{
    arr->data[indx] = val;
    arr->data_vers[indx] = arr->all_vers;
}

static int ArrGetVal(int_array_t *arr, int indx)
{
    int val = arr->data[indx];
    
    if (arr->all_vers > arr->data_vers[indx])
    {
        val = arr->all_data;
    }
    
    return val;
}

static void ArrSetAll(int_array_t *arr, int val)
{
    arr->all_data = val;
    ++(arr->all_vers);
}

/*-------------------- Question 2 - Array implementation ---------------------*/

static int FindSumOfTwo(unsigned int *arr, size_t size, unsigned int sum, 
				 					  unsigned int *first, unsigned int *second)
{
	unsigned int *start = NULL;
	unsigned int *end = NULL;
	int status = FAIL;
	unsigned int tmp_sum = 0;

	assert(NULL != arr);
	assert(0 < size);

	qsort(arr, size, sizeof(unsigned int), &IntCmpFunc);
	
	start = arr;
	end = arr + size - 1;

	do 
	{
		tmp_sum = *start + *end;
		status = SUCCESS;

		if (sum > tmp_sum)
		{
			++start;
			status = FAIL;
		}
		else if (sum < tmp_sum)
		{
			--end;
			status = FAIL;
		}
	} while (sum != tmp_sum && start < end);

	*first = *start;
	*second = *end;

	return status;
}

static int IntCmpFunc(const void *a, const void *b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	return (*(const int *)a - *(const int *)b);
}

/*------------------------ Question 3 - IsCharInArr --------------------------*/

static int IsCharInArr(char *arr, size_t size, char ch)
{
	int is_found = TRUE;
	size_t i = 0;

	assert(NULL != arr);
	assert(0 < size);

	for (i = 0; size > i; ++i)
	{
		is_found *= arr[i] ^ ch;
	}

	return !is_found;
}

/*------------------------ Question 4 - CircShiftArr -------------------------*/

static void CircShiftArr(char *arr, size_t size, size_t n)
{
	assert(NULL != arr);
	assert(0 < size);

	ReverseArr(arr, (arr + size - 1));
	ReverseArr(arr, (arr + n - 1));
	ReverseArr((arr + n), (arr + size - 1));
}

static void SwapChars(char *one, char *other)
{
	char temp = 0;

	assert(NULL != one);
	assert(NULL != other);

	temp = *one;
	*one = *other;
	*other = temp;
}

static void ReverseArr(char *left, char *right)  
{ 
	assert(NULL != left);
	assert(NULL != right);

    while (left < right) 
    {
        SwapChars(left, right);
        ++left;
        --right;
    }
}   

/*------------------------ Question 5 - BitmapSeaShore -----------------------*/

static size_t CountIslands(int *map, int row_size, int column_size)
{
    int i = 0;
    int j = 0;
    size_t counter = 0;
    
    for (i = 0; row_size > i; ++i)
    {
        for (j = 0; column_size > j; ++j)
        {
            counter += DrownIslandIfExist(map, row_size, column_size, i, j);
        }
    }
    
    return counter;
}

static int IsInMap(int row_size, int column_size, int row, int column)
{
    return ((0 <= row && row < row_size) && 
										 (0 <= column && column < column_size));
}

static int DrownIslandIfExist(int *map, int row_size, int column_size, int row,
                         											 int column)
{
    int idx = row * column_size + column;
    if ((!IsInMap(row_size, column_size, row, column) || (0 == map[idx])))
    {
        return 0;
    }
    
    map[idx] = 0;
    
    DrownIslandIfExist(map, row_size, column_size, row - 1, column - 1);
    DrownIslandIfExist(map, row_size, column_size, row - 1, column);
    DrownIslandIfExist(map, row_size, column_size, row - 1, column + 1);
    
    DrownIslandIfExist(map, row_size, column_size, row, column - 1);
    DrownIslandIfExist(map, row_size, column_size, row, column + 1);
    
    DrownIslandIfExist(map, row_size, column_size, row + 1, column - 1);
    DrownIslandIfExist(map, row_size, column_size, row + 1, column);
    DrownIslandIfExist(map, row_size, column_size, row + 1, column + 1);
    
    return 1;
}

/*---------------------- Question 6 - Min stack in o(1) ----------------------*/

static min_stack_t *MinStackCreate(size_t capacity)
{
	min_stack_t *new_stack = NULL;

	assert(0 < capacity);

	new_stack = (min_stack_t *)calloc(1, sizeof(min_stack_t));
	if (NULL == new_stack)
	{
		return NULL;
	}

	new_stack->data = StackCreate(capacity);
	if (NULL == new_stack->data)
	{
		free(new_stack);
		new_stack = NULL;
	}

	new_stack->min_stack = StackCreate(capacity);
	if (NULL == new_stack->min_stack)
	{
		free(new_stack->data);
		new_stack->data = NULL;
		
		free(new_stack);
		new_stack = NULL;
	}

	return new_stack;
}

static void MinStackDestroy(min_stack_t *stack)
{
	StackDestroy(stack->data);
	StackDestroy(stack->min_stack);
		
	free(stack);
	stack = NULL;
}

static int MinStackPush(min_stack_t *stack, void *data)
{
	assert(NULL != stack);
	assert(NULL != stack->data);
	assert(NULL != stack->min_stack);

	if (StackSize(stack->data) < StackCapacity(stack->data))
	{
		if (StackIsEmpty(stack->min_stack) ||
							 *(int *)StackPeek(stack->min_stack) > *(int *)data)
		{
			StackPush(stack->min_stack, data);
		}

		StackPush(stack->data, data);

		return SUCCESS;
	}

	return FAIL;
}

static void MinStackPop(min_stack_t *stack)
{
	assert(NULL != stack);
	assert(NULL != stack->data);
	assert(NULL != stack->min_stack);

	if (!StackIsEmpty(stack->data))
	{
		if (*(int *)StackPeek(stack->data) == *(int *)StackPeek(stack->min_stack))
		{
			StackPop(stack->min_stack);
		}
		StackPop(stack->data);
	}
}

static void *MinStackMinPeek(min_stack_t *stack)
{
	assert(NULL != stack);
	assert(NULL != stack->data);
	assert(NULL != stack->min_stack);
	
	return StackPeek(stack->min_stack);
}

/*--------------------- Question 7 - BalancedParenthesis ---------------------*/

static int CheckParenthesis(const char *str)
{
	int status = SUCCESS;
    stack_t *check_stack = NULL;
    char *run = NULL;
    
    assert(NULL != str);
    
    run = (char *)str;
    
    check_stack = StackCreate(strlen(str));
    
    while ('\0' != *run)
    {
        if (((('{' == *run) || ('[' == *run) || ('(' == *run) || ('<' == *run))))
        {
        	StackPush(check_stack, run);
        }
        else if ((('}' == *run) || (']' == *run) || (')' == *run) || ('>' == *run)) &&
			    ((*run - *(char *)StackPeek(check_stack) == 2) || 
				(*run - *(char *)StackPeek(check_stack) == 1)))
        {
        	StackPop(check_stack);
        }
        
    	++run;
    }
    
    status = BOOL_TO_STATUS(StackIsEmpty(check_stack));
    
    StackDestroy(check_stack);
    
    return status;
}

/*-------------------------- Question 8 - Recursive --------------------------*/

static size_t RecFibonacci(size_t index)
{
	if (1 >= index)
	{
		return index;
	}

	return (RecFibonacci(index - 1) + RecFibonacci(index - 2));
}

static unsigned int RecMulti(unsigned int one, unsigned int other)
{
	if (1 == other)
	{
		return one;
	}

	return (one + RecMulti(one, (other - 1)));
}

static int AddOne(int num)
{
	return RecAddOne(num, 1);
}

static int RecAddOne(int num, int flip_bit)
{
	if (0 == (num & flip_bit))
	{
		return num ^ flip_bit;
	}

	return RecAddOne((num ^ flip_bit), (flip_bit << 1));
}

/*----------------- Question 9 - Arrange Linked List By Pivot ----------------*/

static void ArrangeListByPivot(dlist_t *list, void *pivot)
{
	dlist_iter_t run = NULL;
	dlist_iter_t temp = NULL;
	dlist_iter_t pivot_node = NULL;

	assert(NULL != list);

	pivot_node = DlistPushBack(list, pivot);
	run = DlistBegin(list);

	while (!DlistIterIsEqual(run, pivot_node))
	{
		if (0 < PivCmpFunc(DlistGetVal(run), pivot))
		{
			temp = run;
			run = DlistNext(run);

			DlistPushBack(list, DlistGetVal(temp));
			DlistRemove(temp);
		}
		else
		{
			run = DlistNext(run);
		}
	}

	DlistRemove(pivot_node);
}

static int PivCmpFunc(void *one, void * other)
{
	return (*(int *)one - *(int *)other);
}


/*------------------- Question 13 - BSTInsert Iter & Rec ----------------------*/

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


/*--------------------------------- TESTS ------------------------------------*/

/*----------------- Test Question 1 - Flip singly linked list ----------------*/

static int ArrayIntTest(void)
{
    int status = SUCCESS;
    size_t i = 0;
    int_array_t *arr = ArrCreate(50);

    ArrSetAll(arr, 3);
    
    for (i = 0; i < 50; ++i)
    {
        status += !(3 == ArrGetVal(arr, i));
    }

    ArrSetVal(arr, 10, 10);
    
    if (10 != ArrGetVal(arr, 10))
    {
        status = FAIL;
    }
    
    ArrDestroy(arr);
    arr = NULL;

    return status;
}

/*---------------------- Test Question 2 - SumOfTwoTest ----------------------*/

static int SumOfTwoTest(void)
{
	unsigned int arr[] = {3, 5, 7, 10, 3, 9, 7, 2, 4, 1};
	unsigned int num1 = 0;
	unsigned int num2 = 0;
	int status = FAIL;

	if (0 == FindSumOfTwo(arr, 10, 12, &num1, &num2))
	{
		status = SUCCESS;
	}
	
	if ((2 != num1) || (10 != num2))
	{
		status = FAIL;
	}

	return status;
}

/*---------------------- Test Question 3 - IsCharInArrTest -------------------*/

static int IsCharInArrTest(void)
{
	char arr[] = {'E', 'i', 't', 'a', 'n', 'B'};
	int status = FAIL;

	if (TRUE == IsCharInArr(arr, 6, 'n') && FALSE == IsCharInArr(arr, 6, 'A'))
	{
		status = SUCCESS;
	}

	return status;
}

/*------------------- Test Question 4 - CircShiftArrTest ---------------------*/

static int CircShiftArrTest(void)
{
	char arr[] = {'E', 'i', 't', 'a', 'n', 'B'};
	int status = FAIL;

	CircShiftArr(arr, 6, 3);
	if (('a' == arr[0]) && ('E' == arr[3]))
	{
		status = SUCCESS;
	}

	return status;
}

/*--------------------- Test Question 5 - BitmapSeaShore ---------------------*/

static int BitmapSeaShoreTest(void)
{
	int status = SUCCESS;
	
	int map1[20] = {1,0,0,1,
                    0,1,0,1,
                    0,0,0,0,
                    1,1,0,1};
                    
	size_t islands_num_1 = 4;
	
	int map2[36] = {1,0,0,0,0,1,
	                1,1,0,0,1,1,
	                0,1,1,1,0,1,
	                0,0,0,0,0,0,
	                0,1,1,0,0,1,
	                0,1,0,0,1,0,};
	                
	size_t islands_num_2 = 3;
	
	int map3[25] = {1,0,0,0,1,
	                0,1,0,1,0,
	                0,1,1,1,0,
	                0,1,0,1,0,
	                1,0,0,0,1};
	                
	size_t islands_num_3 = 1;
	
	status += BOOL_TO_STATUS(islands_num_1 == CountIslands(map1, 5, 4));
	
	status += BOOL_TO_STATUS(islands_num_2 == CountIslands(map2, 6, 6));
	
	status += BOOL_TO_STATUS(islands_num_3 == CountIslands(map3, 5, 5));
	
	return status;
}

/*------------------- Test Question 6 - Min stack in o(1) --------------------*/

static int MinStackTest(void)
{
	min_stack_t *test_stack = {0};
	size_t capacity = 15;
	int test_buffer[15] = {2, 3, -7, 5, 12 -8, 9, 4, 8, 0, -3, -5, 9, 11, 18};
	size_t i = 0;
	int status = 1;

	test_stack = MinStackCreate(capacity);
	
	for (i = 0; i < capacity; ++i)
	{
		MinStackPush(test_stack, &test_buffer[i]);
	}
	
	for (i = 0; i < capacity/2; ++i)
	{
		MinStackPop(test_stack);
	}
	
	if (-7 == *(int *)MinStackMinPeek(test_stack))
	{
		status = 0;
	} 
													
	MinStackDestroy(test_stack);
	
	return status;
}

/*------------------ Test Question 7 - CheckParenthesisTest ------------------*/

static int CheckParenthesisTest(void)
{
	int status_counter = 0;
	
	status_counter += CheckParenthesis("{}<>{}([])");
	status_counter += CheckParenthesis("{}{[]}{()}");
	status_counter += CheckParenthesis("{)>");	
	status_counter += CheckParenthesis("<(a{a}cd)>");
	

	return BOOL_TO_STATUS(1 == status_counter);
}

/*----------------------- Test Question 8 - Recursive ------------------------*/

static int RecMultiAndAddTest(void)
{
	int status = FAIL;

	if (101 == AddOne(100) && 1000 == AddOne(999) && 99 == RecMulti(9, 11) &&
		13 == RecFibonacci(7))
	{
		status = SUCCESS;
	}

	return status;
}

/*-------------- Test Question 9 - Arrange Linked List By Pivot --------------*/

static int PivotTest(void)
{
	dlist_t *list = DlistCreate();
	dlist_iter_t iter = NULL;
	int num1 = 12;
	int num2 = 3;
	int num3 = 7;
	int num4 = 9;
	int num5 = 10;
	int status = FAIL;

	DlistPushFront(list, &num1);
	DlistPushFront(list, &num2);
	DlistPushFront(list, &num3);
	DlistPushFront(list, &num4);
	
	ArrangeListByPivot(list, &num5);
	iter = DlistPrev(DlistEnd(list));

	if (12 == *(int *)DlistGetVal(iter))
	{
		status = SUCCESS;
	}

	iter = DlistBegin(list);
	
/*	size_t i = 0;*/
/*	for (i = 0; 4 > i; ++i)*/
/*	{*/
/*		printf("%d, ", *(int *)DlistGetVal(iter));*/
/*		iter = DlistNext(iter);*/
/*	}*/

	DlistDestroy(list);
	return status;
}

/*------------------------ Test Question 13 - BSTInsert -----------------------*/

static int BSTInsertTests(void)
{
	return BSTInsertTest() + BSTRecInsertTest();
}

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

