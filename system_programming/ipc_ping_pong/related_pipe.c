/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 03/05/23	                  *
*	Subject: IPC Ping Pong			  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <unistd.h>		/* sleep, pipes */
#include <semaphore.h>  /* semaphore */
#include <fcntl.h>		/* consts */
#include <sys/stat.h>	/* const modes*/
#include <sys/types.h>	/* const modes*/
#include <stdlib.h>     /* exit */

static int RelatedPipesPingPong(void);
static void ChildWrite(void);
static void ChildRead(void);
static void ParentWrite(void);
static void ParentRead(void);

#define CHILD_PROC (0)

enum status 
{
	FAIL = -1,
	SUCCESS
};

int pipe2parent[2] = {0};
int pipe2child[2] = {0};

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	RelatedPipesPingPong();

	return 0;
}

/*----------------------------------------------------------------------------*/
/*                              HELPER FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static int RelatedPipesPingPong(void)
{
	int pid = 0;
	size_t i = 0;
	
	if((FAIL == pipe(pipe2parent)) || (FAIL == pipe(pipe2child)))
    {
        perror("Failed to open Pipes.\n");
        return FAIL;
    }
    
    pid = fork();
    
    if (0 > pid)	
    {
        perror("Fork failed\n");
        return FAIL;
    }
    else if (CHILD_PROC == pid)
    {
    	for (i = 0; 5 > i; ++i)
        {
        	close(pipe2child[1]);
            close(pipe2parent[0]);
            ChildRead();
            while(sleep(1));
            ChildWrite();
        }
    }
    else
    {
    	for (i = 0; 5 > i; ++i)
        {
        	close(pipe2child[0]);
            close(pipe2parent[1]);
            ParentWrite();
            ParentRead();
            while(sleep(1));
        }
    }
    
    return SUCCESS;
}

static void ChildWrite(void)
{
    if (0 >= write(pipe2parent[1],"Pong", 5))
    {
        perror("Child write failed\n");
        exit(FAIL);
    }
}

static void ChildRead(void)
{
    char buf[5];
    
    if (0 >= read(pipe2child[0], buf, 5))
    {
        perror("Child read failed\n");
        exit(FAIL);
    }

    printf("%s\n", buf);
}

static void ParentWrite(void)
{
    if (0 >= write(pipe2child[1],"Ping", 5))
    {
        perror("Parent write failed\n");
        exit(FAIL);
    }
}

static void ParentRead(void)
{
    char buf[5];
    
    if (0 >= read(pipe2parent[0], buf, 5))
    {
        perror("Parent read failed\n");
        exit(FAIL);
    }

    printf("%s\n", buf);
}

