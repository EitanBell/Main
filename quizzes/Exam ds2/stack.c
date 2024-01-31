/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Tal       *
*	Date: 29/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Stack API	         *
*				     *
**************************************/

#include <stdio.h> /* prinft */
#include <stdlib.h> /* calloc & malloc */ 
#include <assert.h> /* asseer */

#include "stack.h"

struct stack
{
	int top;
	size_t capacity;
	void **items;
};

stack_t *StackCreate(size_t capacity)
{
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	if (NULL == stack)
	{
		return NULL;
	}
	
	stack->top = -1;
	stack->capacity = capacity;
	stack->items = calloc(capacity, sizeof(void *));
	if (NULL == stack->items)
	{
		return NULL;
	}

	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack->items);
	stack->items = NULL;
	
	free(stack);
	stack = NULL;
}

void StackPush(stack_t *stack, void *data)
{
	assert(NULL != stack);
	assert(NULL != data);
	
	++(stack->top);
	*(stack->items + stack->top) = data;	
}

void *StackPop(stack_t *stack)
{
	void *data = NULL;
	
	assert(NULL != stack);
	
	data = *(stack->items + stack->top);
	*(stack->items + stack->top) = NULL;
	--(stack->top);
	
	return data;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return *(stack->items + stack->top);
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (stack->top + 1);
}
int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (-1 == stack->top);
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (stack->capacity);
}









