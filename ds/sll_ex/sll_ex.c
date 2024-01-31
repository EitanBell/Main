/*************************************
*				   			  		 *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:      	                 *
*	Date: 11/12/22	                 *
*	     				       	     *
*	Subject: Signle Linked exercise  *
*				                 	 *
**************************************/

#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#include "sll_ex.h" /* my_slist */

size_t NodesCount(node_t *head);
node_t *ForwardNNodes(node_t *head, size_t n);

enum status
{
	SUCCESS = 0,
	FAIL
};

/*------------------------------Flip-----------------------------------*/

node_t *Flip(node_t *head)
{
    node_t *last_node = NULL;
    node_t *node_to_flip = NULL;
    node_t *temp_node = NULL;
    
    assert(NULL != head);
    
    last_node = head;
    node_to_flip = head->next;

    while (NULL != node_to_flip)
    {
        temp_node = node_to_flip->next;
        node_to_flip->next = last_node;
        last_node = node_to_flip;    
        node_to_flip = temp_node;
    }
    head->next = NULL;
    
    return last_node;
}

/*--------------------------------HasLoop------------------------------------*/

int HasLoop(const node_t *head)
{	
	int status = SUCCESS;
	node_t *fast_step = NULL;
	node_t *slow_step = NULL;
	
	assert(NULL != head);
	
	slow_step = head;
	fast_step = head->next;
	
	while ((NULL != slow_step) && (NULL != fast_step) && 
		   (NULL != fast_step->next)) && !status)
		 
	{
		if (slow_step == fast_step)
		{
			status = FAIL;
		}
		
		slow_step = slow_step->next;
		fast_step = fast_step->next->next;
	}
	return status;
}

/*------------------------------FindIntersection------------------------------*/

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	size_t counter_1 = 0;
	size_t counter_2 = 0;
	size_t count_diff = 0;
	node_t *p_node_1 = NULL;
	node_t *p_node_2 = NULL;
	
	assert(NULL != head_1);
	assert(NULL != head_2);
	
	p_node_1 = head_1;
	p_node_2 = head_2;
	
	counter_1 = NodesCount(head_1);
	counter_2 = NodesCount(head_2);
	
	if (0 < (counter_2 - counter_1))
    {
        count_diff = counter_2 - counter_1;
        p_node_1 = ForwardNNodes(p_node_1, count_diff);
    }
    else
    {
        count_diff = counter_1 - counter_2;
        p_node_2 = ForwardNNodes(p_node_2, count_diff);
    }  
    
    while (((NULL != p_node_1) && (NULL != p_node_2) && (p_node_1 != p_node_2)))
    {
        p_node_1 = p_node_1->next;
        p_node_2 = p_node_2->next;
    }
    
    if ((NULL == p_node_1) || (NULL == p_node_2))
    {
        return NULL;
    }
    else
    {
        return p_node_1;
    }
}

/*------------------------------NodesCount-----------------------------------*/

size_t NodesCount(node_t *head)
{
	size_t counter = 0;
	node_t *runner = NULL;
	
	assert(NULL != head);
	
	runner = head;
	
	while (NULL != runner->next)
	{
		++counter;
		runner = runner->next;
	}
	return counter;
}

/*------------------------------ForwardNNodes--------------------------------*/

node_t *ForwardNNodes(node_t *head, size_t n)
{
	assert(NULL != head);
    
	while (0 < n)
	{
		head = head->next;
		--n;
	}
	return head;
}













