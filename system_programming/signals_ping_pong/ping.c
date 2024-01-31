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
#include <signal.h>		/* kill */
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

static void PingHandler(int signal, siginfo_t *info, void *context);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	sigaction_t ping = {0};
	size_t i = 0;
	
	ping.sa_sigaction = &PingHandler;
	ping.sa_flags = SA_SIGINFO;
	
	if (FAIL == sigaction(SIGUSR1, &ping, NULL))
	{
		return 0;
	}
	
	printf("%d\n", getpid());
	
	for (i = 0; 5 > i; ++i)
	{
		pause();
	}
	
	return 0;
}
	
/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		

/*-------------------------------- SigHandler --------------------------------*/

static void PingHandler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	
	if (SIGUSR1 == signal)
	{
		printf("PING from parent\n");
		kill(info->si_pid, SIGUSR2);
	}
}


