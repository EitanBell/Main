/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Tal	                  *
*	Date: 03/03/23	                  *
*	Subject: Producer-Consumer		  *
*				                	  *
**************************************/
#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <stddef.h> 	/* size_t */
#include <pthread.h> 	/*pthread funcs*/
#include <semaphore.h>	/* semaphore */

#include "dlist.h"		/* dlist API */
#include "c_buff.h"		/* c_buff API */

#define QUEUE_CAP	(50)
#define THREAD_SIZE (3000)
#define CHECK_SIZE (1000)


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

typedef enum lock_state
{
	PRODUCE = 0,
	CONSUME
}lock_t;

typedef struct read_write_fsq
{
	size_t read;
	size_t write;
	size_t size;
	int data[QUEUE_CAP];
}read_write_fsq_t;

enum var_condition
{
	NOT_READ = 0,
	READ
};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static void Exer1(void);
static void *ThrdProduceEx1(void *arguments);
static void *ThrdConsumerEx1(void *arguments);

static void Exer2(void);
static void *ThrdProduceEx2(void *arguments);
static void InitArr(int *arr, size_t size);
static void *ThrdConsumerEx2(void *arguments);

static void Exer3(void);
static void *ThrdProduceEx3(void *arguments);
static void *ThrdConsumerEx3(void *arguments);

static void Exer4(void);
static int InitGlobVarExer4(void);
static void *ThrdProduceEx4(void *arguments);
static void *ThrdConsumerEx4(void *arguments);

static void Exer5(void);
static void FSQEnqueue(read_write_fsq_t *fsq, int value);
static int FSQDequeue(read_write_fsq_t *fsq);
static int InitGlobVarExer5(void);
static int CountConsumeResults(int *arr, size_t size);
static void *ThrdProduceEx5(void *arguments);
static void *ThrdConsumerEx5(void *arguments);

static void Exer6(void);
static int InitGlobVarExer6(void);
static void *ThrdProduceEx6(void *arguments);
static void *ThrdConsumerEx6(void *arguments);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 		
/*---------------------------------- main ------------------------------------*/
		 		
int main(void)
{
	printf("\nExercise 1:\n");
	Exer1();
	
	printf("Exercise 2:\n");
	Exer2();
	
	printf("Exercise 3:\n");
	Exer3();
	
	printf("Exercise 4:\n");
	Exer4();
	
	printf("Exercise 5:\n");
	Exer5();
	
	printf("Exercise 6:\n");
	Exer6();
	
	return 0;
}
		 			 
/*---------------------------------- Exer1 -----------------------------------*/

lock_t glob_lock = PRODUCE;
int glob_value = 0;
int glob_arr[5] = {0};

static void Exer1(void)
{
	pthread_t producer_thrd = 0;
	pthread_t consumer_thrd = 0;
	int i = 0;
	
	while (0 != pthread_create(&producer_thrd, NULL, &ThrdProduceEx1, NULL)){}
	while (0 != pthread_create(&consumer_thrd, NULL, &ThrdConsumerEx1, NULL)){}
	
	pthread_join(producer_thrd, NULL);
	pthread_join(consumer_thrd, NULL);
	
	for (i = 0; i < 5; ++i)
	{
		printf("\tShared array[%d] is: %d\n", i, glob_arr[i]);
	}
	
	(5 == i) ? printf("\tSUCCESS\n\n") : printf("\tFAIL\n\n");
}

/*---------------------------------- Exer2 -----------------------------------*/

dlist_t *glob_dlist_pro = NULL;
dlist_t *glob_dlist_con = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void Exer2(void)
{
	pthread_t producer_thrd[THREAD_SIZE] = {0};
	pthread_t consumer_thrd[THREAD_SIZE] = {0};
	size_t i = 0;
	int arr2[THREAD_SIZE] = {0};
	int status = SUCCESS;
	
	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("\tFalied initialize mutex.\n");
		return;
	}
	
	glob_dlist_pro = DlistCreate();
	if (NULL == glob_dlist_pro)
	{
		printf("\tFalied create dlist.\n");
		pthread_mutex_destroy(&mutex);
		return;
	}
	
	glob_dlist_con = DlistCreate();
	if (NULL == glob_dlist_con)
	{
		printf("\tFalied create dlist.\n");
		pthread_mutex_destroy(&mutex);
		DlistDestroy(glob_dlist_pro);
		return;
	}
	
	InitArr(arr2, THREAD_SIZE);
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
   		while (SUCCESS != pthread_create(&producer_thrd[i], NULL, 
										   		   &ThrdProduceEx2, &arr2[i])){}
		while (SUCCESS != pthread_create(&consumer_thrd[i], NULL, 
			   								 	  &ThrdConsumerEx2, &arr2[i])){}								      
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(producer_thrd[i], NULL);
    	pthread_join(consumer_thrd[i], NULL);
    }
    
   	printf("\t%d produces were produced\n", THREAD_SIZE);
   	printf("\t%lu produces were consumed\n\n", DlistCount(glob_dlist_con));

    DlistDestroy(glob_dlist_pro);
   	glob_dlist_pro = NULL;
   	
   	DlistDestroy(glob_dlist_con);
   	glob_dlist_con = NULL;
   	
   	pthread_mutex_destroy(&mutex);
}

/*---------------------------------- Exer3 -----------------------------------*/

sem_t semaphore = {0};

static void Exer3(void)
{
	pthread_t producer_thrd[THREAD_SIZE] = {0};
	pthread_t consumer_thrd[THREAD_SIZE] = {0};
	size_t i = 0;
	int status = SUCCESS;
	int arr[THREAD_SIZE] = {0};

	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("\tFalied initialize mutex.\n");
		return;
	}
	
	status = sem_init(&semaphore, 0, 0);
	if (SUCCESS != status)
	{
		printf("\tFalied initialize semaphore.\n");
		pthread_mutex_destroy(&mutex);
		return;
	}
	
	glob_dlist_pro = DlistCreate();
	if (NULL == glob_dlist_pro)
	{
		printf("\tFailed to create dlist\n");
		pthread_mutex_destroy(&mutex);
		sem_destroy(&semaphore);
		return;
	}
	
	glob_dlist_con = DlistCreate();
	if (NULL == glob_dlist_con)
	{
		printf("\tFailed to create dlist\n");
		pthread_mutex_destroy(&mutex);
		sem_destroy(&semaphore);
		DlistDestroy(glob_dlist_pro);
   		glob_dlist_pro = NULL;
		return;
	}
	
	InitArr(arr, THREAD_SIZE);
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
   		while (SUCCESS != pthread_create(&producer_thrd[i], NULL, 
										    	    &ThrdProduceEx3, &arr[i])){}
		while (SUCCESS != pthread_create(&consumer_thrd[i], NULL, 
			   										  &ThrdConsumerEx3, NULL)){}								      
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(producer_thrd[i], NULL);
    	pthread_join(consumer_thrd[i], NULL);
    }
    
   	printf("\t%d produces were produced\n", THREAD_SIZE);
   	printf("\t%lu produces were consumed\n\n", DlistCount(glob_dlist_con));
	
 	DlistDestroy(glob_dlist_pro);
   	glob_dlist_pro = NULL;
   	
   	DlistDestroy(glob_dlist_con);
   	glob_dlist_con = NULL;
   	
   	pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);	
}

/*---------------------------------- Exer4 -----------------------------------*/

sem_t sem_write = {0};
sem_t sem_read = {0};
c_buff_t *glob_queue = NULL;

static void Exer4(void)
{
	pthread_t producer_thrd[THREAD_SIZE] = {0};
	pthread_t consumer_thrd[THREAD_SIZE] = {0};
	size_t i = 0;
	int status = SUCCESS;
	int arr[THREAD_SIZE] = {0};
	int res_arr[THREAD_SIZE] = {0};
	int res_exer4 = 0;

	status = InitGlobVarExer4();
	if (SUCCESS != status)
	{
		return;
	}
	
	InitArr(arr, THREAD_SIZE);
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
   		while (SUCCESS != pthread_create(&producer_thrd[i], NULL, 
										    	    &ThrdProduceEx4, &arr[i])){}
		while (SUCCESS != pthread_create(&consumer_thrd[i], NULL, 
			   								   &ThrdConsumerEx4, &res_arr[i])){}								      
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(producer_thrd[i], NULL);
    	pthread_join(consumer_thrd[i], NULL);
    }
    
    res_exer4 = CountConsumeResults(res_arr, THREAD_SIZE);
   	printf("\t%d produces were produced\n", THREAD_SIZE);
   	printf("\t%d produces were consumed\n\n", res_exer4);
	
 	pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_write);
	sem_destroy(&sem_read);
	CbuffDestroy(glob_queue);
}

/*---------------------------------- Exer5 -----------------------------------*/

static void FSQEnqueue(read_write_fsq_t *fsq, int value)
{
	assert(NULL != fsq);
	
	fsq->data[fsq->write] = value;
	fsq->write = (fsq->write + 1) % fsq->size;
}

static int FSQDequeue(read_write_fsq_t *fsq)
{
	int data = 0;
	
	assert(NULL != fsq);
	
	data = fsq->data[fsq->read];
	fsq->read = (fsq->read + 1) % fsq->size;
	
	return data;
}

#define THREAD_SIZE	(3000)

pthread_mutex_t mutex_produce = {0};
pthread_mutex_t mutex_consume = {0};
int glob_count_con_exer5 = 0;
read_write_fsq_t glob_fsq = {0, 0, 5, {0}};

static void Exer5(void)
{
	pthread_t producer_thrd[THREAD_SIZE] = {0};
	pthread_t consumer_thrd[THREAD_SIZE] = {0};
	size_t i = 0;
	int status = SUCCESS;
	int arr[THREAD_SIZE] = {0};
	int res_arr[THREAD_SIZE] = {0};
	int result = 0;
	
	status = InitGlobVarExer5();
	if (SUCCESS != status)
	{
		return;
	}
	
	InitArr(arr, THREAD_SIZE);
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
   		while (SUCCESS != pthread_create(&producer_thrd[i], NULL, 
										    	    &ThrdProduceEx5, &arr[i])){}
		while (SUCCESS != pthread_create(&consumer_thrd[i], NULL, 
			   								   &ThrdConsumerEx5, &res_arr[i])){}								      
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(producer_thrd[i], NULL);
    	pthread_join(consumer_thrd[i], NULL);
    }
    
	result = CountConsumeResults(res_arr, THREAD_SIZE);
   	printf("\t%d produces were produced\n", THREAD_SIZE);
   	printf("\t%d produces were consumed\n\n", result);
	
	pthread_mutex_destroy(&mutex_produce);
   	pthread_mutex_destroy(&mutex_consume);
    sem_destroy(&sem_write);
	sem_destroy(&sem_read);
}

/*---------------------------------- Exer6 -----------------------------------*/

pthread_cond_t var_cond = {0};
size_t glob_read_cond = NOT_READ;

static void Exer6(void)
{
	pthread_t producer_thrd = {0};
	pthread_t consumer_thrd[THREAD_SIZE] = {0};
	size_t i = 0;
	int status = SUCCESS;
	int arr[THREAD_SIZE] = {0};

	status = InitGlobVarExer6();
	if (SUCCESS != status)
	{
		return;
	}
	
	InitArr(arr, THREAD_SIZE);
	
	while (SUCCESS != pthread_create(&producer_thrd, NULL, 
													   &ThrdProduceEx6, NULL)){}

	for (i = 0; i < THREAD_SIZE; ++i)
    {
		while (SUCCESS != pthread_create(&consumer_thrd[i], NULL, 
			   										  &ThrdConsumerEx6, NULL)){}								      
	}
	
	for (i = 0; i < THREAD_SIZE; ++i)
    {
    	pthread_join(consumer_thrd[i], NULL);
    }
    
	pthread_join(producer_thrd, NULL);
	
 	pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
	pthread_cond_destroy(&var_cond);
	
	printf("\tAll %d consumers has been read the message.\n\n", THREAD_SIZE);
}


/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*--------------------------------- InitArr ----------------------------------*/

static void InitArr(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = i;
	}
}

/*------------------------------ ThrdProduceEx1 ------------------------------*/

static void *ThrdProduceEx1(void *arguments)
{
	size_t i = 0;
	(void)arguments;
	
	for (i = 0; i < 5; ++i)
	{
		while (PRODUCE != glob_lock){}	
		
		++glob_value;		
		
		glob_lock = CONSUME;
	}
	
	return NULL;
}

/*----------------------------- ThrdConsumerEx1 ------------------------------*/

static void *ThrdConsumerEx1(void *arguments)
{
	size_t i = 0;
	(void)arguments;
	
	for (i = 0; i < 5; ++i)
	{
		while (CONSUME != glob_lock){}	
	
		glob_arr[i] = glob_value;		
		
		glob_lock = PRODUCE;
	}
	
	return NULL;
}

/*------------------------------ ThrdProduceEx2 ------------------------------*/

static void *ThrdProduceEx2(void *arguments)
{
	assert(NULL != arguments);
	
	pthread_mutex_lock(&mutex);
	
    	DlistPushFront(glob_dlist_pro, arguments);
    
    pthread_mutex_unlock(&mutex);
	
	return NULL;
}

/*----------------------------- ThrdConsumerEx2 ------------------------------*/

static void *ThrdConsumerEx2(void *arguments)
{
	int time_to_consume = FALSE;
	void *produce = NULL;
	(void)arguments;
	
    while (FALSE == time_to_consume)
	{	
		pthread_mutex_lock(&mutex);
		
		if (DlistIsEmpty(glob_dlist_pro))
		{
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			time_to_consume = TRUE;
		}
	}
    
    produce = DlistPopFront(glob_dlist_pro);
	pthread_mutex_unlock(&mutex);
	DlistPushFront(glob_dlist_con, produce);
	
    return NULL;
}

/*------------------------------ ThrdProduceEx3 ------------------------------*/

static void *ThrdProduceEx3(void *arguments)
{	
	assert(NULL != arguments);
	
	pthread_mutex_lock(&mutex);
	
    DlistPushFront(glob_dlist_pro, arguments);
    
    pthread_mutex_unlock(&mutex);
    
	sem_post(&semaphore);
    
    return NULL;
}

/*----------------------------- ThrdConsumerEx3 ------------------------------*/

static void *ThrdConsumerEx3(void *arguments)
{
	void *produce = NULL;
	(void)arguments;
	
	sem_wait(&semaphore);
	
    pthread_mutex_lock(&mutex);	
    	
   	produce = DlistPopFront(glob_dlist_pro);
    	
    pthread_mutex_unlock(&mutex);
   								
   	DlistPushFront(glob_dlist_con, produce);
   								
    return NULL;
}

/*------------------------------ ThrdProduceEx4 ------------------------------*/

static void *ThrdProduceEx4(void *arguments)
{	
	int produce = 0;
	int i = 0;
	
	assert(NULL != arguments);
	
	for (i = 0; CHECK_SIZE > i; ++i)
	{
		produce = *(int *)arguments;
		
		sem_wait(&sem_write);
		
		pthread_mutex_lock(&mutex);
		
			CbuffWrite(glob_queue, &produce, sizeof(int));
		
		pthread_mutex_unlock(&mutex);
		
		sem_post(&sem_read);
    }
    return NULL;
}

/*----------------------------- ThrdConsumerEx4 ------------------------------*/

static void *ThrdConsumerEx4(void *arguments)
{
	int consumer = -1;
	size_t i = 0;
	int *ptr = NULL;

	assert(NULL != arguments);
	
	for (i = 0; CHECK_SIZE > i; ++i)
	{
		ptr = (int *)arguments;
		
		sem_wait(&sem_read);

		pthread_mutex_lock(&mutex);	
		
			CbuffRead(glob_queue, &consumer, sizeof(int));
			
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_write);
		
		*ptr = consumer + 1;
    }
			
    return NULL;
}

/*----------------------------- InitGlobVarExer4 -----------------------------*/

static int InitGlobVarExer4(void)
{
	int status = SUCCESS;
	
	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("Failed initialize mutex\n");
		return FAIL;
	}
	
	status = sem_init(&sem_write, 0, (QUEUE_CAP / sizeof(int)));
	if (SUCCESS != status)
	{
		printf("Failed initialize write_semaphore\n");
		pthread_mutex_destroy(&mutex);
		return FAIL;
	}
	
	status = sem_init(&sem_read, 0, 0);
	if (SUCCESS != status)
	{
		printf("Failed initialize read_semaphore\n");
		pthread_mutex_destroy(&mutex);
		sem_destroy(&sem_write);
		return FAIL;
	}
	
	glob_queue = CbuffCreate(QUEUE_CAP);
	if (NULL == glob_queue)
	{
		printf("Failed create queue\n");
		pthread_mutex_destroy(&mutex);
		sem_destroy(&sem_write);
		sem_destroy(&sem_read);
		return FAIL;
	}
	
	return status;
}

/*----------------------------- InitGlobVarExer5 -----------------------------*/

static int InitGlobVarExer5(void)
{
	int status = SUCCESS;
	
	status = pthread_mutex_init(&mutex_produce, NULL);
	if (SUCCESS != status)
	{
		printf("Failed initialize mutex\n");
		return FAIL;
	}
	
	status = pthread_mutex_init(&mutex_consume, NULL);
	if (SUCCESS != status)
	{
		printf("Failed initialize mutex\n");
		pthread_mutex_destroy(&mutex_produce);
		return FAIL;
	}
	
	status = sem_init(&sem_write, 0, glob_fsq.size);
	if (SUCCESS != status)
	{
		printf("Failed initialize write_semaphore\n");
		pthread_mutex_destroy(&mutex_produce);
		pthread_mutex_destroy(&mutex_consume);
		return FAIL;
	}
	
	status = sem_init(&sem_read, 0, 0);
	if (SUCCESS != status)
	{
		printf("Failed initialize read_semaphore\n");
		pthread_mutex_destroy(&mutex_produce);
		pthread_mutex_destroy(&mutex_consume);
		sem_destroy(&sem_write);
		return FAIL;
	}
	
	return status;
}

/*------------------------------ ThrdProduceEx5 ------------------------------*/

static void *ThrdProduceEx5(void *arguments)
{	
	int produce = 0;
	
	assert(NULL != arguments);
	
	produce = *(int *)arguments;
	
	sem_wait(&sem_write);
	
	pthread_mutex_lock(&mutex_produce);
		
    	FSQEnqueue(&glob_fsq, produce);
    
    pthread_mutex_unlock(&mutex_produce);
    
	sem_post(&sem_read);
    
    return NULL;
}

/*----------------------------- ThrdConsumerEx5 ------------------------------*/

static void *ThrdConsumerEx5(void *arguments)
{
	int consumer = -1;
	int *ptr = NULL;
	
	assert(NULL != arguments);
	
	ptr = (int *)arguments;
	
    sem_wait(&sem_read);

    pthread_mutex_lock(&mutex_consume);	
    
    	consumer = FSQDequeue(&glob_fsq);
    	
    pthread_mutex_unlock(&mutex_consume);
    
    sem_post(&sem_write);
    
    *ptr = consumer + 1;
    							
    return NULL;
}

/*--------------------------- CountConsumeResults ----------------------------*/

static int CountConsumeResults(int *arr, size_t size)
{
	size_t i = 0;
	int result = 0;
	
	assert(NULL != arr);
	
	for (i = 0; (i < size) && (0 < arr[i]); ++i)
	{
			++result;
	}
	
	return result;
}

/*------------------------------ ThrdProduceEx6 ------------------------------*/

static void *ThrdProduceEx6(void *arguments)
{	
	(void)arguments;
	
	pthread_mutex_lock(&mutex);

		glob_read_cond = READ;
		pthread_cond_broadcast(&var_cond);

    pthread_mutex_unlock(&mutex);
    
    sem_wait(&semaphore);
    
	return NULL;
}

/*----------------------------- ThrdConsumerEx6 ------------------------------*/

static void *ThrdConsumerEx6(void *arguments)
{
	size_t i = 0;
	size_t consumed_counter = 0;
	
	(void)arguments;
	
	for (i = 0; THREAD_SIZE < i; ++i)
	{
		pthread_mutex_lock(&mutex);

		while (glob_read_cond <= consumed_counter)
		{
			pthread_cond_wait(&var_cond, &mutex);
		}

		pthread_mutex_unlock(&mutex);
		++consumed_counter;
		
		sem_post(&semaphore);
	}
	
	return NULL;
}

/*----------------------------- InitGlobVarExer6 -----------------------------*/

static int InitGlobVarExer6(void)
{
	int status = SUCCESS;
	
	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("\tFailed initialize mutex\n");
		return FAIL;
	}
	
	status = sem_init(&semaphore, 0, (QUEUE_CAP / sizeof(int)));
	if (SUCCESS != status)
	{
		printf("\tFailed initialize semaphore\n");
		pthread_mutex_destroy(&mutex);
		return FAIL;
	}
	
	status = pthread_cond_init(&var_cond, NULL);
	if (SUCCESS != status)
	{
		printf("\tFailed initialize variable condition\n");
		pthread_mutex_destroy(&mutex);
		sem_destroy(&semaphore);
		return FAIL;
	}
	
	return status;
}

