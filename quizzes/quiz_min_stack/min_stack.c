/*************************************
*				 				     *
*	Developer: Eitan Bellaiche	     *	 
*	Quiz: Min Stack In O(1);		 *
*				 				     *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */

#include "stack.h" /* stack API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

typedef struct min_stack
{
	stack_t *stack;
	stack_t *min_stack;
} min_stack_t;

enum status
{
	SUCCESS = 0,
	FAIL
};


static void MinStackPush(min_stack_t *min_stack, int *data);
static void *MinStackPop(min_stack_t *min_stack);
int MinStackTest(void);

int main (void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tMinStack Test: \t%s\n", TEST_RES(MinStackTest()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

int MinStackTest(void)
{
	min_stack_t test_stack = {0};
	size_t capacity = 10;
	int buffer[10] = {7, 2, -9, 10, 1, 5, -1, 4, 3, 21};
	size_t i = 0;
	int status = SUCCESS;
	
	test_stack.stack = StackCreate(capacity);
	if (NULL == test_stack.stack)
	{
		return FAIL;
	}
	
	test_stack.min_stack = StackCreate(capacity);
	
	for (i = 0; i < capacity; ++i)
	{
		MinStackPush(&test_stack, &buffer[i]);
	}
	
	for (i = 0; i < (capacity / 2); ++i)
	{
		MinStackPop(&test_stack);
	}
	
	status = (*(int *)StackPeek(test_stack.min_stack) == -9) ? SUCCESS : FAIL;	
	
	StackDestroy(test_stack.stack); 
	StackDestroy(test_stack.min_stack);
	
	return status;
}

static void MinStackPush(min_stack_t *min_stack, int *data)
{
	assert(NULL != min_stack);
	assert(NULL != data);
	
	StackPush(min_stack->stack, data);
	
	if (StackIsEmpty(min_stack->min_stack) || 
		*data < *(int *)StackPeek(min_stack->min_stack))
	{
		StackPush(min_stack->min_stack, data);
	}
}

static void *MinStackPop(min_stack_t *min_stack)
{
	assert(NULL != min_stack);
	
	if (StackPeek(min_stack->stack) == StackPeek(min_stack->min_stack)) 
	{
		StackPop(min_stack->min_stack);
	}
	
	return StackPop(min_stack->stack);
}

