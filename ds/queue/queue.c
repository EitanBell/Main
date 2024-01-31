/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Moriya               *
*	Date: 07/12/22	                 *
*	Vlagrind & gdb passed           *
*	Subject: Queue	 			  *
*				                 *
**************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "queue.h" /* my queue */
#include "slist.h" /* my slist */

struct queue
{
	slist_t *que;
};

typedef enum status
{
	SUCCESS = 0,
	FAIL
} status_t;		

/*------------------------------QueueCreate-----------------------------------*/

queue_t *QueueCreate(void)
{
	queue_t *p_queue = NULL;
	
	p_queue = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == p_queue)
	{
		return NULL;
	}
	p_queue->que = SlistCreate();
	if (NULL == p_queue->que)
	{
		free(p_queue);
		return NULL;
	}
	
	return p_queue;
}

/*------------------------------QueueDestroy----------------------------------*/

void QueueDestroy(queue_t *queue)
{
	assert(queue);
	
	SlistDestroy(queue->que);
	
	free(queue);
	queue = NULL;
}

/*------------------------------QueueEnqueue----------------------------------*/

int QueueEnqueue(queue_t *queue, void *data)
{
	slist_iter_t new_element = NULL;
	
	assert(queue);
	
	new_element = SlistInsertBefore(SlistEnd(queue->que), data);
	if (NULL == new_element)
	{
		return FAIL;
	}
	
	return SUCCESS;
}

/*-------------------------------QueueDequeue----------------------------------*/

void QueueDequeue(queue_t *queue)
{
	assert(queue);
	
	SlistRemove(SlistBegin(queue->que));
}

/*------------------------------SlistIsEmpty----------------------------------*/

int QueueIsEmpty(const queue_t *queue)
{
	assert(queue);
	
	return SlistIsEmpty(queue->que);

}

/*-------------------------------QueueSize-----------------------------------*/

size_t QueueSize(const queue_t *queue)
{
	return SlistCount(queue->que);
}

/*-------------------------------QueuePeek-----------------------------------*/

void *QueuePeek(const queue_t *queue)
{
	assert(queue);
	
	return(SlistGetVal(SlistBegin(queue->que)));
}

/*-------------------------------QueueAppend----------------------------------*/

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	assert(dest);
	assert(src);
	
	dest->que = SlistAppend(dest->que, src->que);
	
	return dest;
}







