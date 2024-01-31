/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 27/02/23	                  *
*	Subject: Sem Manipulation		  *
*				                	  *
**************************************/
#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <stddef.h> 	/* size_t */
#include <pthread.h> 	/* pthread funcs */
#include <stdlib.h> 	/* malloc */

#define THREAD_SIZE (5)

enum status
{
	FAIL = -1,
	SUCCESS
};


/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static void ThreadsMapping(void);
static void *ThrdFunc(void *arguments);
static void PrintNoThreadsMapping(void);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 		
/*---------------------------------- main ------------------------------------*/
		 		
int main(void)
{
	
	printf("\nInside Threads Mapping:\n");
	ThreadsMapping();
	
	printf("\nOutside of Threads Mapping:\n");
	PrintNoThreadsMapping();
	return 0;
}
		 			 
/*------------------------------ ThreadsMapping ------------------------------*/

pthread_mutex_t mutex = {0};
int g_value = 222;

static void ThreadsMapping(void)
{
	pthread_t thread[THREAD_SIZE] = {0};
	size_t i = 0;
	int arr[THREAD_SIZE] = {0};
	int status = 0; 	
	
	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("\tFalied initialize mutex.\n");
		return;
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
	{
		arr[i] = i;
		while (0 != pthread_create(&thread[i], NULL, &ThrdFunc, &arr[i])){}
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(thread[i], NULL);
    }

	pthread_mutex_destroy(&mutex);
}

/*-------------------------------- ThrdFunc ----------------------------------*/

static void *ThrdFunc(void *arguments)
{
	int thread_num = 5;
	int *stack_address = &thread_num;
	void *thread_heap = malloc(1);
	
	pthread_mutex_lock(&mutex);
	
	printf("\n\tThread number:\t %d\n", *(int *)arguments + 1);
	printf("\tThe address of thread Stack:\t\t %p\n", (void *)arguments);
	printf("\tThe address of Thread Local Storage:\t %p\n", (void *)stack_address);
	printf("\tThe address of thread Heap:\t\t %p\n", thread_heap);
	
	pthread_mutex_unlock(&mutex);
	
	free(thread_heap);
	thread_heap = NULL;
	
	return NULL;
}

/*-------------------------- PrintNoThreadsMapping ---------------------------*/

static void PrintNoThreadsMapping(void)
{
	int num = 10;
	void *no_thread_heap = malloc(1);
	
	printf("\n\tThe address of Stack:\t\t\t %p\n", (void *)&num);
	printf("\tThe address of global varialbe:\t\t %p\n", (void *)&g_value);
	printf("\tThe address of Heap:\t\t\t %p\n", no_thread_heap);
}

/* 
	[start]				[end]
   0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]

   0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]

*/
/*
Inside Threads Mapping:

	Thread number:	 1
	The address of thread Stack:			 0x7ffcda6b6850
	The address of Thread Local Storage:	 0x7f0d9dbfee34
	The address of thread Heap:		 		 0x7f0d98000b70

	Thread number:	 2
	The address of thread Stack:			 0x7ffcda6b6854
	The address of Thread Local Storage:	 0x7f0d9d3fde34
	The address of thread Heap:				 0x7f0d90000b70

	Thread number:	 4
	The address of thread Stack:			 0x7ffcda6b685c
	The address of Thread Local Storage:	 0x7f0d97ffee34
	The address of thread Heap:				 0x7f0d90000b70

	Thread number:	 3
	The address of thread Stack:			 0x7ffcda6b6858
	The address of Thread Local Storage:	 0x7f0d9cbfce34
	The address of thread Heap:				 0x7f0d8c000b70

	Thread number:	 5
	The address of thread Stack:			 0x7ffcda6b6860
	The address of Thread Local Storage:	 0x7f0d977fde34
	The address of thread Heap:		 		 0x7f0d8c000b70

Outside of Threads Mapping:

	The address of Stack:				 0x7ffcda6b688c
	The address of global varialbe:		 0x55d9ad674010
	The address of Heap:				 0x55d9add65c50

*/




