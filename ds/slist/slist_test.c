/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Moriya	            *
*	Date: 07/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Signle Linked List	  *
*				                 *
**************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "slist.h"

#define TEST_RES(status) (status ? "\xE2\x9C\x93 SUCCESS": "\xe2\x9c\x97 FAIL")

size_t SlistCountWithForEach(const slist_t *list);
void RunTests(void);
int SlistInsertTest(void);
int SlistRemoveTest(void);
int SlistSetGetTest(void);
int SlistCountTest(void);
int SlistIsEmptyTest(void);
int SlistIterIsEqualTest(void);
int SlistFindAndIsEqualTest(void);
static int IsIntMatchTest(const void *data, void *param);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{

	printf("\n******************LET THE TESTS BEGIN******************\n\n");
    printf("SlistInsert TEST:\t\t\t%s\n", 
            TEST_RES(SlistInsertTest()));
  
    printf("Slistremove TEST:\t\t\t%s\n", 
            TEST_RES(SlistRemoveTest()));
            
	printf("SlistSetGet TEST:\t\t\t%s\n", 
            TEST_RES(SlistSetGetTest()));
            
	printf("SlistCount TEST:\t\t\t%s\n", 
            TEST_RES(SlistCountTest()));
            
    printf("SlistFind TEST:\t\t\t\t%s\n", 
            TEST_RES(SlistFindAndIsEqualTest()));        
    
    printf("SlistIterIsEqual TEST:\t\t\t%s\n", 
            TEST_RES(SlistIterIsEqualTest()));        
                            
    printf("SlistIsEmpty TEST:\t\t\t%s\n",   
            TEST_RES(SlistIsEmptyTest()));         
}

int SlistInsertTest(void)
{
	slist_t *list = NULL;
    slist_iter_t iter = NULL;
    int num1 = 5;

    list = SlistCreate();
    iter = SlistBegin(list);
    
    iter = SlistInsertBefore(iter, &num1);
    
    if ((*(int *)SlistGetVal(iter)) == num1)
    {
    	SlistDestroy(list);
    	return 1;
    }
    else
    {
    	SlistDestroy(list);
    	return 0;
    }
}

int SlistRemoveTest(void)
{
	slist_t *list = NULL;
    slist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;

    list = SlistCreate();
    iter = SlistBegin(list);
    
    iter = SlistInsertBefore(iter, &num1);
    iter = SlistInsertBefore(iter, &num2);
    
    iter = SlistBegin(list);

    iter = SlistRemove(iter);
    
    if ((*(int *)SlistGetVal(iter)) == num1)
    {
    	SlistDestroy(list);
    	return 1;
    }
    else
    {
    	SlistDestroy(list);
    	return 0;
    }
}

int SlistSetGetTest(void)
{
	slist_t *list = NULL;
    slist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;

    list = SlistCreate();
    iter = SlistBegin(list);
    
    iter = SlistInsertBefore(iter, &num1);
    iter = SlistSetVal(iter, &num2);
    
    if ((*(int *)SlistGetVal(iter)) == num2)
    {
    	SlistDestroy(list);
    	return 1;
    }
    else
    {
    	SlistDestroy(list);
    	return 0;
    }
    
}

int SlistCountTest(void)
{
	slist_t *list = NULL;
    slist_iter_t iter = NULL;
    int num1 = 5;
    int num2 = 7;

    list = SlistCreate();
    iter = SlistBegin(list);
    
    iter = SlistInsertBefore(iter, &num1);
    iter = SlistInsertBefore(iter, &num2);
    
    iter = SlistBegin(list);
	
	if (SlistCount(list) == 2)
	{
    	SlistDestroy(list);
    	return 1;
    }
    else
    {
    	SlistDestroy(list);
    	return 0;
    }
}

int SlistIsEmptyTest(void)
{
	slist_t *list = NULL;
	
	list = SlistCreate();

	if (SlistIsEmpty(list))
	{
		SlistDestroy(list);
		return 1;
	}
	else
	{
		SlistDestroy(list);
		return 0;
	}
}

int SlistIterIsEqualTest(void)
{
	slist_t *list = NULL;
    slist_iter_t iter = NULL;

    list = SlistCreate();
    iter = SlistBegin(list);
    
    if (SlistIterIsEqual(iter, iter))
    {
    	SlistDestroy(list);
    	return 1;
    }
    else
    {
    	SlistDestroy(list);
    	return 0;
    }
}

int SlistFindAndIsEqualTest(void)
{
	int status = 0;
	
	slist_t *list = NULL;
	slist_iter_t iter = NULL;
	slist_iter_t iter_match = NULL;
	slist_iter_t iter_from = NULL;
	slist_iter_t iter_to = NULL;
	
	int data = 56;
	int *ptr = &data;
	float new_val = 100.65;
	float *ptr_new_val = &new_val;
	
	list = SlistCreate();
	iter = SlistEnd(list);
	
	iter = SlistInsertBefore(iter, ptr);
	iter = SlistInsertBefore(iter, ptr);
	iter = SlistInsertBefore(iter, ptr);
	iter = SlistInsertBefore(iter, ptr);
	iter = SlistInsertBefore(iter, ptr);
	iter_match = SlistInsertBefore(iter, ptr_new_val);
	
	iter_from = SlistBegin(list);
	iter_to = SlistEnd(list);
	
	if(iter_match == SlistFind(iter_from, iter_to, IsIntMatchTest, ptr_new_val))
	{
		status = 1;
	}
	
	SlistDestroy(list);
	
	return status;
}

static int IsIntMatchTest(const void *data, void *param)
{
	assert(data);
	assert(param);
	
	return (*(int *)data == *(int *)param);
}









