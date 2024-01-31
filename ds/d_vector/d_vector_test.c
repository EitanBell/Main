/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Ariel B.    	       *
*	Date: 05/12/22	                 *
*	Valgrind & GDB passed           *
*	Subject: Dynamic Vector	       *
*				                 *
**************************************/
#include <stdio.h>        /* printf    */

#include "d_vector.h"   

#define TEST_RES(status) (status ? "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m": "\xe2\x9c\x97\033[0;31mFAIL\033[0m")

static void RunTests(void);
static int VectorCreateTest(void);
static int VectorPushBackTest(void);
static int VectorResizeTest(void);
static int VectorPopBackTest(void);
static int VectorLoopTest(void);
/*static int VectorLoopMegaTest(void);*/

int main(void)
{
    RunTests();

    return 0;
}

static void RunTests(void)
{
    printf("\n******************LET THE TESTS BEGIN******************\n\n");
    
    printf("VectorCreate TEST:\t\t\t%s\n", 
            TEST_RES(VectorCreateTest()));
            
    printf("VectorPushBack TEST:\t\t\t%s\n", 
            TEST_RES(VectorPushBackTest()));
            
    printf("VectorResize TEST:\t\t\t%s\n", 
            TEST_RES(VectorResizeTest()));
            
    printf("VectorPop TEST:\t\t\t\t%s\n", 
            TEST_RES(VectorPopBackTest()));

	printf("VectorLoop TEST:\t\t\t%s\n", 
			TEST_RES(VectorLoopTest()));
               
    printf("\n******************TESTS FINISHED!******************\n\n");
}

static int VectorCreateTest(void)
{
    int val = 4;
    int capacity = 50;
    vector_t *victor = VectorCreate(capacity, sizeof(val));    
    if (NULL != victor)
    {
        VectorDestroy(victor);
        return 1;
    }
    else
    {
        VectorDestroy(victor);
        return 0;
    }
}

static int VectorPushBackTest(void)
{
    int val = 4;
    int *data = &val;
    vector_t *victor = VectorCreate(10, sizeof(val));
    victor = VectorPushBack(victor, data);
    if (4 == *(int *)(VectorGetAccessElement(victor, 0)))
    {
        VectorDestroy(victor);
        return 1;
    }
    else
    {
        VectorDestroy(victor);
        return 0;
    }
}

static int VectorResizeTest(void)
{
	vector_t *victor = VectorCreate(10, sizeof(int));
	victor = VectorResize(victor, 20);
	
	if (20 == VectorCapacity(victor))
	{
		VectorDestroy(victor);
		return 1;
	}
	else
	{
		VectorDestroy(victor);
		return 0;
	}
}

static int VectorPopBackTest(void)
{
	int val = 4;
	int val1 = 7;
	int *data = &val;
	int *data1 = &val1;
	vector_t *victor = VectorCreate(25, sizeof(val));
	victor = VectorPushBack(victor,(void *)data);
	victor = VectorPushBack(victor, (void *)data1);
	victor = VectorPopBack(victor);
	victor = VectorPopBack(victor);
    
     if (0 == VectorSize(victor))
     {
         VectorDestroy(victor);
         return 1;
     }
     else
     {
         VectorDestroy(victor);
         return 0;
     } 
}



static int VectorLoopTest(void)
{
	size_t max_capacity = 100;
	size_t index = 0;
	int popped_all_the_way = 0;
	int resize_check = 0;
	vector_t *victor = VectorCreate(21, sizeof(size_t));
	
	for (index = 0; index < max_capacity; ++index)
	{
		victor = VectorPushBack(victor, (void *)&index);
		
		if (!(index == *(size_t *)VectorGetAccessElement(victor, index) &&
		    (index + 1) == VectorSize(victor)))
		{
			VectorDestroy(victor);
			return 0;
		}
	}
	
	victor = VectorPushBack(victor, &index);
	
	for (; index > 0; --index)
	{
		victor = VectorPopBack(victor);
		
		if (!(index == *(size_t *)VectorGetAccessElement(victor, index) ||
			(index - 1) != VectorSize(victor)))
		{
			VectorDestroy(victor);
			return 0;
		}
	}
	
	victor = VectorPopBack(victor);
	
	if (0 == VectorSize(victor))
	{
		popped_all_the_way = 1;
	}
	else
	{
		VectorDestroy(victor);
		return 0;
	}
	
	victor = VectorResize(victor, 50);
	if (50 == VectorCapacity(victor))
	{
		resize_check = 1;
	}
	else
	{
		VectorDestroy(victor);
		return 0;
	}
	
	VectorDestroy(victor);
	
	return (popped_all_the_way && resize_check);
	
}




















