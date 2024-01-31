/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah                  *
*	Date: 26/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Recursion  			  *
*				                	  *
**************************************/

#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h>		/* size_t */

#include "stack.h"		/* stack API */

typedef struct node
{
	void *data;
	struct node *next;
}node_t;

int IterFibonacci(int element_index);

int RecFibonacci(int element_index);

node_t *RecFlipList(node_t *node);

void RecStackSort(stack_t *stack);

void RecStackSortInsert(stack_t *stack, int *data);
 
size_t RecStrLen(const char *str);

int RecStrCmp(const char *str1, const char *str2);

char *RecStrCpy(char *dst, const char *src);

char *RecStrCat(char *dst, const char *src);

char *RecStrStr(const char *haystack, const char *needle);

#endif /* __RECURSION_H__ */

