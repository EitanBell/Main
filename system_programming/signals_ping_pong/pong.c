/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 20/02/23	                  *
*	Subject: Signals Ping Pong		  *
*				                	  *
**************************************/
#define _POSIX_SOURCE
#define _GNU_SOURCE

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* atoi */
#include <signal.h>		/* sleep */
#include <unistd.h>		/* pid_t, kill */
#include <sys/wait.h>	/* wait */

enum status 
{
	FAIL = -1,
	SUCCESS
};

typedef struct sigaction sigaction_t;
/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/

static void PongHandler(int signal);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(int argc, const char **argv)
{
	sigaction_t pong = {0};
	pid_t pid = 0;
	size_t i = 0;
	
	pong.sa_handler = &PongHandler;
	pid = atoi(argv[(argc - 1)]);
	
	if (FAIL == sigaction(SIGUSR2, &pong, NULL))
	{
		return 0;
	}
	
	for (i = 0; 5 > i; ++i)
	{
		kill(pid, SIGUSR1);
		pause();
	}
	
	return 0;
}
	
/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		

/*-------------------------------- SigHandler --------------------------------*/

static void PongHandler(int signal)
{
	if (SIGUSR2 == signal)
	{
		printf("PONG from parent\n");
	}
}


