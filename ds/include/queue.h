/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Moriya               *
*	Date: 07/12/22	                 *
*	Vlagrind & gdb passed           *
*	Subject: Queue	 			  *
*				                 *
**************************************/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h>		/* size_t */


typedef struct queue queue_t;

/**
 * The function creates a queue.
 * @return The function returns pointer to the created queue or NULL on fail.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
queue_t *QueueCreate(void);

/**
 * The function frees the allocated memory of the queue.
 * passing a NULL will cause an undefined behavior.
 * @param queue - the queue to free.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void QueueDestroy(queue_t *queue);

/**
 * The function inserts the argument data to the end of the queue. 
 * @param queue - the queue to which the function inserts the data.
 * @param data - the new data to insert to the queue.
 * @return a status for failure (0) or succes(1).
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int QueueEnqueue(queue_t *queue, void *data);

/**
 * The function removes the last member of the queue.
 * @param queue - the queue to remove from.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void QueueDequeue(queue_t *queue);

/**
 * The functions returns the first value of the queue.
 * @param queue - the queue to get the value from.
 * @return a pointer to the value in the first position of the queue.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *QueuePeek(const queue_t *queue);

/**
 * The functin checks if the passed queue is empty. 
 * @param queue - the queue to check if empty.
 * @return 1 for true, 0 for false.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int QueueIsEmpty(const queue_t *queue);
    
/**
 * The function counts the number of memebers in the passed queue.
 * @param queue - the queue to count.
 * @return the size in the specified queue.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
size_t QueueSize(const queue_t *queue);

/**
 * The function preforms concatenation of queues - src to dest. 
 * @param dest - the queue to which the src will be concatenat. 
 * @param src - the list to concatenat.
 * @return a pointer to the new updated queue.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
queue_t *QueueAppend(queue_t *dest, queue_t *src);

#endif /* __QUEUE_H__ */








