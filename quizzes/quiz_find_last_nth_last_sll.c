
#include <stdio.h>  /* printf */
#include <assert.h>  /* assert */

typedef struct node
{
	int data;
	struct node *next;
}node_t;

node_t *FindNthLastInSLL(node_t *root, size_t n);
static size_t CountNodes(node_t *root);

int main(void)
{
	node_t node_arr[10];
	node_t *node = NULL;
	size_t i = 0;
	
	for (i = 0; 9 > i; ++i)
	{
		node_arr[i].data = i + 1;
		node_arr[i].next = &node_arr[i + 1]; 
	}
	node_arr[i].data = i + 1;
	node_arr[i].next = NULL;
	
	node = &node_arr[0];
	for (i = 0; node != NULL; ++i)
	{
		printf("%d -> ", node->data);
		node = node->next;
	}
	printf("\n");
	
		printf("value of the 5th node from the last is = %d\n",  
									   FindNthLastInSLL(&node_arr[0], 5)->data);
	
	return 0;
}

node_t *FindNthLastInSLL(node_t *root, size_t n)
{
	node_t *p1 = NULL;
	node_t *p2 = NULL;
	size_t i = 0;
	
	assert(NULL != root);
	assert(CountNodes(root) > n);
	
	p1 = root;
	p2 = root;
	
	for (i = 0; n > i; ++i)
	{
		p2 = p2->next;
	}
	
	while (NULL != p2)
	{
		p1 = p1->next;
		p2 = p2->next;
	}
	
	return p1;
}

static size_t CountNodes(node_t *root)
{
	size_t count = 0;
	node_t *node = NULL;
	
	assert(NULL != root);
	
	node = root;
	
	while (NULL != node)
	{
		++count;
		node = node->next;
	}
	
	return count;
}

