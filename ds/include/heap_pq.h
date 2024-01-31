/*************************************
*				   			 		 *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Matan                 *
*	Date: 08/02/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: Priority Queue (heap)	 *
*				                	 *
**************************************/

#ifndef __HEAP_PQ_H__
#define __HEAP_PQ_H__

#include <stddef.h>	/* size_t */

/****************************************************************************/
/*																			*/
/*	Priority Queue is a linear data structure that is similar to a queue,	*/
/*	and every element has some priority value associated with it. The 		*/
/*	priority of the elements determines the order in which elements are 	*/
/*	stored and the order in which they are removed). If in any case the 	*/
/*	elements have same priority, they are served as per their ordering in   */
/*	the queue - FIFO.														*/ 
/*																			*/
/*	The main functions of priority queue are enqueue and dequeue.			*/
/*	Enqueue push the element to place by order of priority, which received	*/
/*	by comparison function.													*/														
/*																			*/
/****************************************************************************/


typedef struct ilrd_pq ilrd_pq_t;


/*-----------------------------------------------------------
Description:
	This is the function prototype for the priority criteria.
-----------------------------------------------------------*/
typedef int(*pq_cmp_t)(const void *data1, const void *data2);

/*-----------------------------------------------------------
Description:
	This is the function prototype for function used to
	erase an element in the priority queue.
-----------------------------------------------------------*/
typedef int(*pq_is_match_t)(const void *data, void *param);

/*-----------------------------------------------------------------------------
Description: 
	The function creates a priority queue that's priotized according to
	the compare function recieved from the user.
	
Parameters:
	cmp - the compare function that is used to determine the element position
	in the queue. Notice that enqueue happens when cmp returns a negative
	value.
	
Return Value:
	On success returns a handle to a priority queue. Otherwise, NULL is
	returned.
Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
ilrd_pq_t *PQCreate(pq_cmp_t cmp);

/*-----------------------------------------------------------------------------
Description: 
	The function frees all priority queue's allocated memory.
	
Parameters:
	pq - the handle to a priority queue for destroy. 
	
Return Value:
	The is no return value.

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
void PQDestroy(ilrd_pq_t *pq);

/*-----------------------------------------------------------------------------
Description: 
	The function pushes the element data to the queue according to its
	priority. In case there are elements with the same priority the policy
	is FIFO.
	
Parameters:
	pq - the handle to a priority queue for enqueue. 
	data - a void pointer to the data to be pushed to the queue.
	
Return Value:
	On success, returns success status - 0, otherwise fail status - 1.
	
	returned.
Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
int PQEnqueue(ilrd_pq_t *pq, void *data);


/*-----------------------------------------------------------------------------
Description: 
	The function takes off the priority queue's front element. If the queue is 
	empty, behavior is not defined.
	
Parameters:
	pq - the handle to a priority queue for dequeue. 
	
Return Value:
	The function returns the value stored in the front element which has been
	removed.	

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
void *PQDequeue(ilrd_pq_t *pq); 

/*-----------------------------------------------------------------------------
Description:
	The function checks if the priority queue is empty.

Parameters:
	pq - the handle to a priority queue. 
	
Return Value:
	The function returns 1 if the priority queue is empty. Otherwise returns 0.

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
int PQIsEmpty(const ilrd_pq_t *pq);

/*-----------------------------------------------------------------------------
Description:
	The function returns the value of the element in the front of the priority
	queue.

Parameters:
	pq - the handle to a priority queue. 
	
Return Value:
	The function returns the value stored in the front of the priority
	queue.	

Complexity: 
	time: O(1)  / space: O(1) 
-----------------------------------------------------------------------------*/
void *PQPeek(const ilrd_pq_t *pq); 

/*-----------------------------------------------------------------------------
Description: 
	The function counts the number of elements in the queue. 
	
Parameters:
	pq - the handle to a priority queue. 
	
Return Value:
	The function returns the number of elements stored in the priority queue. 

Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
size_t PQSize(const ilrd_pq_t *pq);

/*-----------------------------------------------------------------------------
Description: 
	The function removes all the data which stored in the queue, but not
	destroy the queue. 
	
Parameters:
	pq - the handle to a priority queue for clear. 
	
Return Value:
	This function are always successful.  

Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
void PQClear(ilrd_pq_t *pq); 

/*-----------------------------------------------------------------------------
Description: 
	The function removes a specific element from the queue that matches 
	the data recieved from the user. The match is tested by the function
	is_match recieved from the user as a pointer to a function, and only the 
	first match (the match with the highest priority) will be removed.
	
Parameters:
	pq - the handle to a priority queue for clear.
	data - a pointer to the data for check matching.
	is_match - pointer to a funtion for comparison.
	
Return Value:
	In case the specific data is found in the queue, returns its value.
	Otherwise, NULL is returned.  

Complexity: 
	time: O(n)  / space: O(1) 
-----------------------------------------------------------------------------*/
void *PQErase(ilrd_pq_t *pq, void *data, pq_is_match_t is_match);

#endif 	/* __HEAP_PQ_H__ */




