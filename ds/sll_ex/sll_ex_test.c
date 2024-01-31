/*----------------------------------------------------------------------------*/
/* DEVELOPER: ELIRAN TESSLER                                                  */
/* LAST MODIFIED: 			                                                  */
/* WORKSHEET NAME: Linked List Exercises                                      */
/* REVIEWER: Shauli                                                           */
/*----------------------------------------------------------------------------*/

#include <stdio.h> /* printf */

#include "sll_ex.h" /* header file of this source file */

static void FlipTest(void);
static void HasLoopTest(void);
static void FindIntersectionTest(void);
static void FindIntersectionTest2(void);
static void FindIntersectionTest3(void);
static void FindIntersectionTest4(void);

int main(void)
{
	FlipTest();
	HasLoopTest();
	FindIntersectionTest();
	FindIntersectionTest2();
	FindIntersectionTest3();
	FindIntersectionTest4();
	
	return 0;
}

static void FlipTest(void)
{
	int a = 7;
	int b = 5;
	int c = 3;
	node_t head_adr = {0};
	node_t middle_adr = {0};
	node_t tail_adr = {0};
	node_t *tail = &tail_adr;
	node_t *head = &head_adr;
	node_t *middle = &middle_adr;
	
	head->next = middle;
	middle->next = tail;
	tail->next = NULL;
	
	head->data = &a;
	middle->data = &b;
	tail->data = &c;
	
	head = Flip(head);
	
	(head->data == &c) ? printf("Flip SUCCESS\n") : printf("Flip FAIL\n");
}

static void HasLoopTest(void)
{       
    node_t head_node1 = {0};
    node_t second_node1 = {0};
    node_t third_node1 = {0};
    
    node_t head_node2 = {0};
    node_t second_node2 = {0};
    node_t third_node2 = {0};
    
    head_node1.next = &second_node1;
    second_node1.next = &third_node1;
    third_node1.next = &second_node1;
    
    head_node2.next = &second_node2;
    second_node2.next = &third_node2;
    third_node2.next = NULL;
    
    (1 == HasLoop(&head_node1) && 0 == HasLoop(&head_node2)) ?
    printf("HasLoop SUCCESS\n") : printf("HasLoop FAIL\n");
    
}


static void FindIntersectionTest(void)
{
    node_t head1_node = {0};
    node_t second1_node = {0};
    node_t third1_node = {0};
    node_t fourth1_node = {0};
	
    node_t head2_node = {0};
	node_t second2_node = {0};
    node_t third2_node = {0};
	
    node_t tail_node = {0};
    tail_node.next = NULL;
    
    head1_node.next = &second1_node;
    second1_node.next = &third1_node;
    third1_node.next = &fourth1_node;
    fourth1_node.next = &tail_node;
    
    head2_node.next = &second2_node;
    second2_node.next = &third2_node;
    third2_node.next = &second1_node;
    
   
    (&second1_node == (node_t *)FindIntersection(&head1_node, &head2_node))?
    printf("FindIntersection SUCCESS\n") : printf("FindIntersection FAIL\n");
}

static void FindIntersectionTest2(void)
{
    node_t head_node1 = {0};
    node_t second_node1 = {0};
    node_t third_node1 = {0};
    node_t tail_node1 = {0};
    
    node_t head_node2 = {0};
    node_t second_node2 = {0};
    node_t third_node2 = {0};
    
    head_node1.next = &second_node1;
    second_node1.next = &third_node1;
    third_node1.next = &tail_node1;
    tail_node1.next = NULL;
    
    head_node2.next = &second_node2;
    second_node2.next = &third_node2;
    third_node2.next = &head_node1;
    
   	if (&head_node1 == FindIntersection(&head_node1, &head_node2))
   	{
    	printf("FindIntersection 2 SUCCESS\n");
    }
    else
    {
    	printf("FindIntersection 2 FAIL\n");
    }
}

static void FindIntersectionTest3(void)
{
    node_t head_node1 = {0};
    node_t second_node1 = {0};
    node_t third_node1 = {0};
    node_t tail_node1 = {0};
    
    node_t head_node2 = {0};
    node_t second_node2 = {0};
    node_t third_node2 = {0};
    
    head_node1.next = &second_node1;
    second_node1.next = &third_node1;
    third_node1.next = &tail_node1;
    tail_node1.next = NULL;
    
    head_node2.next = &second_node2;
    second_node2.next = &third_node2;
    third_node2.next = &tail_node1;
    
   	(&tail_node1 == FindIntersection(&head_node1, &head_node2))?
    printf("FindIntersection 3 SUCCESS\n") : printf("FindIntersection 3 FAIL\n");
}


static void FindIntersectionTest4(void)
{
    node_t head_node1 = {0};
    node_t second_node1 = {0};
    node_t third_node1 = {0};
    node_t tail_node1 = {0};
    
    node_t head_node2 = {0};
    node_t second_node2 = {0};
    node_t third_node2 = {0};
    
    head_node1.next = &second_node1;
    second_node1.next = &third_node1;
    third_node1.next = &tail_node1;
    tail_node1.next = NULL;
    
    head_node2.next = &second_node2;
    second_node2.next = &third_node2;
    third_node2.next = NULL;
    
   	(NULL == FindIntersection(&head_node1, &head_node2))?
    printf("FindIntersection 4 SUCCESS\n") : printf("FindIntersection 4 FAIL\n");
}

