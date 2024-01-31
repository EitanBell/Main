/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Subject: Quiz Ping Pong			  *
*				                	  *
**************************************/
#define _GNU_SOURCE

#include <stdio.h>		/* printf */
#include <pthread.h>	/* pthread_create */
#include <semaphore.h>

static sem_t g_sem1 = {0};
static sem_t g_sem2 = {0};

static void *DoPing(void *args);
static void *DoPong(void *args);

/*----------------------------------- main -----------------------------------*/

int main(void)
{
	pthread_t ping_thread = {0};
	pthread_t pong_threads = {0};
	int status = 0;
		
	status = sem_init(&g_sem1, 0, 1);
	status += sem_init(&g_sem2, 0, 0);
	if (0 != status)
	{
		printf("Error occured.\n");
		return -1;
	}
	
	status = pthread_create(&ping_thread, NULL, &DoPing, NULL);
	status += pthread_create(&pong_threads, NULL, &DoPong, NULL);
	if (0 != status)
	{
		printf("Error occured.\n");
		return -1;
	}
	
	status = pthread_join(ping_thread, NULL);
	status += pthread_join(pong_threads, NULL);
	if (0 != status)
	{
		printf("Error occured.\n");
		return -1;
	}
	
	sem_destroy(&g_sem1);
	sem_destroy(&g_sem2);
	
	return 0;
}

/*---------------------------------- DoPing ----------------------------------*/

static void *DoPing(void *args)
{
	while (1)
	{
		sem_wait(&g_sem1);
		printf("PING\n");
		sem_post(&g_sem2);
	}
	
	(void)args;
	pthread_exit(NULL);
}

/*---------------------------------- DoPong ----------------------------------*/

static void *DoPong(void *args)
{
	while (1)
	{
		sem_wait(&g_sem2);
		printf("PONG\n");
		sem_post(&g_sem1);
	}
	
	(void)args;
	pthread_exit(NULL);
}



