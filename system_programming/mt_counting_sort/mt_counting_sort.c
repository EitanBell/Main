/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Yuval                   	   *
*	Date: 09/03/23	             	       *
*	Subject: Multi Threaded Counting Sort  *
*				                		   *
*******************************************/

#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */
#include <stdlib.h> /* calloc, free */	
#include <pthread.h> /* pthread */

#include "mt_counting_sort.h" /* mt_counting_sort API */

#define ASCII_SIZE (256)

enum status	
{
	SUCCESS = 0,
	FAIL
};

typedef struct thread_container
{
	unsigned char *container;
	size_t container_size;
	unsigned char *shared_hist;
}thread_container_t;

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static void SetHistArr(unsigned char *shared_hist, unsigned char *arr, 
																   size_t size);
static void SetSortedArr(unsigned char *sorted_arr, unsigned char *shared_hist, 
											   unsigned char *arr, size_t size);
static void CopyArrays(unsigned char *from_arr, unsigned char *to_arr, 
																   size_t size);
static int InitThreads(thread_container_t *containers, pthread_t *threads, 
		 unsigned char *arr, unsigned char *shared_hist, size_t container_size);
static void *ThreadHistsUpdate(void *args);
static void FreeResources(thread_container_t *containers, 
	      unsigned char *histogram, pthread_t *threads, pthread_mutex_t *mutex);				   											

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;														   	    
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTION                               */
/*----------------------------------------------------------------------------*/

/*------------------------------ MTCountingSort ------------------------------*/

int MTCountingSort(unsigned char *arr, size_t size, size_t threads_num)
{
	unsigned char shared_hist[ASCII_SIZE + 1] = {0};
	unsigned char *sorted_arr = NULL;
	thread_container_t *containers = {0};
	pthread_t *threads = NULL;
	size_t container_size = 0;
	size_t i = 0;	
	int thread_status = SUCCESS;
	
	assert(NULL != arr);
	assert(0 < size);
	assert(0 < threads_num);
	
	if (0 != pthread_mutex_init(&g_mutex, NULL))
	{
		return FAIL;
	}
	
	sorted_arr = (unsigned char *)calloc(size, sizeof(unsigned char));
	if (NULL == sorted_arr)
	{
		FreeResources(NULL, NULL, NULL, &g_mutex);
		
		return FAIL;
	}
	
	container_size = size / threads_num;
	
	containers = (thread_container_t *)malloc(sizeof(thread_container_t) * 
																   threads_num);
	if (NULL == containers)
	{
		FreeResources(NULL, sorted_arr, NULL, &g_mutex);
		
		return FAIL;
	}
	
	threads = (pthread_t *)malloc(sizeof(pthread_t) * threads_num);
	if (NULL == threads)
	{
		FreeResources(containers, sorted_arr, NULL, &g_mutex);
		
		return FAIL;
	}
	
	for (i = 0; ((threads_num - 1) > i) && (SUCCESS == thread_status); ++i)
	{	
		thread_status = InitThreads(&containers[i], &threads[i], 
				     (arr + (i * container_size)), shared_hist, container_size);
	}
	
	thread_status = InitThreads(&containers[i], &threads[i], 
								  	  (arr + (i * container_size)), shared_hist, 
										   container_size + size % threads_num);
	
	if (FAIL == thread_status)
	{
		FreeResources(containers, sorted_arr, NULL, &g_mutex);
	}
	
	for (i = 0; threads_num > i; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	
	SetHistArr(shared_hist, arr, size);
	
	SetSortedArr(sorted_arr, shared_hist, arr, size);
	
	CopyArrays(sorted_arr, arr, size);
	
	FreeResources(containers, sorted_arr, threads, &g_mutex);
	
	return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*-------------------------------- SetHistArr --------------------------------*/

static void SetHistArr(unsigned char *shared_hist, unsigned char *arr, 
																	size_t size)
{
	size_t i = 0;
	
	assert(NULL != shared_hist);
	assert(NULL != arr);
	assert(0 < size);
	
	for (i = 1; i <= ASCII_SIZE; ++i)
	{
		shared_hist[i] += shared_hist[i - 1];
	}
}

/*------------------------------- SetSortedArr -------------------------------*/

static void SetSortedArr(unsigned char *sorted_arr, unsigned char *shared_hist, 
												unsigned char *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != sorted_arr);
	assert(NULL != shared_hist);
	assert(NULL != arr);
	assert(0 < size);
	
	for (i = size - 1; i > 0; --i)
	{
		sorted_arr[shared_hist[arr[i]] - 1] = arr[i];
		--shared_hist[arr[i]];
	}
	sorted_arr[shared_hist[arr[0]] - 1] = arr[0];
}

/*-------------------------------- CopyArrays --------------------------------*/

static void CopyArrays(unsigned char *from_arr, unsigned char *to_arr, 
																	size_t size)
{
	size_t i = 0;
	
	assert(NULL != from_arr);
	assert(NULL != to_arr);
	assert(0 < size);
	
	for (i = 0; i < size; ++i)
	{
		to_arr[i] = from_arr[i];
	}
}

/*-------------------------------- InitThreads -------------------------------*/

static int InitThreads(thread_container_t *containers, pthread_t *threads, 
		  unsigned char *arr, unsigned char *shared_hist, size_t container_size)
{
	assert(NULL != containers);
	assert(NULL != threads);
	assert(NULL != arr);
	assert(NULL != shared_hist);
	
	containers->container = arr;
	containers->container_size = container_size;  
	containers->shared_hist = shared_hist;
		  		
	if (0 != pthread_create(threads, NULL, &ThreadHistsUpdate, containers))
	{
		return FAIL;
	}
	
	return SUCCESS;
}

/*---------------------------- ThreadHistsUpdate -----------------------------*/

static void *ThreadHistsUpdate(void *args)
{
	size_t i = 0;
	unsigned char thread_hist[ASCII_SIZE] = {0};
	thread_container_t *thread_cont = NULL;
	
	assert(NULL != args);
	
	thread_cont = (thread_container_t *)args; 
	
	for (i = 0; thread_cont->container_size > i; ++i)
	{
		++(thread_hist[thread_cont->container[i]]);
	}
	
	pthread_mutex_lock(&g_mutex);	
	
	for (i = 0; ASCII_SIZE > i; ++i)
	{
		thread_cont->shared_hist[i] += thread_hist[i];
	}
	
	pthread_mutex_unlock(&g_mutex);
	
	pthread_exit(NULL);
}

/*------------------------------- FreeResources ------------------------------*/

static void FreeResources(thread_container_t *containers, 
	       unsigned char *histogram, pthread_t *threads, pthread_mutex_t *mutex)
{
	free(containers);
	containers = NULL; 
	
	free(histogram);
	histogram = NULL;
	
	free(threads);
	threads = NULL;
	
	pthread_mutex_destroy(mutex);
}

