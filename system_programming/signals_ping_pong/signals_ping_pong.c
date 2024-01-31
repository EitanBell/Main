/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 20/02/23	                  *
*	Subject: Signals Ping Pong		  *
*				                	  *
**************************************/
#define _GNU_SOURCE

#include <stdio.h>		/* printf, scanf */
#include <stdlib.h>		/* fork, system */
#include <string.h>		/* strcmp, strcspn */
#include <unistd.h>		/* pid_t, execl */
#include <sys/wait.h>	/* waitpid */

enum status
{
	FAIL = -1,
	SUCCESS
};

typedef struct sigaction sigaction_t;
/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/

void SignalPingPongExe1(void);
static void SignalHandler(int sig_num);
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	SignalPingPongExe1();
	
	return 0;
}
	
/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*---------------------------- SignalPingPongExe1 ----------------------------*/

void SignalPingPongExe1(void)
{
	pid_t child_pid = 0;
	size_t i = 0;
	
	sigaction_t ping = {0};
	sigaction_t pong = {0};
	
	ping.sa_handler = &SignalHandler;
	pong.sa_handler = &SignalHandler;
	
	if (FAIL == sigaction(SIGUSR1, &ping, NULL) ||
		FAIL == sigaction(SIGUSR2, &pong, NULL))
	{
		printf("Sigaction Failed.\n");
		return;
	}
	
	child_pid = fork();
	
	if (0 < child_pid)
	{
		for (i = 0; i < 5; ++i)
		{
			kill(child_pid, SIGUSR1);
			pause();
		}
	}
	else if (0 == child_pid)
	{
		for (i = 0; i < 5; ++i)
		{
			pause();
		}
		
		exit(0);
	}
	else
	{
		printf("Fork Failed.\n");
	}
}

/*-------------------------------- SigHandler --------------------------------*/

static void SignalHandler(int sig_num)
{
	if (SIGUSR1 == sig_num)
	{
		printf("PING from parent\n");
		kill(getppid(), SIGUSR2);
	}
	else if (SIGUSR2 == sig_num)
	{
		printf("PONG from child\n");
	}
}


