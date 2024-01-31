/*************************************
*						       *
*	Developer: Eitan Bellaiche      *	      
*	Date: 12/12/22	                 *
*	Valgrind & GDB passed           *
*	Subject: Quiz Parantheses       *
*				   			  *
**************************************/

#include <stdio.h> /* prinft */	
#include <assert.h> /* assert */
#include <stdlib.h> /* calloc & malloc */ 

#define MATCHING(close, open) (((close) - (open)) <= 2)

typedef struct stack
{
	int top;
	size_t capacity;
	void **items;
}stack_t;

void IsParanthesesBalanced(char *str);
stack_t *StackCreate(size_t capacity);
void StackDestroy(stack_t *stack);
void StackPush(stack_t *stack, void *data);
void *StackPop(stack_t *stack);
int StackIsEmpty(const stack_t *stack);
void *StackPeek(const stack_t *stack);


int main(void)
{
	IsParanthesesBalanced("[()](){[()]()}");
	IsParanthesesBalanced("[{}(])");
	IsParanthesesBalanced("([])");
	IsParanthesesBalanced("[");
	
	return 0;
}

/*************************** IsParanthesesBalanced****************************/

void IsParanthesesBalanced(char *str)
{
	char *to_insert = NULL;
	stack_t *stack = NULL;
	
	assert(NULL != str);
	
	to_insert = str;
	
	stack = StackCreate(8);
	if (NULL == stack)
	{
		return;
	}
	
	while ('\0' != *to_insert)
	{
		if (*to_insert == '{' || *to_insert == '(' || *to_insert == '[')
		{
			StackPush(stack, to_insert);
		}
		else 
		{	
			if (StackIsEmpty(stack))
			{
				printf("Not Balanced\n");
				StackDestroy(stack);
				return;
			}
			
			if (MATCHING(*to_insert, *(char *)StackPeek(stack)))
			{
				StackPop(stack);
			}
			
		}
		++to_insert;
	}
	if (StackIsEmpty(stack))
	{
		StackDestroy(stack);
		printf("Balanced\n");
	}
	else
	{
		printf("Not Balanced\n");
	}
	
}

/****************************** STACK functions *******************************/

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

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (-1 == stack->top);
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return *(stack->items + stack->top);
}




