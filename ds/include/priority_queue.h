/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: priority queue   	  *
*				                 *
**************************************/

/*------------------------------------------------------------------------------
PRIORITY QUEUE:
	A priority queue is a data structure that similar to regular queue(FIFO),
	in which each element additionally has a priority associated with it.
	Note: The funtions 

MAIN FUNCTIONALITY:
	create, Destroy, Enqueue, Dequeue, IsEmpty, Peek, Size, Clear, Erase.

POLICY
	FIFO by Priority
------------------------------------------------------------------------------*/

#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#include <stddef.h>	/* size_t */

typedef struct ilrd_pq ilrd_pq_t;

/*---------------------------------------------------------------------------
Description:
	A comparison function that compares between elements' priorities in the 
     queue.
	
Parameters:
	Two data elements to compare.
		
Return Value:
	Returns 0 if the two elements it received has the same  
	priority.
	Returns a positive value when the first element is greater than the second.  
	Returns a negative value when the second element is greater than the first.		
---------------------------------------------------------------------------*/
typedef int(*pq_cmp_t)(const void *data1, const void *data2);

/*---------------------------------------------------------------------------
Description:
	A comparison function used to determine if two elements are equals. 
     The function returns 1 if the two elements are equal, and 0 if not. 
	
Parameters:
	Data element and param.
		
Return Value:
	1 if the two elements are equal, and 0 if not.		
---------------------------------------------------------------------------*/
typedef int(*pq_is_match_t)(const void *data, void *param);

 /*---------------------------------------------------------------------------
Description:
	The function creates a priority queue.
	
Parameters:
	cmp: A comparison function that determines the priority type of the 
	queue.
		
Return Value:
	Pointer to the created queue or NULL if fails.
	
Complexity: 
	time: O(1)  / space: O(1) 		
---------------------------------------------------------------------------*/
ilrd_pq_t *PQCreate(pq_cmp_t cmp);

/*------------------------------------------------------------------------------
Description:
	The function frees the allocated memory of the priority queue.
	
Parameters:
	pq: the priority queue to free.
	NOTE: passing a NULL will cause an undefined behavior.
		
Return Value:
	None;
	
Complexity: 
	time: O(n)  / space: O(1) 		
------------------------------------------------------------------------------*/
void PQDestroy(ilrd_pq_t *pq);

/*------------------------------------------------------------------------------
Description:
	The function inserts the argument data to the priority queue, in the right  
	location, according to the priority type of queue. 
	
Parameters:
	pq: the queue to which the function inserts the data.
	data: the new data to insert to the queue.
		
Return Value:
	A status for failure(0) or succes(1).
	
Complexity: 
	time: O(n)  / space: O(1) 		
------------------------------------------------------------------------------*/
int PQEnqueue(ilrd_pq_t *pq, void *data);


/*------------------------------------------------------------------------------
Description:
	The function removes the first element of the queue (top priority).
	NOTE: Dequeue from empty queue will cause an undefined behavior.
	
Parameters:
	pq: the queue to remove from.
		
Return Value:
	A status for failure(0) or succes(1).
	
Complexity: 
	time: O(1)  / space: O(1) 		
------------------------------------------------------------------------------*/
void *PQDequeue(ilrd_pq_t *pq); 

/*------------------------------------------------------------------------------
Description:
	The functin checks if the passed priority queue is empty.
	
Parameters:
	pq: the queue to check if is empty.
		
Return Value:
	A status for false(0) or true(1).
	
Complexity: 
	time: O(1)  / space: O(1) 		
------------------------------------------------------------------------------*/
int PQIsEmpty(const ilrd_pq_t *pq);

/*------------------------------------------------------------------------------
Description:
	The functions returns the first value of the priority queue.
	NOTE: Peek from empty queue will cause an undefined behavior.
	
Parameters:
	pq: the queue to get the value from.
		
Return Value:
	A pointer to the value in the first position of the queue.
	
Complexity: 
	time: O(1)  / space: O(1) 		
------------------------------------------------------------------------------*/
void *PQPeek(const ilrd_pq_t *pq); 

/*------------------------------------------------------------------------------
Description:
	The function counts the number of memebers in the passed priority queue.
	
Parameters:
	pq: the queue to count.
		
Return Value:
	The size in the given queue.
	
Complexity: 
	time: O(n)  / space: O(1) 		
------------------------------------------------------------------------------*/
size_t PQSize(const ilrd_pq_t *pq);

/*------------------------------------------------------------------------------
Description:
	The function clears all the memebers in the passed priority queue, such  
	that the queue stays valid for use.
	
Parameters:
	pq: the queue to clear.
		
Return Value:
	None
	
Complexity: 
	time: O(n)  / space: O(1) 		
------------------------------------------------------------------------------*/
void PQClear(ilrd_pq_t *pq); 

/*------------------------------------------------------------------------------
Description:
	The function removes the given data from the priority queue. If the data 
	exists more than once, it will remove its first occurrence.
	
Parameters:
	pq: the queue to erase from.
	data: the data to erase.
	is_match: a pointer to a comparison function.
		
Return Value:
	 The erased data or NULL if data not in queue.
	
Complexity: 
	time: O(n)  / space: O(1) 		
------------------------------------------------------------------------------*/
void *PQErase(ilrd_pq_t *pq, void *data, pq_is_match_t is_match);

#endif 	/* __PRIORITY_QUEUE_H__ */




