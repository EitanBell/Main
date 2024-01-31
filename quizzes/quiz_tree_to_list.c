/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 25/01/23	                  *
*	Subject: GetNonRepeatingRandom    *
**************************************/

	#include <stdio.h>		/* printf */
	#include <assert.h>		/* assert */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
typedef enum child
{
	LEFT = 0,
	RIGHT = 1
} child_t;

typedef struct node 
{
	void *data;
	struct node *child[2];
} node_t;

static int Test(void);
static node_t *BSTToSortedList(node_t *root);
static void RecBSTToSortList(node_t *node, node_t **head, node_t **tail);


int main (void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tBSTToSortedList Test: \t%s\n", TEST_RES(Test()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

static int Test(void)
{
	int status = 0;
	int test[7] = {10, 6, 14, 4, 8, 12, 16};
	int res[7] = {4, 6, 8, 10, 12, 14, 16};
	node_t tree[7] = {0};
	node_t *head = NULL;
	node_t *run = NULL;	
	size_t i = 0;
	
	tree[0].data = &test[0];
	tree[0].child[LEFT] = &tree[1];
	tree[0].child[RIGHT] = &tree[2];

	tree[1].data = &test[1];
	tree[1].child[LEFT] = &tree[3];
	tree[1].child[RIGHT] = &tree[4];

	tree[2].data = &test[2];
	tree[2].child[LEFT] = &tree[5];
	tree[2].child[RIGHT] = &tree[6];

	tree[3].data = &test[3];
	tree[3].child[LEFT] = NULL;
	tree[3].child[RIGHT] = NULL;

	tree[4].data = &test[4];
	tree[4].child[LEFT] = NULL;
	tree[4].child[RIGHT] = NULL;

	tree[5].data = &test[5];
	tree[5].child[LEFT] = NULL;
	tree[5].child[RIGHT] = NULL;

	tree[6].data = &test[6];
	tree[6].child[LEFT] = NULL;
	tree[6].child[RIGHT] = NULL;

	head = BSTToSortedList(&tree[0]);
	run = head;
	
	for (i = 0 ; i < 7; ++i)
	{
		status += !(res[i] == *(int *)run->data);
		run = run->child[RIGHT];
	}
	
	return status;
}

static node_t *BSTToSortedList(node_t *root) 
{
    node_t *head = NULL;
    node_t *tail = NULL;
    
    assert(NULL != root);
   
    RecBSTToSortList(root, &head, &tail);
    
    return head;
}

static void RecBSTToSortList(node_t *node, node_t **head, node_t **tail) 
{
    if (NULL == node) 
    {
        return;
    }
    
    RecBSTToSortList(node->child[LEFT], head, tail);
    
    if (NULL == *tail) 
    {
		*head = node;
	} 
	else 
    {
		(*tail)->child[RIGHT] = node;
		node->child[LEFT] = *tail;
    }
    
	*tail = node;
    
	RecBSTToSortList(node->child[RIGHT], head, tail);
}
 

