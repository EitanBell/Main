/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Sveta	                  *
*	Date: 20/02/23	                  *
*	Subject: 100k Threads 			  *
*				                	  *
**************************************/
#include <stdio.h> 	 /* printf */
#include <assert.h>  /* assert */
#include <unistd.h>  /* sleep */
#include <time.h> 	 /* time */
#include <pthread.h> /* pthread_create */
#include <omp.h>	 /* openMP */

#define MAX_THREAD_NUM (100000)
#define MAX_AVAIL_THREADS (32750)
#define LARGE_NUM (1255267140)
#define THREAD_SIZE (3000)

enum status
{
	FAIL = -1,
	SUCCESS
};

typedef struct range
{
	size_t start;
	size_t end;
	size_t sum;
}range_t;

size_t arr_glob_exe1[MAX_THREAD_NUM] = {0};
size_t arr_glob_exe2[MAX_AVAIL_THREADS] = {0};
size_t sum_of_divisors = 0;
pthread_mutex_t mutex = {0};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
void Exer1(void);
static void *SetArray(void *num);
static void InitThreadArr(pthread_t *thread_arr, size_t size);
static void InitIndexArr(size_t *arr, size_t size);

void Exer2(void);
static void *SetArrayExe2(void *num);

void Exer3(void);
static void *SetArrayExe3(void *num);

void Exer4(void);
static void *SumOfDivisors(void *num);

void Exer5(void);

void Exer6(void);
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------------ main ----------------------------------*/

int main(void)
{
	time_t start_time = 0;	
	
/*	start_time = time(NULL);*/
/*	Exer1();*/
/*	printf("Exer1 duration: %ld sec \n", (time(NULL) - start_time));*/
/*	*/
/*	start_time = time(NULL);*/
/*	Exer2();*/
/*	printf("Exer2 duration: %ld sec \n", (time(NULL) - start_time));*/
	
/*	start_time = time(NULL);*/
/*	Exer3();*/
/*	printf("Exer3 duration: %ld sec \n", (time(NULL) - start_time));*/
/*	*/
/*	start_time = time(NULL);*/
/*	Exer4();*/
/*	printf("Exer4 duration: %ld sec \n", (time(NULL) - start_time));*/
/*	*/
	start_time = time(NULL);
	Exer5();
	printf("Exer5 duration: %ld sec \n", (time(NULL) - start_time));
	
	/*Must compile with -fopenmp flag*/
	start_time = time(NULL);
	Exer6();
	printf("Exer5 duration: %ld sec \n", (time(NULL) - start_time));
	
	return 0;
}

/*------------------------------------ Exer1 ---------------------------------*/

void Exer1(void)
{
	size_t i = 0;
	int status = SUCCESS;
	size_t arr_inx[MAX_THREAD_NUM] = {0};
	pthread_t thread_arr[MAX_THREAD_NUM] = {0};
	
	InitThreadArr(thread_arr, MAX_THREAD_NUM);
	InitIndexArr(arr_inx, MAX_THREAD_NUM);
	
	for (i = 0; i < MAX_THREAD_NUM; ++i)
	{
		pthread_create(&thread_arr[i], NULL, &SetArray, (void *)&arr_inx[i]);
	}
	
	sleep(10);
	
	for (i = 0; i < MAX_AVAIL_THREADS && 0 == status; ++i) 
	{
		status = (arr_glob_exe1[i] != i); 
		printf("%ld, ",arr_glob_exe1[i]);
	}
	
	(SUCCESS == status) ? printf("Success.\n") : printf("Failed.\n");
}

/*------------------------------------ Exer2 ---------------------------------*/

void Exer2(void)
{
	size_t i = 0;
	int status = SUCCESS;
	size_t arr2[MAX_AVAIL_THREADS] = {0};
	pthread_t thread_arr2[MAX_AVAIL_THREADS] = {0};
	
	InitThreadArr(thread_arr2, MAX_AVAIL_THREADS);
	InitIndexArr(arr2, MAX_AVAIL_THREADS);
	
	for (i = 0; i < MAX_AVAIL_THREADS; ++i)
	{
		while (SUCCESS != pthread_create(&thread_arr2[i], NULL, &SetArrayExe2,
														  (void *)&arr2[i]));
	}
	
	for (i = 0; i < MAX_AVAIL_THREADS && 0 == status; ++i) 
	{
		status = (arr_glob_exe2[i] != i); 
		printf("%ld, ",arr_glob_exe2[i]);
	}
	
	(SUCCESS == status) ? printf("Success.\n") : printf("Failed.\n");
}

/*------------------------------------ Exer3 ---------------------------------*/

void Exer3(void)
{
	size_t i = 0;
	int status = SUCCESS;
	size_t arr2[MAX_AVAIL_THREADS] = {0};
	pthread_t thread_arr2[MAX_AVAIL_THREADS] = {0};
	pthread_attr_t attr = {0};
	
	InitThreadArr(thread_arr2, MAX_AVAIL_THREADS);
	InitIndexArr(arr2, MAX_AVAIL_THREADS);
	
	status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (0 != status)
	{
		printf("Error to set thread attribute: %d\n", status);
	}
	
	for (i = 0; i < MAX_AVAIL_THREADS; ++i)
	{
		while (SUCCESS != pthread_create(&thread_arr2[i], &attr, &SetArrayExe3,
														  (void *)&arr2[i]));
	}
	
	for (i = 0; i < MAX_AVAIL_THREADS && 0 == status; ++i) 
	{
		status = (arr_glob_exe2[i] != i); 
		printf("%ld, ",arr_glob_exe2[i]);
	}
	
	(SUCCESS == status) ? printf("Success.\n") : printf("Failed.\n");
}

/*------------------------------------ Exer4 ---------------------------------*/

void Exer4(void)
{
	size_t i = 0;
	int status = SUCCESS;
	range_t args_arr[THREAD_SIZE] = {0};
	pthread_t thread_arr4[THREAD_SIZE] = {0};
	
	status = pthread_mutex_init(&mutex, NULL);
	if (SUCCESS != status)
	{
		printf("Failed initialize mutex\n");
		return;
	}
	
	for (i = 0; THREAD_SIZE > i; ++i)
	{
		args_arr[i].start = i * (LARGE_NUM / THREAD_SIZE);
		args_arr[i].end = (LARGE_NUM / THREAD_SIZE);
		args_arr[i].sum = LARGE_NUM;
	}

	for (i = 0; THREAD_SIZE > i; ++i)
	{
		while (SUCCESS != pthread_create(&thread_arr4[i], NULL, &SumOfDivisors,
												  	    (void *)&args_arr[i])){}
	}
	
	for (i = 0; THREAD_SIZE > i; ++i)
	{
		pthread_join(thread_arr4[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);

	printf("The sum of divisors for 1,255,267,140 is: %lu\n", sum_of_divisors);
}

/*------------------------------------ Exer5 ---------------------------------*/

void Exer5(void)
{
	size_t i = 0;
	size_t sum = 0;

	for (i = 1; LARGE_NUM >= i; ++i)
	{
		if (0 == LARGE_NUM % i)
		{
			sum += i;
		}
	}
	
	printf("The sum of divisors for 1,255,267,140 is: %lu\n", sum);
}

/*------------------------------------ Exer6 ---------------------------------*/

void Exer6(void)
{
	size_t i = 0;
	size_t total_sum = 0;
	size_t mid_sum = 0;
	
	#pragma omp parallel private(mid_sum) shared(total_sum)
	{
		mid_sum = 0;
		
		#pragma omp for 
		for (i = 1; LARGE_NUM >= i; ++i)
		{
			if (0 == LARGE_NUM % i)
			{
				mid_sum += i;
			}
		}
		
		#pragma omp critical
		{
			total_sum += mid_sum;
		}
	}
	
	printf("The divisors sum with openMP for 1,255,267,140 is: %lu\n", total_sum);
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*--------------------------------- SetArray ---------------------------------*/

static void *SetArray(void *num)
{
	int index = 0;
	
	assert(NULL != num);
	
	index = *(int *)num;
	arr_glob_exe1[index] = index;
	
	pthread_exit(NULL);
	return NULL;
}

/*------------------------------ SetArrayExe2 --------------------------------*/

static void *SetArrayExe2(void *num)
{
	int index = 0;
	
	assert(NULL != num);
	
	index = *(int *)num;
	arr_glob_exe2[index] = index;
	
	pthread_exit(NULL);
	return NULL;
}

/*------------------------------ SetArrayExe3 --------------------------------*/

static void *SetArrayExe3(void *num)
{
	int index = 0;
	
	assert(NULL != num);
	
	index = *(int *)num;
	arr_glob_exe2[index] = index;
	
	pthread_detach(pthread_self()); 
	
	pthread_exit(NULL);
	return NULL;
}


/*------------------------------ InitThreadArr -------------------------------*/

static void InitThreadArr(pthread_t *thread_arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != thread_arr);
	
	for (i = 0; i < size; ++i)
	{
		thread_arr[i] = i;
	}
}

/*------------------------------- InitIndexArr -------------------------------*/

static void InitIndexArr(size_t *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = i;
	}
}

/*------------------------------ SumOfDivisors -------------------------------*/

static void *SumOfDivisors(void *num)
{
	size_t i = 0;
	range_t range = *(range_t *)num;

	for (i = 1; range.end >= i; ++i)
	{
		if (0 == range.sum % (range.start + i))
		{
			pthread_mutex_lock(&mutex);
			
			sum_of_divisors += (range.start + i);
				
			pthread_mutex_unlock(&mutex);
		}
	}
	
	return NULL;
}


	
