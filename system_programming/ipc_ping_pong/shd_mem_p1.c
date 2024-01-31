/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 03/05/23	                  *
*	Subject: IPC Ping Pong			  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf, fgets */
#include <fcntl.h>      /* modes */
#include <sys/ipc.h>    /* key_t */
#include <sys/shm.h>    /* shm_id, shmget, shmat */

enum status 
{
	FAIL = -1,
	SUCCESS
};

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	int shm_id = 0;
	key_t key;
	char *buffer = NULL;
	size_t buf_size = 80;
	
	key = ftok("/tmp/shared_memory", 'G');

	if (FAIL == (shm_id = shmget(key, buf_size, 0666 | IPC_CREAT)))
    {
        perror("shmget failed");
        return 1;
    }
	
	if (FAIL == *(buffer = (char *)shmat(shm_id, (void *)0, 0)))
    {
        perror("shmat failed");
        return 1;
    }
    
    printf("Write your message to send: \n");
    fgets(buffer, buf_size, stdin);
    
    shmdt(buffer);

	return 0;
}

