/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 29/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: AVL  	  				  *
*				                	  *
**************************************/

#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf */

#include "avl.h" /* AVL API */

#define MAX(a,b) ((a) > (b) ? (a) : (b))

enum bool_val
{
	FALSE = 0,
	TRUE
};

#define COUNT (10)

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
} child_dir_t;

typedef struct avl_node
{
	void *data;
	size_t height;
	struct avl_node *child[NUM_OF_CHILDREN];
} avl_node_t;

struct avl
{
	avl_node_t *root;
	avl_cmp_t cmp_func;
};

typedef enum rotation
{
	BALANCED = 0,
	LL,
	RR,
	LR,
	RL
} rotation_t;

typedef int (*rec_foreach_t)(avl_t *avl, avl_node_t *node, 
										  avl_callback_t callback, void *param);
typedef avl_node_t *(*rem_funcs_t)(avl_node_t *node, avl_cmp_t cmp);
typedef avl_node_t *(*rot_funcs_t)(avl_node_t *node);

/*------------- Helper Funcs -------------*/
static avl_node_t *CreateNode(void *data);
static void AVLInit(avl_t *avl, avl_cmp_t new_cmp_func);
static void NodeInit(avl_node_t *node, void *new_data, size_t node_height, 
									avl_node_t *new_left, avl_node_t *new_right);
static void DestroyTree(avl_node_t *node);
static avl_node_t *RecInsert(avl_node_t *node, avl_node_t *new_node,
														    avl_cmp_t cmp_func);

static int RecForEachPreOrder(avl_t *avl, avl_node_t *run_node,
										  avl_callback_t callback, void *param);																	
static int RecForEachInOrder(avl_t *avl, avl_node_t *run_node,
										  avl_callback_t callback, void *param);																	
static int RecForEachPostOrder(avl_t *avl, avl_node_t *run_node,
										  avl_callback_t callback, void *param);		
										  																							
static int RecFindNode(avl_cmp_t cmp, const void *data, void **found_data,
														  	  avl_node_t *node);	
static avl_node_t *RecRemove(avl_node_t *node, const void *data, avl_cmp_t cmp);	
static avl_node_t *RemoveIfNoChild(avl_node_t *node, avl_cmp_t cmp);
static avl_node_t *RemoveIfOneChild(avl_node_t *node, avl_cmp_t cmp);
static avl_node_t *RemoveIfTwoChild(avl_node_t *node, avl_cmp_t cmp);

static size_t RecCount(avl_node_t *node);									   				      
static size_t NumOfChildren(avl_node_t *node);
static avl_node_t *DiveLeft(avl_node_t *node);

static avl_node_t *ReBalanceTree(avl_node_t *node);
static size_t GetHeight(avl_node_t *node);
static void UpdateHeight(avl_node_t *node);
static int BalanceFactor(avl_node_t *node);
static int BalanceRotateStatus(avl_node_t *node);
static avl_node_t *Rotate(avl_node_t *node, child_dir_t direction);
static avl_node_t *DoNotRatate(avl_node_t *node);
static avl_node_t *RotateLL(avl_node_t *node);
static avl_node_t *RotateRR(avl_node_t *node);
static avl_node_t *RotateLR(avl_node_t *node);
static avl_node_t *RotateRL(avl_node_t *node);	

void print2DUtil(avl_node_t *root, int space);
void print2D(avl_t *avl);		
		
/*--------------------------Function LUT Definitions--------------------------*/

static rec_foreach_t foreach_funcs[3] = {&RecForEachPreOrder,
										 &RecForEachInOrder,
						   				 &RecForEachPostOrder};

static rem_funcs_t remove_funcs[3] = {&RemoveIfNoChild,
									  &RemoveIfOneChild,
									  &RemoveIfTwoChild};

static rot_funcs_t rotation_funcs[5] = {&DoNotRatate,
										&RotateLL,
										&RotateRR,
										&RotateLR,
										RotateRL};
		
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*-------------------------------- AVLCreate ---------------------------------*/

avl_t *AVLCreate(avl_cmp_t cmp)
{
		avl_t *new_avl = NULL;
		
		assert(NULL != cmp);
		
		new_avl = (avl_t *)malloc(sizeof(avl_t));
		if (NULL == new_avl)
		{
			return NULL;
		}

		AVLInit(new_avl, cmp);
		
		return new_avl;
}

/*-------------------------------- AVLDestroy --------------------------------*/

void AVLDestroy(avl_t *avl)
{
	assert(NULL != avl);

	if (NULL != avl->root)
	{
		DestroyTree(avl->root);
	}
	
	free(avl);
	avl = NULL;
}

/*------------------------------- AVLInsert ----------------------------------*/

int AVLInsert(avl_t *avl, void *data)
{
	avl_node_t *new_node = NULL;
	
	assert(NULL != avl);

	new_node = CreateNode(data);
	if(NULL == new_node)
	{
		return FAIL;
	}

	avl->root = RecInsert(avl->root, new_node, avl->cmp_func);	
	
	return SUCCESS;
}

/*-------------------------------- AVLRemove ---------------------------------*/

void AVLRemove(avl_t *avl, void *data)
{
	assert(NULL != avl);
	
	avl->root = RecRemove(avl->root, data, avl->cmp_func);
}

/*-------------------------------- AVLFind -----------------------------------*/

int AVLFind(const avl_t *avl, const void *data, void **found_data)
{
	assert(NULL != avl);
	
	return RecFindNode(avl->cmp_func, data, found_data, avl->root);
}

/*------------------------------ AVLForEach ----------------------------------*/

int AVLForEach(avl_t *avl, avl_callback_t callback, void *param, order_t order)
{
	assert(NULL != avl);
	assert(NULL != callback);
	
	return foreach_funcs[order](avl, avl->root, callback, param);
}

/*------------------------------ AVLIsEmpty ----------------------------------*/

int AVLIsEmpty(const avl_t *avl)
{
	assert(NULL != avl);
	
	return (NULL == avl->root);
}

/*----------------------------- AVLGetHeight ---------------------------------*/

ssize_t AVLGetHeight(const avl_t *avl)
{
	assert(NULL != avl);
	
	return (GetHeight(avl->root) - 1);
}

/*------------------------------- AVLCount -----------------------------------*/

size_t AVLCount(const avl_t *avl)
{
	assert(NULL != avl);
	
	return RecCount(avl->root);
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*--------------------------------- AVLInit ----------------------------------*/

static avl_node_t *CreateNode(void *data)
{
	avl_node_t *new_node = NULL;
	
	new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	NodeInit(new_node, data, 1, NULL, NULL);
	
	return new_node;
}

/*--------------------------------- AVLInit ----------------------------------*/

static void AVLInit(avl_t *avl, avl_cmp_t new_cmp_func)
{
	assert(avl);
	assert(new_cmp_func);

	avl->root = NULL;
	avl->cmp_func = new_cmp_func;
}
/*-------------------------------- NodeInit ---------------------------------*/

static void NodeInit(avl_node_t *node, void *new_data, size_t node_height, 
									avl_node_t *new_left, avl_node_t *new_right)
{
	assert(node);
	
	node->data = new_data;
	node->height = node_height;
	node->child[LEFT] = new_left;
	node->child[RIGHT] = new_right;
}

/*------------------------------- DestroyTree --------------------------------*/

static void DestroyTree(avl_node_t *node)
{
	if (NULL != node->child[LEFT])
	{
		DestroyTree(node->child[LEFT]);
	}
	
	if (NULL != node->child[RIGHT])
	{
		DestroyTree(node->child[RIGHT]);
	}
	
	free(node);
	node = NULL;
}

/*-------------------------------- RecInsert ---------------------------------*/

static avl_node_t *RecInsert(avl_node_t *node, avl_node_t *new_node,
														     avl_cmp_t cmp_func)
{
	int cmp_res = 0;
	child_dir_t direction = LEFT;
	
	assert(NULL != cmp_func);
	
	if (NULL == node)
	{
		return new_node;
	}
	
	cmp_res = cmp_func(node->data, new_node->data);
	assert(0 != cmp_res);
	
	direction = (0 > cmp_res);
	node->child[direction] = RecInsert(node->child[direction], new_node,
																	  cmp_func);
	
	return ReBalanceTree(node);
}

/*------------------------------ RecFindNode ---------------------------------*/

static int RecFindNode(avl_cmp_t cmp, const void *data, void **found_data,
														  	   avl_node_t *node)
{
	int cmp_res = 0;
	child_dir_t direction = LEFT;
	
	assert(NULL != cmp);
	
	if (NULL == node)
	{
		return FALSE;
	}
	
	cmp_res = cmp(data, node->data);
	if (0 == cmp_res)
	{
		*found_data = node->data;
		
		return TRUE;
	}
	
	if (0 < cmp_res)
	{
		direction = RIGHT;
	}
	
	return RecFindNode(cmp, data, found_data, node->child[direction]);
}

/*---------------------------- RecForEachPreOrder ----------------------------*/

static int RecForEachPreOrder(avl_t *avl, avl_node_t *run_node,
										   avl_callback_t callback, void *param)
{
	int status = SUCCESS;
	
	status = callback(run_node->data, param);
	
	if ((NULL == run_node) && (SUCCESS == status))
	{
		return SUCCESS;
	}
	
	if ((NULL != run_node->child[LEFT]) && (SUCCESS == status))
	{
		status = RecForEachPreOrder(avl, run_node->child[LEFT], 
															   callback, param);
	}

	if ((NULL != run_node->child[RIGHT]) && (SUCCESS == status))
	{
		status = RecForEachPreOrder(avl, run_node->child[RIGHT], 
															   callback, param);
	}
	
	return status;
}

/*---------------------------- RecForEachInOrder ----------------------------*/

static int RecForEachInOrder(avl_t *avl, avl_node_t *run_node,
										   avl_callback_t callback, void *param)
{
	int status = SUCCESS;
	
	if (NULL == run_node)
	{
		return SUCCESS;
	}
	
	status = RecForEachInOrder(avl, run_node->child[LEFT], callback, param);
	if (SUCCESS != status)
	{
		return status;
	}
	
	status = callback(run_node->data, param);
	if (SUCCESS != status)
	{
		return status;
	}
	
	return RecForEachInOrder(avl, run_node->child[RIGHT], callback, param);
}


/*---------------------------- RecForEachPostOrder ---------------------------*/

static int RecForEachPostOrder(avl_t *avl, avl_node_t *run_node,
										   avl_callback_t callback, void *param)
{
	int status = SUCCESS;
	
	if (NULL == run_node)
	{
		return SUCCESS;
	}
	
	status = RecForEachPostOrder(avl, run_node->child[LEFT], callback, param);
	if (SUCCESS != status)
	{
		return status;
	}

	status = RecForEachPostOrder(avl, run_node->child[RIGHT], callback, param);	
	if (SUCCESS != status)
	{
		return status;
	}
	
	return callback(run_node->data, param);
}


/*-------------------------------- RecCount ----------------------------------*/

static size_t RecCount(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return (RecCount(node->child[LEFT]) + RecCount(node->child[RIGHT]) + 1);
}

/*-------------------------------- RecRemove ---------------------------------*/

static avl_node_t *RecRemove(avl_node_t *node, const void *data,
															 avl_cmp_t cmp_func)
{
	int cmp_res = 0;
	child_dir_t direction = LEFT;
	size_t num_of_children = 0;
	
	assert(NULL != cmp_func);
	
	if (NULL == node)
	{
		return node;
	}
	
	cmp_res = cmp_func(data, node->data);
	
	if (0 == cmp_res)
	{
		num_of_children = NumOfChildren(node);
		node = remove_funcs[num_of_children](node, cmp_func);						
	}
	else 
	{
		if (0 < cmp_res)
		{
			direction = RIGHT;
		}
		node->child[direction] = RecRemove(node->child[direction],
															    data, cmp_func);	
		node = ReBalanceTree(node);  									
	}
	
	return node;
}

/*------------------------------ NumOfChildren -------------------------------*/

static size_t NumOfChildren(avl_node_t *node)
{
	size_t num_of_children = 0;
	
	num_of_children += (NULL != node->child[LEFT]) ? 1 : 0;
	num_of_children += (NULL != node->child[RIGHT]) ? 1 : 0;
	
	return num_of_children;	
}

/*----------------------------- RemoveIfNoChildr -----------------------------*/

static avl_node_t *RemoveIfNoChild(avl_node_t *node, avl_cmp_t cmp)
{
	assert(NULL != node);
	(void)cmp;
	
	free(node);
	node = NULL;
	
	return NULL;
}

/*---------------------------- RemoveIfOneChild ------------------------------*/

static avl_node_t *RemoveIfOneChild(avl_node_t *node, avl_cmp_t cmp)
{
	avl_node_t *child_to_rem = NULL;
	(void)cmp;
	
	assert(NULL != node);
	
	if (NULL != node->child[LEFT])
	{
		child_to_rem = node->child[LEFT];
	}
	else
	{
		child_to_rem = node->child[RIGHT];
	}
	
	NodeInit(node, child_to_rem->data, child_to_rem->height, 
				  		 child_to_rem->child[LEFT], child_to_rem->child[RIGHT]);
	free(child_to_rem);
	child_to_rem = NULL;
	
	return node;
}

/*---------------------------- RemoveIfTwoChild ------------------------------*/

static avl_node_t *RemoveIfTwoChild(avl_node_t *node, avl_cmp_t cmp)
{
	avl_node_t *successor = NULL;
	
	assert(NULL != node);

	successor = DiveLeft(node->child[RIGHT]);
	node->data = successor->data;
	
	node->child[RIGHT] = RecRemove(node->child[RIGHT], successor->data, cmp);
	
	return node;
}

/*-------------------------------- DiveLeft ----------------------------------*/

static avl_node_t *DiveLeft(avl_node_t *node)
{
	assert(NULL != node);
	
	if (NULL == node->child[LEFT])
	{
		return node;
	}
	
	return DiveLeft(node->child[LEFT]);
}

/*------------------------------- GetHeight ----------------------------------*/

static size_t GetHeight(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	return node->height;
}


/*--------------------------- RecBalanceAVLTree ------------------------------*/

static avl_node_t *ReBalanceTree(avl_node_t *node)
{
	rotation_t rotation = BALANCED;
	
	assert(NULL != node);
	
	UpdateHeight(node);
	rotation = BalanceRotateStatus(node);
	
	return rotation_funcs[rotation](node);
}

/*------------------------------ UpdateHeight --------------------------------*/

static void UpdateHeight(avl_node_t *node)
{
	assert(NULL != node);
	
	node->height = MAX(GetHeight(node->child[LEFT]), 
					   GetHeight(node->child[RIGHT])) + 1;
}

/*------------------------------ BalanceFactor -------------------------------*/

static int BalanceFactor(avl_node_t *node)
{
	assert(NULL != node);
	
	return (GetHeight(node->child[LEFT]) - GetHeight(node->child[RIGHT]));
}

/*--------------------------- BalanceRotateStatus ----------------------------*/

static int BalanceRotateStatus(avl_node_t *node)
{
	int rotation = BALANCED;
	
	assert(NULL != node);
	
	if (1 < BalanceFactor(node))
	{
		if (0 > BalanceFactor(node->child[LEFT]))
		{
			rotation = LR;
		}
		else
		{
			rotation = LL;
		}
	}
	else if (-1 > BalanceFactor(node))
	{
		if (0 < BalanceFactor(node->child[RIGHT]))
		{
			rotation = RL;
		}
		else
		{
			rotation = RR;
		}
	}

	return rotation;
}

/*--------------------------------- Rotate -----------------------------------*/

static avl_node_t *Rotate(avl_node_t *node, child_dir_t direction)
{
	avl_node_t *root = node;
	avl_node_t *pivot = NULL;
	child_dir_t rot_dir = direction;
	child_dir_t opp_dir = direction ^ 1;
	
	assert(NULL != node);
	
	pivot = root->child[opp_dir];
	root->child[opp_dir] = pivot->child[rot_dir];
	pivot->child[rot_dir] = root;

	UpdateHeight(root);
	UpdateHeight(pivot);
	
	return pivot;
}

/*-------------------------------- DoNotRatate -------------------------------*/

static avl_node_t *DoNotRatate(avl_node_t *node)
{
	assert(NULL != node);
	
	return node;
}

/*-------------------------------- RotateLL ----------------------------------*/

static avl_node_t *RotateLL(avl_node_t *node)
{
	assert(NULL != node);
	
	return Rotate(node, RIGHT);
}

/*-------------------------------- RotateRR ----------------------------------*/

static avl_node_t *RotateRR(avl_node_t *node)
{
	assert(NULL != node);
	
	return Rotate(node, LEFT);
}

/*-------------------------------- RotateLR ----------------------------------*/

static avl_node_t *RotateLR(avl_node_t *node)
{
	assert(NULL != node);
	
	node->child[LEFT] = Rotate(node->child[LEFT], LEFT);
	return Rotate(node, RIGHT);
}

/*-------------------------------- RotateRL ----------------------------------*/

static avl_node_t *RotateRL(avl_node_t *node)
{
	assert(NULL != node);
	
	node->child[RIGHT] = Rotate(node->child[RIGHT], RIGHT);
	return Rotate(node, LEFT);
}

/*-------------------------------- print2DUtil -------------------------------*/

void print2DUtil(avl_node_t *root, int space)
{
	int i = 0;
	if (root == NULL)
	{
		return;
	}
 
    space += COUNT;
 
    print2DUtil(root->child[RIGHT], space);
 
    printf("\n");
    for (i = COUNT; i < space; i++)
        printf(" ");
    printf("%d(%lu)\n", *(int *)root->data, root->height);
 
    print2DUtil(root->child[LEFT], space);
}

/*---------------------------------- print2D ---------------------------------*/

void print2D(avl_t *avl)
{
    print2DUtil(avl->root, 0);
}
