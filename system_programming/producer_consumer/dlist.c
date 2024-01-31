/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Gali                 *
*	Date: 17/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: doubly Linked List	  *
*				                 *
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dlist.h" /* dlist */

#define ITER_TO_NODE(iter) ((dlist_node_t *)iter)
#define NODE_TO_ITER(node) ((dlist_iter_t)node)
#define IS_DUMMY(node) (NULL == ((node)->next) || (NULL == (node)->prev))

typedef struct dlist_node
{
	void *data;
	dlist_iter_t prev;
	dlist_iter_t next;
}dlist_node_t;

struct dlist
{
	dlist_iter_t tail;
	dlist_iter_t head;
};	
enum bool_val
{
	FALSE,
	TRUE = 1
};

enum status
{
	SUCCESS,
	FAIL = 1
};

static dlist_node_t *GetDummy(dlist_node_t *where);
static dlist_node_t *CreateNode(void *data, dlist_node_t *prev,
												dlist_node_t *next);
static void RemoveNode(dlist_node_t *node);												

/*------------------------------DlistCreate-----------------------------------*/

dlist_t *DlistCreate(void)
{
	dlist_node_t *head_dummy = NULL;
	dlist_node_t *tail_dummy = NULL;
	
	dlist_t *new_list = (dlist_t *)malloc(sizeof(dlist_t));
	if (NULL == new_list)
	{
		return NULL;
	}
	
	tail_dummy = CreateNode(new_list, NULL, NULL);
	if (NULL == tail_dummy)
	{
		free(new_list);
		new_list = NULL;
		
		return NULL;
	}
	
	head_dummy = CreateNode(new_list, NULL, tail_dummy);
	if (NULL == head_dummy)
	{
		free(tail_dummy);
		tail_dummy = NULL;
		
		free(new_list);
		new_list = NULL;
		
		return NULL;
	}

	tail_dummy->prev = head_dummy;
	
	new_list->head = head_dummy;
	new_list->tail = tail_dummy;
	
	return new_list;
}

/*------------------------------DlistRemove---------------------------------*/

dlist_iter_t DlistRemove(dlist_iter_t where)
{
	dlist_iter_t where_next = NULL;
	
	assert(NULL != ITER_TO_NODE(where)); 
	
	where_next = DlistNext(where);
	
	RemoveNode(ITER_TO_NODE(where));
	
	return where_next;
}

/*------------------------------DlistPopFront---------------------------------*/

void *DlistPopFront(dlist_t *list)
{
	void *head_val = NULL;
	
	assert(NULL != list);
	
	head_val = DlistGetVal(DlistBegin(list));
	DlistRemove(DlistBegin(list));
	
	return head_val;
}

/*------------------------------DlistPopBack---------------------------------*/

void *DlistPopBack(dlist_t *list)
{
	void *tail_prev_val = NULL;
	
	assert(NULL != list);
	
	tail_prev_val = DlistGetVal(DlistPrev(DlistEnd(list)));
	DlistRemove(DlistPrev(DlistEnd(list)));
	
	return tail_prev_val;
}

/*------------------------------DlistPushFront--------------------------------*/

dlist_iter_t DlistPushFront(dlist_t *list, void *data)
{
	assert(NULL != list);
	assert(NULL != data);
	
	return ((dlist_iter_t)DlistInsertBefore(DlistBegin(list), data));
}

/*------------------------------DlistPushBack---------------------------------*/

dlist_iter_t DlistPushBack(dlist_t *list, void *data)
{
	assert(NULL != list);
	assert(NULL != data);
	
	return ((dlist_iter_t)DlistInsertBefore(DlistEnd(list), data));
}

/*------------------------------DlistDestroy---------------------------------*/

void DlistDestroy(dlist_t *list)
{	
	assert(NULL != list);
	
	while (!DlistIsEmpty(list))
	{
		DlistRemove(DlistBegin(list));
	}
	
	free(list->head);
	list->head = NULL;
	
	free(list->tail);
	list->tail = NULL;
	
	free(list);
	list = NULL;
}

/*------------------------------DlistIsEmpty---------------------------------*/

int DlistIsEmpty(const dlist_t *list)
{
	assert(NULL != list);
	
	return (IS_DUMMY(DlistBegin(list)));

}

/*------------------------------DlistBegin---------------------------------*/

dlist_iter_t DlistBegin(const dlist_t *list)
{
	assert(NULL != list);
	
	return DlistNext(NODE_TO_ITER(list->head));
}


/*------------------------------DlistEnd---------------------------------*/

dlist_iter_t DlistEnd(const dlist_t *list)
{
	assert(NULL != list);
	
	return (NODE_TO_ITER(list->tail));
}


/*------------------------------DlistNext---------------------------------*/

dlist_iter_t DlistNext(dlist_iter_t current)	
{
	assert(NULL != current);
	
	return (NODE_TO_ITER(ITER_TO_NODE(current)->next));
}


/*------------------------------DlistPrev---------------------------------*/

dlist_iter_t DlistPrev(dlist_iter_t current)
{
	assert(NULL != current);
	
	return (NODE_TO_ITER(ITER_TO_NODE(current)->prev));
}


/*------------------------------DlistInsertBefore-----------------------------*/

dlist_iter_t DlistInsertBefore(dlist_iter_t where, void *data)
{
	dlist_node_t *new_node = NULL;
	
	assert(NULL != where);
	assert(NULL != data);
	
	new_node = CreateNode(data, where->prev, where);
	if (NULL == new_node)
	{
		return (NODE_TO_ITER(GetDummy(where)));
	}
	DlistPrev(where)->next = new_node;
	where->prev = new_node;

	return (NODE_TO_ITER(new_node));
}

/*---------------------------------DlistCount---------------------------------*/

size_t DlistCount(const dlist_t *list)
{
	size_t count = 0;
	dlist_iter_t iter = NULL;
	
	assert(NULL != list);
	
	iter = DlistBegin(list);
	
	while (DlistEnd(list) != iter)
	{
		iter = DlistNext(iter);
		++count;
	}
	
	return count;
}



/*------------------------------DlistIterIsEqual------------------------------*/

int DlistIterIsEqual(const dlist_iter_t one, const dlist_iter_t other)
{
	assert(NULL != one);
	assert(NULL != other);
	
	return (one == other);
}

/*---------------------------------DlistSetVal--------------------------------*/

dlist_iter_t DlistSetVal(dlist_iter_t iter, void *data)
{
	assert(NULL != iter);
	assert(NULL != data);
	
	ITER_TO_NODE(iter)->data = data;
	
	return iter;
}

/*---------------------------------DlistGetVal--------------------------------*/

void *DlistGetVal(const dlist_iter_t iter)
{
	assert(NULL != iter);
	
	return (ITER_TO_NODE(iter)->data);
}

/*---------------------------------DlistFind--------------------------------*/

dlist_iter_t DlistFind(const dlist_iter_t from, const dlist_iter_t to, 
					   dlist_is_match_t is_match, void *param)
{	
	dlist_iter_t runner = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	
	runner = from;
	
	while (runner != to && !is_match(runner->data, param))
	{
		runner = DlistNext(runner);
	}
	
	return runner;
}	


/*---------------------------------DlistForEach------------------------------*/

int DlistForEach(dlist_iter_t from, dlist_iter_t to, 
			     dlist_do_action_t do_action, void *param)
{
	int status = SUCCESS;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != do_action);
		
	while (from != to && SUCCESS == status)
	{
		status = do_action(from->data, param);
		from = DlistNext(from);
	}
	
	return status;
}


/*---------------------------------DlistMultiFind-----------------------------*/

int DlistMultiFind(dlist_t *result_list ,const dlist_iter_t from,
			const dlist_iter_t to, dlist_is_match_t is_match, void *param)
{
	int status = SUCCESS;
	
	dlist_iter_t runner = NULL;

	assert (NULL != result_list);	
	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != is_match);
	
	runner = from;
	while (((runner = DlistFind(runner, to, is_match, param)) != to) &&
			SUCCESS == status)
	{
		status = (NULL == DlistPushFront(result_list, runner->data)->data);
		runner = DlistNext(runner);
	}
	
	return status;
}


/*---------------------------------DlistSplice--------------------------------*/

dlist_iter_t DlistSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	dlist_node_t *to_prev = NULL;
	
	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != where);
	
	to_prev = DlistPrev(to);
	
	from->prev->next = to;
	to->prev = DlistPrev(from);
	
	from->prev = DlistPrev(where);
	where->prev->next = from;
	
	where->prev = to_prev;
	to_prev->next = where;
	
	return to_prev;
}



/*------------------------------HELPER FUNCTIONS------------------------------*/

/*--------------------------------DlistGetTail--------------------------------*/

static dlist_node_t *CreateNode(void *data, dlist_node_t *prev,
												 dlist_node_t *next)
{
	dlist_node_t *node = NULL;
	
	assert(NULL != data);
	
	node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if (NULL == node)
	{
		return NULL;
	}
	
	node->data = data;
	node->next = next;
	node->prev = prev;
	
	return node;
}

static dlist_node_t *GetDummy(dlist_node_t *where)
{
	assert(NULL != where);
	
	while (NULL != where->data)
	{
		where = DlistNext(where);
	}
	
	return where;
}
static void RemoveNode(dlist_node_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;

	free(node);
	node = NULL;
}




