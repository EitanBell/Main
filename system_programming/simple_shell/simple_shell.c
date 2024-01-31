/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 20/02/23	                  *
*	Subject: Simple Shell		 	  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf, scanf */
#include <assert.h>		/* assert */
#include <stdlib.h>		/* fork, system */
#include <string.h>		/* strcmp, strcspn */
#include <unistd.h>		/* pid_t, execl */
#include <sys/wait.h>	/* waitpid */

#include "simple_shell.h" /* Simple Shell API */

#define CMD_LEN (64)
#define ARGS_NUM (16)

enum status
{
	FAIL = -1,
	SUCCESS
};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/

static int ForkHandler(void);
static int SystemHandler(void);

static void SetCommandToToken(char *command_buf, char **args);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*-------------------------------- SimpleShell -------------------------------*/
int SimpleShell(void)
{
	char method = 0;
	int status = SUCCESS;

	printf("Enter a system call method.\n'S' for system(), 'F' for fork().\n");	
	scanf("%c", &method);
	getc(stdin);
	
	if ('F' == method)
	{
		status = ForkHandler();
	}
	else if ('S' == method)
	{
		status = SystemHandler();
	}
	else
	{
		printf("Invalid input.\n");
	}
	
	return status;
}
	
/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*----------------------------- ForkShellHandler -----------------------------*/

static int ForkHandler(void)
{
	pid_t child_pid = 0;
	int status = SUCCESS;
	char command_buf[CMD_LEN] = {0};
	char *args[ARGS_NUM] = {0};
	
	while (1)
	{
		printf("\033[1;31mNew-Shell-Prompt:~$ \033[0m");
		
		fgets(command_buf, CMD_LEN, stdin);
		command_buf[strcspn(command_buf, "\n")] = '\0';
		
		SetCommandToToken(command_buf, args);
		
		if (0 == strcmp(command_buf, "exit"))
		{
			return SUCCESS;
		}

		child_pid = fork();

		if (0 > child_pid)
		{
			printf("Fork failed.");
			return FAIL;
		}
		else if (0 == child_pid)
		{
			if ((0 > (status = execvp(args[0], args))))
			{
				printf("Command not found.\n");
				return FAIL;
			}
		}
		else
		{
			waitpid(child_pid, &status, 0);
		}
	} 
	
	return status;
}

/*------------------------------- SystemHandler ------------------------------*/

static int SystemHandler(void)
{
	int status = SUCCESS;
	char command_buf[CMD_LEN] = {0};
	
	while (1) 
	{
		printf("\033[1;31mNew-Shell-Prompt:~$ \033[0m");
		
		fgets(command_buf, CMD_LEN, stdin);
		
		if (0 == strcmp(command_buf, "exit\n"))
		{
			return SUCCESS;
		}
		
		if ((0 > (status = system(command_buf))))
		{
			printf("Command not found.\n");
			return FAIL;
		}
	} 
	
	return status;
}

/*---------------------------- SetCommandToToken -----------------------------*/

static void SetCommandToToken(char *command_buf, char **args)
{
	size_t i = 0;
	char *token = NULL;
	
	assert(NULL != command_buf);
	assert(NULL != args);
	
	token = strtok(command_buf, " ");
	
	while (NULL != token)
	{
		args[i] = token;
		token = strtok(NULL, " ");
		++i;
	}
	args[i] = NULL;
}

