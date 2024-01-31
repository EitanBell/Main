

#include <stdio.h> 			/* printf */
#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */

typedef struct node 
{
	void *data;
	struct node *child[2];
} node_t;

typedef struct tree
{
	node_t *root;
} tree_t;

typedef struct queue_node queue_node_t;

struct queue_node
{
	void *data;
	queue_node_t *next;
};

typedef struct queue
{
	queue_node_t *front;
	queue_node_t *rear;
	
} queue_t;

typedef enum child
{
	LEFT = 0,
	RIGHT
} child_t;

static int QueueEnqueue(queue_t *queue, void *data);
static void *QueueDequeue(queue_t *queue);
static int QueueIsEmpty(queue_t *queue);

static void LevelBLevelPrint(tree_t *tree, queue_t *queue);

int main(void)
{
	int test[5] = {1, 2, 3, 4, 5};
	tree_t tree = {0};
	node_t tree_arr[5] = {0};
	queue_t queue = {NULL, NULL};
	
	tree_arr[0].data = &test[0];
	tree_arr[0].child[LEFT] = &tree_arr[1];
	tree_arr[0].child[RIGHT] = &tree_arr[2];

	tree_arr[1].data = &test[1];
	tree_arr[1].child[LEFT] = &tree_arr[3];
	tree_arr[1].child[RIGHT] = &tree_arr[4];

	tree_arr[2].data = &test[2];
	tree_arr[2].child[LEFT] = NULL;
	tree_arr[2].child[RIGHT] = NULL;

	tree_arr[3].data = &test[3];
	tree_arr[3].child[LEFT] = NULL;
	tree_arr[3].child[RIGHT] = NULL;

	tree_arr[4].data = &test[4];
	tree_arr[4].child[LEFT] = NULL;
	tree_arr[4].child[RIGHT] = NULL;

	tree.root = &tree_arr[0];
	LevelBLevelPrint(&tree, &queue);	
	
	return 0;
}

static void LevelBLevelPrint(tree_t *tree, queue_t *queue)
{
	node_t *queue_front = NULL;
	
	assert(NULL != tree);
	assert(NULL != queue);	
	
	if (NULL != tree->root)
	{
		QueueEnqueue(queue, tree->root);
	}
	
	while (!QueueIsEmpty(queue))
	{
		queue_front = (node_t *)QueueDequeue(queue);
		
		if (NULL != queue_front->child[LEFT])
		{
			QueueEnqueue(queue, queue_front->child[LEFT]);
		}

		if (NULL != queue_front->child[RIGHT])
		{
			QueueEnqueue(queue,  queue_front->child[RIGHT]);
		}
		
		printf("%d, ", *(int *)queue_front->data);
	}
	printf("\n");
}

static int QueueEnqueue(queue_t *queue, void *data)
{
	queue_node_t *new_node = NULL;
	
	assert(NULL != queue);
	
	new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
	if (NULL == new_node)
	{
		return -1;
	}
	
	if (NULL == queue->front)
	{
		queue->front = new_node;
		queue->rear = new_node;
	}
	else
	{
		queue->rear->next = new_node;
		queue->rear = new_node;
	}
	
	new_node->data = data;
	new_node->next = NULL;
	
	return 0;

}

static void *QueueDequeue(queue_t *queue)
{
	void *data_to_ret = NULL;
	queue_node_t *front_to_deq = NULL;
	
	assert(NULL != queue);
	
	front_to_deq = queue->front;
	data_to_ret = front_to_deq->data;
	
	queue->front = queue->front->next;
	if (NULL == queue->front)
	{
		queue->rear = NULL;
	}
	
	free(front_to_deq);
	front_to_deq = NULL;
	
	return data_to_ret;
}

static int QueueIsEmpty(queue_t *queue)
{
	assert(NULL != queue);
	
	return (NULL == queue->front);
}

