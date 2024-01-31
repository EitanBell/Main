/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah                  *
*	Date: 26/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Recursion  			  *
*				                	  *
**************************************/

#include <assert.h>  	/* assert */
#include <stddef.h>  	/* size_t */
#include <stdlib.h> 	/* calloc, free */	
#include <string.h> 	/* strncmp */

#include "stack.h" 		/* stack API */
#include "recursion.h"  /* recursion API */
									 
/*------------------------------ IterFibonacci -------------------------------*/

int IterFibonacci(int element_index)
{
	int num = 1;
	int prev_num = 0;
	int temp = 0;
	int i = 0;
	
	assert(0 < element_index);
	
	for (i = 0; i < element_index; ++i)
	{
		temp = prev_num;
		prev_num = num;
		num += temp;
	}
	return prev_num;
}

/*------------------------------- RecFibonacci -------------------------------*/

int RecFibonacci(int element_index)
{
	assert(0 != element_index);
	
	if (1 == element_index || 2 == element_index)
	{
		return 1;
	}
	
	return ((RecFibonacci(element_index - 1) +
		    (RecFibonacci(element_index - 2))));
}

/*------------------------------- RecFlipList -------------------------------*/

node_t *RecFlipList(node_t *node)
{
	node_t *new_head = NULL;
	
	assert(NULL != node);
	
	if (NULL == node || NULL == node->next)
	{
		return node;
	}
	
	new_head = RecFlipList(node->next);
	node->next->next = node; 
	node->next = NULL;

	return new_head;
}

/*------------------------------- RecStackSort -------------------------------*/

void RecStackSort(stack_t *stack)
{
	int *top_val = NULL;
	
	assert(NULL != stack);
	
	if (!StackIsEmpty(stack))
	{
		top_val = StackPop(stack);
		RecStackSort(stack);
		RecStackSortInsert(stack, top_val);
	}
}

/*---------------------------- RecStackSortInsert ----------------------------*/

void RecStackSortInsert(stack_t *stack, int *data)
{
	int *top_val = NULL;
	
	assert(NULL != stack);
	assert(NULL != data);
	
	if (StackIsEmpty(stack) || (*(int *)data >= *(int *)StackPeek(stack)))
	{
		StackPush(stack, data);
		return;
	}
	
	top_val = StackPop(stack);
	RecStackSortInsert(stack, data);
	StackPush(stack, top_val);
}

/*--------------------------------- RecStrLen --------------------------------*/

size_t RecStrLen(const char *str)
{
	assert(NULL != str);
	
	if ('\0' == *str)
	{
		return 0;
	}
	
	return (RecStrLen(str + 1) + 1);
}

/*--------------------------------- RecStrCmp --------------------------------*/

int RecStrCmp(const char *str1, const char *str2)
{
	assert(NULL != str1);
	assert(NULL != str2);
	
	if ((*str1 != *str2) || ('\0' == *str1) || ('\0' == *str2))
	{
		return (*str1 - *str2);
	}

	return (RecStrCmp(str1 + 1 ,str2 + 1));
}

/*--------------------------------- RecStrCpy --------------------------------*/

char *RecStrCpy(char *dst, const char *src)
{
	assert(NULL != dst);
	assert(NULL != src);	
	
	*dst = *src;
	
	if ('\0' == *src)
	{
		return dst;
	}

	return (RecStrCpy(dst + 1, src + 1) - 1);
}

/*--------------------------------- RecStrCat --------------------------------*/

char *RecStrCat(char *dst, const char *src)
{
	assert(NULL != dst);
	assert(NULL != src);
	
	if ('\0' == *dst)
	{
		return RecStrCpy(dst, src);
	}
	
	return (RecStrCat(dst + 1, src) - 1);
}

/*--------------------------------- RecStrStr --------------------------------*/

char *RecStrStr(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);
	
	if (0 == strncmp(haystack, needle, RecStrLen(needle)))
	{
		return (char *)haystack;
	}
	
	return (RecStrStr(haystack + 1, needle));
}

