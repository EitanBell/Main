/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 03/05/23	                  *
*	Subject: IPC Ping Pong			  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <string.h>     /* memcpy */
#include <sys/ipc.h>    /* key_t */
#include <sys/msg.h>    /* msgget, msgsnd */
#include <stdlib.h>     /* exit */

struct msg_buf 
{
   long msg_type;
   char msg_text[128];
};

enum status 
{
	FAIL = -1,
	SUCCESS
};

struct msg_buf msg_buf;

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	int msg_id = 0;
	key_t key;
	size_t i = 0;
	
	key = ftok("/tmp/msg_queue", 'Q');

	if (FAIL == (msg_id = msgget(key, 0666 | IPC_CREAT)))
    {
        perror("msgget failed");
        return 1;
    }
	
	msg_buf.msg_type = 1;

	if (NULL == (memcpy(msg_buf.msg_text, "Ping", 5)))
    {
        perror("memcpy failed");
        return 1;
    }

	for (i = 0; 5 > i; ++i)
    {
        if (FAIL == (msgsnd(msg_id, &msg_buf, sizeof(msg_buf), 0)))
        {
            perror("msgsnd failed");
            return 1;
        }
    }
    
    printf("message queue: 5 messages added to the queue.\n");

	return 0;
}

