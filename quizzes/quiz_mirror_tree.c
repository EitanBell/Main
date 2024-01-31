/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 16/2/23	                  *
*	Subject: Mirror a tree            *
**************************************/

#include <stdio.h> /* printf */

#define COUNT (10)

typedef struct node
{
	struct node *children[2];
	void *data;
}node_t;

void MirrorBinaryTree(node_t *node);
void print2DUtil(node_t* root, int space);

int main(void)
{
	int data[5] = {5,4,7,1,2};
	node_t arr[5] = {0};
	
	arr[0].children[0] = &arr[1];
	arr[0].children[1] = &arr[2];
	arr[0].data = &data[0];
	
	arr[1].children[0] = &arr[3];
	arr[1].children[1] = &arr[4];
	arr[1].data = &data[1];
	
	arr[2].children[0] = NULL;
	arr[2].children[1] = NULL;
	arr[2].data = &data[2];
	
	arr[3].children[0] = NULL;
	arr[3].children[1] = NULL;
	arr[3].data = &data[3];
	
	arr[4].children[0] = NULL;
	arr[4].children[1] = NULL;
	arr[4].data = &data[4];
	
	printf("Before Mirror:\n");
	print2DUtil(&arr[0], 0);
	
	MirrorBinaryTree(&arr[0]);
	
	printf("After Mirror:\n");
	print2DUtil(&arr[0], 0);
	
	return 0;
}

void MirrorBinaryTree(node_t *node)
{
	node_t *temp = NULL;
	
	if (NULL == node)
	{
		return ;
	}
	
	MirrorBinaryTree(node->children[0]);
	MirrorBinaryTree(node->children[1]);
	
	temp = node->children[0];
	node->children[0] = node->children[1];
	node->children[1] = temp;
}

void print2DUtil(node_t* root, int space)
{
	int i = 0;
	if (root == NULL)
	{
		return;
	}
 
    space += COUNT;
 
    print2DUtil(root->children[1], space);
 
    printf("\n");
    for (i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", *(int *)root->data);
 
    print2DUtil(root->children[0], space);
}



