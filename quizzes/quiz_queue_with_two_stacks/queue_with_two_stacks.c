/*************************************
*				 				     *
*	Developer: Eitan Bellaiche	     *	 
*	Quiz: Queue With Two Stacks;	 *
*				 				     *
**************************************/

#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, free */
#include <assert.h>	/* assert */

#include "stack.h" /* stack API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

struct stack
{
	size_t top;
	size_t capacity;
	void **items;
};

typedef struct queue
{
	size_t capacity;
	size_t size;
	stack_t *stack_to_push;
	stack_t *stack_to_pop;
} queue_t;

enum status
{
	SUCCESS = 0,
	FAIL
};


/*--------- Function Declarations ---------*/
queue_t *QueueCreate(size_t capacity);
void QueueDestroy(queue_t *queue);
void *Dequeue(queue_t *queue);
int Enqueue(queue_t *queue, void *data);
int QueueWithTwoStacksTest(void);

int main (void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tQueueWithTwoStacks Test: \t%s\n", TEST_RES(QueueWithTwoStacksTest()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

int QueueWithTwoStacksTest(void)
{
	queue_t *queue = NULL;
	int values[6] = {5, 2, 4, 1, 3, 6};
	int status = SUCCESS;
	size_t i = 0;
	size_t capacity = 6;
	
	queue = QueueCreate(capacity);
	
	for (i = 0; i < capacity; ++i)
	{
		Enqueue(queue, &values[i]);
	}
	
	for (i = 0; i < capacity; ++i)
	{
		status = (values[i] == (*(int *)Dequeue(queue))) ? SUCCESS : FAIL;
	}
	
	QueueDestroy(queue);
	
	return status;
}

/*------------------------------ Enqueue  ------------------------------------*/

int Enqueue(queue_t *queue, void *data)
{	
	assert(NULL != queue);
	
	StackPush(queue->stack_to_push, data);
	++(queue->size);
	
	return SUCCESS;
}

/*------------------------------ Dequeue  ------------------------------------*/

void *Dequeue(queue_t *queue)
{
	void *data = NULL;
					 
	assert(NULL != queue);
	
	while (!StackIsEmpty(queue->stack_to_push))
	{
		StackPush(queue->stack_to_pop, StackPop(queue->stack_to_push));
	}
	
	data = StackPop(queue->stack_to_pop);
	
	while (!StackIsEmpty(queue->stack_to_pop))
	{
		StackPush(queue->stack_to_push, StackPop(queue->stack_to_pop));
	}
	--(queue->size);
	
	return data;	
}

/*----------------------------- QueueCreate  ---------------------------------*/

queue_t *QueueCreate(size_t capacity)
{
	queue_t *queue = NULL;
	
	assert(0 < capacity);
	
	queue = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->stack_to_push = StackCreate(capacity);
	if (NULL == queue->stack_to_push)
	{
		free(queue);
		queue = NULL;
		
		return NULL;
	}
	
	queue->stack_to_pop = StackCreate(capacity);
	if (NULL == queue->stack_to_pop)
	{
		StackDestroy(queue->stack_to_push);
		
		free(queue);
		queue = NULL;
		
		return NULL;
	}
	
	queue->size = 0;
	
	return queue;		
}

/*----------------------------- QueueDestroy  ---------------------------------*/

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	StackDestroy(queue->stack_to_push);
	StackDestroy(queue->stack_to_pop);

	free(queue);
	queue = NULL;
}






