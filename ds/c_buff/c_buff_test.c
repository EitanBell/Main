/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Tal       *
*	Date: 29/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Stack API	         *
*				     *
**************************************/
#include <stdio.h> /* prinft */	

#include "c_buff.h" /* my stack */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m": "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

enum status 
{
	SUCCESS = 0,
	FAIL
};

static void RunTests(void);
static int CbuffCreateTest(void);
static int CbuffCapacityTest(void);
static int CbuffGetFreeSpaceTest(void);
static int CbuffIsEmptyTest(void);
static int CbuffWriteTest(void);
static int CbuffReadTest(void);
static int CbuffReadWriteIntegrationTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
    printf("\n******************LET THE TESTS BEGIN******************\n\n");
    
    printf("CbuffCreate TEST:\t\t\t%s\n", TEST_RES(CbuffCreateTest()));

    printf("CbuffCapacity TEST:\t\t\t%s\n", TEST_RES(CbuffCapacityTest()));
            
    printf("CbuffGetFreeSpace TEST:\t\t\t%s\n",
								    TEST_RES(CbuffGetFreeSpaceTest()));
            
    printf("CbuffIsEmpty TEST:\t\t\t%s\n", TEST_RES(CbuffIsEmptyTest()));

    printf("CbuffWrite TEST:\t\t\t%s\n", TEST_RES(CbuffWriteTest()));
    
    printf("CbuffRead TEST:\t\t\t\t%s\n", TEST_RES(CbuffReadTest()));
    
    printf("CbuffReadWriteIntegration TEST:\t\t%s\n",
							 TEST_RES(CbuffReadWriteIntegrationTest()));
               
    printf("\n******************TESTS FINISHED!******************\n\n");
    
    
}

static int CbuffCreateTest(void)
{
    int capacity = 20;
    c_buff_t *buffer = NULL;
    int status = FAIL;
    
    buffer = CbuffCreate(capacity);    
    if (NULL != buffer)
    {
        status = SUCCESS;
    }
    else
    {
        status = FAIL;
    }
    
    CbuffDestroy(buffer);
    return status;
    
    
}
static int CbuffCapacityTest(void)
{
    int capacity = 20;
    c_buff_t *buffer = NULL;
    int status = FAIL;
    
    buffer = CbuffCreate(capacity);    
    if (NULL == buffer)
    {
        status = FAIL;
    }
    if (20 == CbuffCapacity(buffer)) 
    {
    	   status = SUCCESS; 	
    }
    
    CbuffDestroy(buffer);
    return status;
}

static int CbuffGetFreeSpaceTest(void)
{
    int capacity = 20;
    c_buff_t *buffer = NULL;
    int status = FAIL;
    
    buffer = CbuffCreate(capacity);    
    if (NULL == buffer)
    {
        status = FAIL;
    }
    
    if (20 == CbuffGetFreeSpace(buffer))
    {
     	status = SUCCESS;
    }
    
    CbuffDestroy(buffer);
    return status;
}
static int CbuffIsEmptyTest(void)
{
    int capacity = 20;
    c_buff_t *buffer = NULL;
    
    buffer = CbuffCreate(capacity);    
    if (NULL == buffer)
    {
        CbuffDestroy(buffer);
        return FAIL;
    }
    
    if (CbuffIsEmpty(buffer))
    {
    		CbuffDestroy(buffer);
     	return SUCCESS;
    }
    CbuffDestroy(buffer);
    return FAIL;
}

static int CbuffWriteTest(void)
{
	c_buff_t *cbuffer = NULL;
	int test[20] = {0};
	size_t i = 0;
	ssize_t written_bytes = 0;
	size_t nbytes = 60;
	int status1 = 0;
	int status2 = 0;
	
	for (i = 0; i < 20; ++i)
	{
		*(test + i) = i * i;
	}
	
	cbuffer = CbuffCreate(BUFSIZ);
	
	written_bytes = CbuffWrite(cbuffer, test, nbytes);
	
	if (nbytes == (size_t)written_bytes)
	{
		status1 = SUCCESS;
	}
	
	if (BUFSIZ - (size_t)written_bytes == CbuffGetFreeSpace(cbuffer))
	{
		status2 = SUCCESS;
	}
	
	CbuffDestroy(cbuffer);
	
	return status1 && status2;
}

static int CbuffReadTest(void)
{
	int status = 1;
	int val = 4;
	int capacity = 50;
	int capacity2 = 0;
	void *data = &val;
	c_buff_t *buffy = CbuffCreate(capacity);
	c_buff_t *buffy2 = CbuffCreate(capacity2);
	
	CbuffWrite(buffy, data, sizeof(val));
	
	if (4 == CbuffRead(buffy, data, sizeof(val)) &&
		0 == CbuffRead(buffy2, data, sizeof(val)))
	{
		status = 0;
	}

	CbuffDestroy(buffy);
	CbuffDestroy(buffy2);
	
	return status;
}


static int CbuffReadWriteIntegrationTest(void)
{
    c_buff_t *buffy = NULL;
    int status = 1;
    int capacity = 10;
    char data[] = "Gal";
    char data1[] = "OrNoy";
    char *dest_r1 = NULL;
    char *dest_r2 = NULL;
    char *dest_r3 = NULL;
    
    dest_r1 = (char *)calloc(sizeof(char), capacity);
    if (NULL == dest_r1)
    {
    	return -1;
    }
    
    dest_r2 = (char *)calloc(sizeof(char), capacity);
    if (NULL == dest_r2)
    {
    	return -1;
    }
    
    dest_r3 = (char *)calloc(sizeof(char), capacity);
    if (NULL == dest_r3)
    {
    	return -1;
    }
    buffy = CbuffCreate(capacity);
	
	CbuffWrite(buffy, data, sizeof(data) - 1);
	CbuffWrite(buffy, data1, sizeof(data1) - 1);
	
	CbuffRead(buffy, dest_r1, sizeof(data) - 1);
/*	printf("%s\n", dest_r1);*/
	CbuffWrite(buffy, data1, sizeof(data1) - 1);
	
	CbuffRead(buffy, dest_r2, 3);
/*	printf("%s\n", dest_r2);*/
	
	CbuffRead(buffy, dest_r3, 7);
/*	printf("%s\n", dest_r3);*/
	
	
    if (10 == CbuffGetFreeSpace(buffy) && !strcmp(dest_r1, "Gal") 
   	 	&& !strcmp(dest_r2, "OrN") && !strcmp(dest_r3, "oyOrNoy"))
    {
       	status = 0;
    }

    free(dest_r1);
    free(dest_r2);
    free(dest_r3);        
    CbuffDestroy(buffy);
    
    return status;
}
















