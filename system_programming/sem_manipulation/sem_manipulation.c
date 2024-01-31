/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 27/02/23	                  *
*	Subject: Sem Manipulation		  *
*				                	  *
**************************************/
#include <stdio.h>			/* fgets */
#include <stdlib.h>			/* atoi */
#include <assert.h>			/* assert */
#include <fcntl.h> 			/* O_CREAT */
#include <semaphore.h>		/* sem_open */
#include <errno.h>			/* SEM_FAILED */

#define INPUT_LEN (30)
#define TO_NUM_ARG (2)

enum status
{
	FAIL = -1,
	SUCCESS
};

enum bool_status
{
	FALSE = 0,
	TRUE
};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static int SemDecreament(sem_t *sem, char *buffer);
static int SemIncreament(sem_t *sem, char *buffer);
static void SemPrintVal(sem_t *sem);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*----------------------------- SemManipulations -----------------------------*/

int SemManipulations(const char *sem_name)
{
	sem_t *new_sem = NULL;
	int is_exit = FALSE;
	char buffer[INPUT_LEN] = {0};
	int status = 0;
	
	assert(NULL != sem_name);
	
	new_sem = sem_open(sem_name, O_CREAT, 0644, 0);
	if (SEM_FAILED == new_sem)
	{
		return FAIL;
	}
	
	printf("\nPlease enter as following:\n");
	printf("I - Increaments Semaphore's Value.\n");
	printf("D - Decreaments Semaphore's Value.\n");
	printf("V - View Semaphore's Value.\nX - For Exit.\n");
	
	while (!is_exit)
	{
		fgets(buffer, INPUT_LEN, stdin);
		
		switch (*buffer)
		{
			case 'D':
			{
				status = SemDecreament(new_sem, buffer);
				break;
			}
			case 'I':
			{
				status = SemIncreament(new_sem, buffer);
				break;
			}
			case 'V':
			{
				SemPrintVal(new_sem);
				break;
			}
			case 'X':
			{
				is_exit = TRUE;
				break;
			}
			default:
			{
				printf("Invalid Operation.\n");
				status = FAIL;
				break;
			}
		}
	}
	sem_close(new_sem);

	return status;
}
	
/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*------------------------------- SemDecrement -------------------------------*/
		
static int SemDecreament(sem_t *sem, char *buffer)
{
	int status = SUCCESS;
	int num = 0;
	int i = 0;

	assert(NULL != sem);
	assert(NULL != buffer);

	num = atoi(buffer + TO_NUM_ARG);
	
	for (i = 0; (num > i) && (SUCCESS == status); ++i)
	{
		status = sem_wait(sem);
	}
	
	return status;
}	 											
														
/*------------------------------- SemIncrement -------------------------------*/

static int SemIncreament(sem_t *sem, char *buffer)
{
	int status = SUCCESS;
	int num = 0;
	int i = 0;
	
	assert(NULL != sem);
	assert(NULL != buffer);

	num = atoi(buffer + TO_NUM_ARG);
	for (i = 0; (num > i) && (SUCCESS == status); ++i)
	{
		status = sem_post(sem);
	}
	
	return status;
}	 
/*-------------------------------- SemPrintVal -------------------------------*/

static void SemPrintVal(sem_t *sem)
{
	int sem_val = 0;
	
	assert(NULL != sem);
	
	sem_getvalue(sem, &sem_val);
	printf("The Semaphore Value Is: %d\n", sem_val);
}


