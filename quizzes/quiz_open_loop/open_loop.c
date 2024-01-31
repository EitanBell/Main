/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:      	            *
*	Date: 11/12/22	                 *
*	     				       *
*	Subject: Signle Linked exercise *
*				                 *
**************************************/

#include <assert.h> /* assert */
#include <stdio.h> /* printf */

#include "slist.h" /* my_slist */

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" :\
							    "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")

typedef struct node
{
	void *data;
	struct node *next;
}node_t;

int HasLoop(const node_t *head);
int OpenLoopInSLL(const node_t *head);

int main(void)
{
    int status = 2;    
    
    node_t head_node = {0};
    node_t second_node = {0};
    node_t third_node = {0};
    
    node_t head_node1 = {0};
    node_t second_node1 = {0};
    node_t third_node1 = {0};
    
    head_node.next = &second_node;
    second_node.next = &third_node;
    third_node.next = &second_node;
    
    head_node1.next = &second_node1;
    second_node1.next = &third_node1;
    third_node1.next = NULL;
    
    if (1 == HasLoop((const node_t *)&head_node)
    	 && 0 == HasLoop((const node_t *)&head_node1))
    {
        status -= 1;
    }    
    
    status += OpenLoopInSLL(&head_node);
    
    if (NULL == third_node.next)
    {
    	status -= 1;
    }
    printf("\n******************* TESTS BEGIN ********************\n\n");
    
    printf("\tTest Result: \t\t%s\n", TEST_RES(status));
    
    printf("\n****************** TESTS FINISHED ******************\n\n");
    return status;
}




/*--------------------------------HasLoop------------------------------------*/

int HasLoop(const node_t *head)
{

	node_t *fast_step = NULL;
	node_t *slow_step = NULL;
	int status = 0;
	
	assert(NULL != head);
	
	fast_step = (node_t *)head->next;
	slow_step = (node_t *)head;
	
	while (NULL != slow_step  && NULL != fast_step
			&& NULL != fast_step->next && !status)
	{
		if (slow_step == fast_step)
		{
			status = 1;
		}	
		
		slow_step = slow_step->next;
		fast_step = fast_step->next->next;
	}
	
	return status;
}

/*------------------------------OpenLoopInSLL----------------------------------*/

int OpenLoopInSLL(const node_t *head)
{
	node_t *back_node = NULL;
	node_t *curr_node = NULL;
	int status = 1;
	
	assert(head);
	
	back_node = (node_t *)head;
	curr_node = (node_t *)head->next;
	
	if (HasLoop(head))
	{
		while (status)
		{
		 	back_node = back_node->next;
		 	curr_node = back_node->next;
		 	
			while (back_node->next != curr_node->next && status)
		 	{
		 		if (back_node == curr_node->next)
		 		{
		 			status = 0;
		 		}
		 	}	
		}
		
		curr_node->next = NULL;	
	}
	return status;
}







