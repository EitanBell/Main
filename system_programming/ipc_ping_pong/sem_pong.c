/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 03/05/23	                  *
*	Subject: IPC Ping Pong			  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <unistd.h>		/* sleep */
#include <semaphore.h>  /* semaphore */
#include <fcntl.h>		/* consts */
#include <sys/stat.h>	/* const modes*/

static int PingPongSem(void);

enum status 
{
	FAIL = -1,
	SUCCESS
};

const char *g_sem_name_1 = "ping_sem";
const char *g_sem_name_2 = "pong_sem";

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	PingPongSem();

	return 0;
}

/*----------------------------------------------------------------------------*/
/*                              HELPER FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

static int PingPongSem(void)
{
	sem_t *ping_sem = NULL;
	sem_t *pong_sem = NULL;
	size_t i = 0;

	ping_sem = sem_open(g_sem_name_1, 0);
	if (SEM_FAILED == ping_sem)
    {
        perror("Failed to open the semaphore 1\n");
        return FAIL;
    }

	pong_sem = sem_open(g_sem_name_2, 0);
	if (SEM_FAILED == pong_sem)
    {
        perror("Failed to open the semaphore 2\n");
        return FAIL;
    }

    for (i = 0; 5 > i; ++i)
    {
        if (SUCCESS != sem_wait(ping_sem))
        {
            return FAIL;
        }

        while(sleep(1));
        printf("Pong\n");

        if (SUCCESS != sem_post(pong_sem))
        {
            return FAIL;
        }
    }

    if (FAIL == (sem_close(ping_sem)))
    {
        perror("Failed to close the semaphore");
        return FAIL;
    }

	if (FAIL == (sem_close(pong_sem)))
    {
        perror("Failed to close the semaphore");
        return FAIL;
    }

	return SUCCESS;
}