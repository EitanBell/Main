/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 18/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Binary Search Tree   	  *
*				                	  *
**************************************/

#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */
#include <stdlib.h> /* malloc, free */	

#include "ilrd_bst.h" /* binary_search_tree */

#define ITER_TO_NODE(iter) ((bst_node_t *)iter)
#define NODE_TO_ITER(node) ((bst_iter_t)node)

#define GET_ROOT(bst) ((bst->dummy->child[LEFT]))

enum bool_val
{
	FALSE = 0,
	TRUE
};

enum status	
{
	SUCCESS = 0,
	FAIL
};

typedef enum child_dir 
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
}child_dir_t;

struct bst 
{
	bst_node_t *dummy	;
	bst_cmp_t cmp_func;
};

struct bst_node
{
	void *data;
	bst_node_t *parent;
	bst_node_t *child[NUM_OF_CHILDREN];
};

/*------------- Helper Funcs -------------*/
static bst_node_t *CreateNode(void);
static void NodeInit(bst_node_t *node, void *new_data, bst_node_t *new_parent,
							       bst_node_t *new_left, bst_node_t *new_right);
static void BSTInit(bst_t *bst, bst_cmp_t new_cmp_func);									 									 
static void DisconnectLeaf(bst_node_t *node_to_diconnect);
static int IsLeftChild(bst_node_t *child);
static int IsRightChild(bst_node_t *child);
static void ParentUpdate(bst_node_t *parent, bst_node_t *new_child, 
													    bst_node_t *prev_child);
static void GrandsonUpdate(bst_node_t *where);
static void RemoveIfNoChild(bst_node_t *where);
static void RemoveIfOneChild(bst_node_t *where);
static void RemoveIfTwoChild(bst_node_t *where);
								 			 
/*-------------------------------- BSTCreate ---------------------------------*/

bst_t *BSTCreate(bst_cmp_t cmp)
{
		bst_t *new_bst = NULL;
		
		assert(NULL != cmp);
		
		new_bst = (bst_t *)malloc(sizeof(bst_t));
		if (NULL == new_bst)
		{
			return NULL;
		}
		
		new_bst->dummy = CreateNode();
		if (NULL == new_bst->dummy)
		{
			free(new_bst);
			new_bst = NULL;
			
			return NULL;
		}
		
		BSTInit(new_bst, cmp);
		NodeInit(new_bst->dummy, NULL, NULL, NULL, NULL);
		
		return new_bst;
}

/*-------------------------------- BSTDestroy --------------------------------*/

void BSTDestroy(bst_t *bst)
{
	bst_node_t *runner = NULL;
	bst_node_t *node_to_remove = NULL;
	
	assert(NULL != bst);
	
	runner = bst->dummy->child[LEFT];
	
	while (!BSTIsEmpty(bst))
	{
		if (NULL != runner->child[LEFT])
		{
			runner = runner->child[LEFT];
		}	
		else if (NULL != runner->child[RIGHT])
		{
			runner = runner->child[RIGHT];
		}
		else
		{
			node_to_remove = runner;
			runner = runner->parent;
			
			DisconnectLeaf(node_to_remove);
			
			free(node_to_remove);
            node_to_remove = NULL;
		}
	}
	free(bst->dummy);
    bst->dummy = NULL;
	
	free(bst);
	bst = NULL;
}

/*-------------------------------- BSTInsert --------------------------------*/

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
	bst_node_t *new_node = NULL;
	bst_node_t *parent = NULL;
	int res_cmp = -1;
	
	assert(NULL != bst);

	parent = bst->dummy;
	new_node = GET_ROOT(bst);
	
	while (NULL != new_node)
	{
		res_cmp = bst->cmp_func(data, new_node->data);
		assert(0 != res_cmp);
		
		parent = new_node;
		new_node = new_node->child[(0 < res_cmp)];
	}
	
	new_node = CreateNode();
	if (NULL == new_node)
	{
		return BSTEnd(bst);
	}
	
	NodeInit(new_node, data, parent, NULL, NULL);
	parent->child[(0 < res_cmp)] = new_node;
	
	return NODE_TO_ITER(new_node);
}

/*-------------------------------- BSTRemove ---------------------------------*/

void BSTRemove(bst_iter_t where)
{
	size_t num_of_children = 0;
	bst_node_t *where_node = NULL;
	
	assert(NULL != ITER_TO_NODE(where));
	
	where_node = ITER_TO_NODE(where);
	num_of_children = ((NULL != where_node->child[LEFT]) +
					  (NULL != where_node->child[RIGHT]));
					  
	switch (num_of_children)
	{
		case 0:
		{
			RemoveIfNoChild(where_node);
			break;		
		}
		
		case 1:
		{
			RemoveIfOneChild(where_node);
			break;
		}
		
		case 2:
		{
			RemoveIfTwoChild(where_node);
			break;
		}
	}
}

/*-------------------------------- BSTCount ---------------------------------*/

size_t BSTCount(const bst_t *bst)
{
	bst_node_t *runner = NULL;
	size_t counter = 0;
	
	assert(NULL != bst);

	runner = BSTBegin(bst);
	
	while (!BSTIsEqual(NODE_TO_ITER(runner), BSTEnd(bst)))
	{
		++counter;
		runner = BSTNext(runner);
	}
	
	return counter;
}

/*------------------------------- BSTForEach ---------------------------------*/

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_callback_t callback,
																    void *param)
{
	bst_node_t *runner = NULL;
	int status = 0;
	
	assert(NULL != ITER_TO_NODE(from));
	assert(NULL != ITER_TO_NODE(to));
	assert(NULL != callback);
	
	runner = from;
	
	while (!(BSTIsEqual(NODE_TO_ITER(runner), to) && !status))
	{
		status = callback(BSTGetVal(NODE_TO_ITER(runner)), param);
		runner = BSTNext(runner);
	}
	
	return status;
}

/*-------------------------------- BSTIsEmpty --------------------------------*/

int BSTIsEmpty(const bst_t *bst)
{
	assert(NULL != bst);
	
	return (NULL == bst->dummy->child[LEFT]);
}

/*-------------------------------- BSTGetVal ---------------------------------*/

void *BSTGetVal(bst_iter_t iter)
{
	assert(NULL != ITER_TO_NODE(iter));
	
	return (iter->data);
}

/*--------------------------------- BSTEnd -----------------------------------*/

bst_iter_t BSTEnd(const bst_t *bst)
{
	assert(NULL != bst);
	
	return (bst->dummy);
}

/*------------------------------- BSTIsEqual ---------------------------------*/

int BSTIsEqual(bst_iter_t one, bst_iter_t other)
{
	assert(NULL != ITER_TO_NODE(one));
	assert(NULL != ITER_TO_NODE(other));
	
	return (one == other);
}

/*------------------------------- BSTFind ---------------------------------*/

bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
	bst_node_t *runner = NULL;
	int res_cmp = 1;
	
	assert(NULL != bst);
	
	runner = GET_ROOT(bst);
	
	while ((NULL != runner) && (0 != res_cmp))
	{
		res_cmp = bst->cmp_func(data, runner->data);
		if (0 != res_cmp)
		{
			runner = runner->child[(0 < res_cmp)];
		}
	}
	
	if (NULL == runner)
	{
		runner = BSTEnd(bst);
	}
	
	return NODE_TO_ITER(runner);
}

/*------------------------------- BSTBegin -----------------------------------*/

bst_iter_t BSTBegin(const bst_t *bst)
{
	bst_iter_t runner = NULL;
	
	assert(NULL != bst);
	
	if (BSTIsEmpty(bst))
	{
		return NULL;
	}
	
	runner = bst->dummy->child[LEFT];
	
	while (NULL != runner->child[LEFT])
	{
		runner = runner->child[LEFT];
	}
	
	return runner;
}

/*------------------------------- BSTNext -----------------------------------*/

bst_iter_t BSTNext(bst_iter_t current)
{
	bst_node_t *runner = NULL;
	
	assert(NULL != ITER_TO_NODE(current));
	
	runner = ITER_TO_NODE(current);
	
	if (NULL != runner->child[RIGHT])
	{
		runner = runner->child[RIGHT];
		
		while (NULL != runner->child[LEFT])
		{
			runner = runner->child[LEFT];
		}
	}
	else
	{
		while ((NULL != runner->parent->parent) &&
				 	  (runner->parent->child[LEFT] != runner))
		{
			runner = runner->parent;
		}
		runner = runner->parent;
	}
	
	return NODE_TO_ITER(runner);
}

/*------------------------------- BSTPrev -----------------------------------*/

bst_iter_t BSTPrev(bst_iter_t current)
{
	bst_node_t *runner = NULL;
	
	assert(NULL != ITER_TO_NODE(current));
	
	runner = ITER_TO_NODE(current);
	
	if (NULL != runner->child[LEFT])
	{
		runner = runner->child[LEFT];
		
		while (NULL != runner->child[RIGHT])
		{
			runner = runner->child[RIGHT];
		}
	}
	else
	{
		while ((NULL != runner->parent->parent) &&
				 	  (runner->parent->child[RIGHT] != runner))
		{
			runner = runner->parent;
		}
		runner = runner->parent;
	}
	
	return NODE_TO_ITER(runner);
}


/*---------------------------- HELPER FUNCTION -------------------------------*/

/*------------------------------- CreateNode ---------------------------------*/

static bst_node_t *CreateNode(void)
{
	bst_node_t *new_node = NULL; 
	
	new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	return new_node;	
}


/*-------------------------------- NodeInit ---------------------------------*/

static void NodeInit(bst_node_t *node, void *new_data, bst_node_t *new_parent,
									bst_node_t *new_left, bst_node_t *new_right)
{
	assert(node);
	
	node->data = new_data;
	node->parent = new_parent;
	node->child[LEFT] = new_left;
	node->child[RIGHT] = new_right;
}

/*--------------------------------- BSTInit ----------------------------------*/

static void BSTInit(bst_t *bst, bst_cmp_t new_cmp_func)
{
	assert(bst);
	assert(new_cmp_func);

	bst->cmp_func = new_cmp_func;
}

/*----------------------------- RemoveIfNoChild ------------------------------*/

static void RemoveIfNoChild(bst_node_t *where)
{
	assert(NULL != where);
	
	ParentUpdate(where->parent, NULL, where);
	
	free(where);
	where = NULL;
}

/*---------------------------- RemoveIfOneChild ------------------------------*/

static void RemoveIfOneChild(bst_node_t *where)
{
	assert(NULL != where);
	
	if (NULL != where->child[RIGHT])
	{
		ParentUpdate(where->parent, where->child[RIGHT], where);
		where->child[RIGHT]->parent = where->parent;
	}
	else
	{
		ParentUpdate(where->parent, where->child[LEFT], where);
		where->child[LEFT]->parent = where->parent;
	}
	
	free(where);
	where = NULL;
}

/*--------------------------- RemoveIfTwoChild -------------------------------*/

static void RemoveIfTwoChild(bst_node_t *where)
{
	bst_node_t *next_node = NULL;
	
	assert(NULL != where);
	
	next_node = ITER_TO_NODE(BSTNext(NODE_TO_ITER(where)));
	where->data = next_node->data;
	GrandsonUpdate(next_node);
	
	free(next_node);
	next_node = NULL;	
}


/*----------------------------- ParentUpdate ---------------------------------*/

static void ParentUpdate(bst_node_t *parent, bst_node_t *new_child, 
													     bst_node_t *prev_child)
{
	assert(NULL != parent);
	assert(NULL != prev_child);
	
	if (IsLeftChild(prev_child))
	{
		parent->child[LEFT] = new_child;
	}
	else
	{
		parent->child[RIGHT] = new_child;
	}
		
}

/*------------------------------ ChildUpdate ---------------------------------*/

static void GrandsonUpdate(bst_node_t *where)
{
	assert(NULL != where);
	
	if (NULL != where->child[RIGHT])
	{
		ParentUpdate(where->parent, where->child[RIGHT], where);
		where->child[RIGHT]->parent = where->parent;
	}
	else
	{
		ParentUpdate(where->parent, NULL, where);
	}

}

/*------------------------------ ChildUpdate ---------------------------------*/

static void DisconnectLeaf(bst_node_t *node_to_diconnect)
{
	if (IsLeftChild(node_to_diconnect))
	{
		node_to_diconnect->parent->child[LEFT] = NULL;
	}
            
	else if (IsRightChild(node_to_diconnect))
	{
		node_to_diconnect->parent->child[RIGHT] = NULL;
	}
}

/*------------------------------ IsLeftChild ---------------------------------*/

static int IsLeftChild(bst_node_t *child)
{
	assert(NULL != child);
	
	return (child->parent->child[LEFT] == child);
}

/*----------------------------- IsRightChild ---------------------------------*/

static int IsRightChild(bst_node_t *child)
{
	assert(NULL != child);
	
	return (child->parent->child[RIGHT] == child);
}



 
