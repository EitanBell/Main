/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Matan	                  *
*	Date: 06/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Heap  	  				  *
*				                	  *
**************************************/

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>	/* size_t */

/*-----------------------------------------------------------
Description:
	A complete tree data structure where each node's key value is one of two
	options:
		1. bigger or equal to the key value of its children.
		2. smaller or equal to the key value of its children.	
Main Functions: 
	HeapPush, HeapPop, HeapPeek, HeapRemove
-----------------------------------------------------------*/
typedef struct heap heap_t;

/*-----------------------------------------------------------
Description:
	A function that gets heap_data and new_data as an argument, 
	returns 0 if they are equal, negative number if new_data is bigger and 
	positive number otherwise. 
	This function is used in order to determine the order of members in the
	heap. You can add another param to the function as needed, that can be 
	an additional parameter for the comparison.
-----------------------------------------------------------*/
typedef int(*heap_cmp_t)(const void *heap_data, const void *new_data, 
																   void *param); 

/*-----------------------------------------------------------
Description:
	A function that gets two parameters as an argument, returns 1 if they are 
	equal, and 0 otherwise. This function used in HeapRemove in order to find
	the member to remove from the heap.
-----------------------------------------------------------*/
typedef int(*heap_is_match_t)(const void *data, void *param);

/*-----------------------------------------------------------------------------
Description: 
	Dynamically allocates and initializes a new heap.
Parameters:
	cmp - pointer to heap_cmp_t function.
	param - pointer to a prameter (optional) to send to cmp function.
Return Value:
	Pointer to the new heap.
Complexity: 
	time: O(1) / space: O(1) 
-----------------------------------------------------------------------------*/
heap_t *HeapCreate(heap_cmp_t cmp, void *param);

/*-----------------------------------------------------------------------------
Description: 
	Frees all memory resources of a given heap.
Parameters:
	heap - The heap to destroy.
Return Value:
	None.
Complexity: 
	time: O(1) / space: O(1) 
-----------------------------------------------------------------------------*/
void HeapDestroy(heap_t *heap);

/*-----------------------------------------------------------------------------
Description: 
	Adds a new member to the heap.
	The insertion will be done according to the heap properties.
Parameters:
	heap - the heap to add the new member to.
	data - the new member to add.
Return Value:
	0 on success. 
	1 on fail.
Complexity: 
	time: Amortized O(logn) / space: Amortized O(n) 
-----------------------------------------------------------------------------*/
int HeapPush(heap_t *heap, void *data); 

/*-----------------------------------------------------------------------------
Description: 
	Removes the root of the heap, keeping heap properties.
Parameters:
	heap - Handle to the heap.
Return Value:
	The removed member (former root).
Complexity: 
	time: Amortized O(logn) / space: Amortized O(n) 
-----------------------------------------------------------------------------*/
void *HeapPop(heap_t *heap); 

/*-----------------------------------------------------------------------------
Description:
	Returns the root of the heap.
NOTE!
	- Trying too peek from an empty vector may cause an undifined behavior.
Parameters:
	heap - Handle to the heap.
Return Value:
	The max (or min) value in the heap.
Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
void *HeapPeek(const heap_t *heap);

/*-----------------------------------------------------------------------------
Description: 
	Removes a member from the heap, keeping heap properties. The function will 
	search the member to remove with is_match function provided to it as an 
	argument. 
NOTE!
	- param is an unused paramerter.
	- If there is more than one member that matches, it is unspecified which
	  member will be removed
Parameters:
	heap - Handle to the heap.
	data - the data to remove from the heap.
	is_match - heap_is_match_t function.
	param - unused parameter.
Return Value:
	The removed member.
Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
void *HeapRemove(heap_t *heap, void *data, heap_is_match_t is_match, 
																   void *param);

/*-----------------------------------------------------------------------------
Description:
	Checks if an heap is empty.
Parameters:
	heap - Handle to the heap.
Return Value:
	1 if heap is empty.
	0 otherwise.
Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
int HeapIsEmpty(const heap_t *heap);

/*-----------------------------------------------------------------------------
Description: 
	Counts the number of members in the heap.
Parameters:
	heap - the heap to count members of.
Return Value:
	Number of members in the heap.
Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
size_t HeapSize(const heap_t *heap);


#endif 	/* __HEAP_H__ */



