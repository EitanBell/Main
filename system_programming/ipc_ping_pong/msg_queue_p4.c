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
	
	key = ftok("/tmp/msg_queue", 'Q');

	if (FAIL == (msg_id = msgget(key, 0666 | IPC_CREAT)))
    {
        perror("msgget failed");
        return 1;
    }
	
	if (FAIL == (msgrcv(msg_id, &msg_buf, sizeof(msg_buf), 1, 0)))
    {
        perror("msgrcv failed");
        return 1;
    }
    
	printf("Message in P4: %s\n", msg_buf.msg_text);
	
	return 0;
}

