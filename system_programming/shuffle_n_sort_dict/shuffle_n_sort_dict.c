/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Aviv                    	   *
*	Date: 12/03/23	             	       *
*	Subject: Shuffle and Sort dict files   *
*				                		   *
*******************************************/
#include <stdio.h>	 /* FILE */
#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* calloc, free */	
#include <pthread.h> /* pthread funcs*/
#include <string.h>	 /* strlen, strcpy, strchr */

#include "shuffle_n_sort_dict.h" /* shuffle_n_sort_dict.h API */

#define ASCII_SIZE (256)

enum status	
{
	SUCCESS = 0,
	FAIL
};

typedef struct thread_info
{
	unsigned char **arr;
	size_t size;
}thread_info_t;

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static void *ThreadSortFunc(void *data);
void FreeResources(void *threads, void *data);
static int CmpFunc(const void *data1, const void *data2);
static int MergeSort(unsigned char **arr_to_sort, size_t num_elements);
static void Merge(unsigned char **arr_to_sort, int left, int mid, int right, 
													  unsigned char **temp_arr);
static void InitTempLeftHalf(unsigned char **left_temp, unsigned char **arr, 
										   size_t left_temp_size, int left_arr);
static void InitTempRightHalf(unsigned char **right_temp, unsigned char **arr, 
										   size_t right_temp_size, int mid_arr);
static void PlaceCorrectInArr(unsigned char **arr_to_sort, 
	   unsigned char **left_temp, unsigned char **right_temp, 
	   size_t left_temp_index, size_t right_temp_index, size_t left_temp_size, 
								size_t right_temp_size, size_t arr_start_index);
static void RecMergeSort(unsigned char **arr_to_sort, int left, int right, 
													  unsigned char **temp_arr);
void SelectionSort(unsigned char **arr, size_t size);
static void Swap(unsigned char **elem1, unsigned char **elem2);
													  
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTION                               */
/*----------------------------------------------------------------------------*/

/*-------------------------- MTMergeSortDictionary ---------------------------*/

int MTMergeSortDictionary(unsigned char **dict, size_t size, size_t threads_num)
{
	thread_info_t *thrd_blocks = NULL;
	pthread_t *threads = NULL;
	size_t thrd_block_size = 0;
	size_t i = 0;
	size_t status = SUCCESS;
		
	assert(NULL != dict);
	assert(0 < size);
	assert(0 < threads_num);
	
	thrd_block_size = size / threads_num;
	
	thrd_blocks = (thread_info_t *)malloc(sizeof(thread_info_t) * threads_num);
	if (NULL == thrd_blocks)
	{
		return FAIL;
	}
	
	threads = (pthread_t *)malloc(sizeof(pthread_t) * threads_num);
	if (NULL == threads)
	{
		FreeResources(thrd_blocks, NULL);
		
		return FAIL;
	}
	
	for (i = 0; (threads_num - 1 > i) && (SUCCESS == status); ++i)
	{
		thrd_blocks[i].arr = dict + i * thrd_block_size;
		thrd_blocks[i].size = thrd_block_size;
		
		if (SUCCESS != pthread_create(&threads[i], NULL, &ThreadSortFunc, 
															   &thrd_blocks[i]))
	 	{
	 		FreeResources(thrd_blocks, threads);
	 		
	 		return FAIL;
	 	}
	}
	
	thrd_blocks[i].arr = dict + i * thrd_block_size;
	thrd_blocks[i].size = thrd_block_size + size % threads_num;
	
	if (SUCCESS != pthread_create(&threads[i], NULL, &ThreadSortFunc, 
															   &thrd_blocks[i]))
 	{
 		FreeResources(thrd_blocks, threads);
 		
 		return FAIL;
 	}
	
	for (i = 0; threads_num > i; ++i)
	{
		if (SUCCESS != pthread_join(threads[i], NULL))
		{
			FreeResources(thrd_blocks, threads);
			
			return FAIL;
		}
		
	}
	
	qsort(dict, size, sizeof(unsigned char *), &CmpFunc);
/*	MergeSort(dict, size);	*/
	
	FreeResources(thrd_blocks, threads);
	
	return status;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*------------------------------ ThreadSortFunc ------------------------------*/

static void *ThreadSortFunc(void *data)
{
	thread_info_t *thrd_info = NULL;
	
	assert(NULL != data);
	
	thrd_info = (thread_info_t *)data;
	
	qsort(thrd_info->arr, thrd_info->size, sizeof(unsigned char *), &CmpFunc);
/*	SelectionSort(thrd_info->arr, thrd_info->size);*/
	
	pthread_exit(NULL);
}
/*---------------------------------- CmpFunc ---------------------------------*/

static int CmpFunc(const void *data1, const void *data2)
{
	assert(NULL != data1);
	assert(NULL != data1);
	
	return (strcmp(*(char **)data1, *(char **)data2));
}

/*------------------------------- FreeResources ------------------------------*/

void FreeResources(void *threads, void *data)
{
	assert(NULL != threads);
	assert(NULL != data);
	
	free(threads);
	threads = NULL;
	
	free(data);
	data = NULL;
}

/*------------------------------- MergeSort ----------------------------------*/

static int MergeSort(unsigned char **arr_to_sort, size_t num_elements)
{
	unsigned char **temp_arr = NULL;
	
	assert(NULL != arr_to_sort);
	
	temp_arr = (unsigned char **)malloc(num_elements * sizeof(unsigned char *));
	if (NULL == temp_arr)
	{
		return FAIL;
	}
	
	RecMergeSort(arr_to_sort , 0, num_elements - 1, temp_arr);
	
	free(temp_arr);
	temp_arr = NULL;
	
	return SUCCESS;
}

/*--------------------------------- Merge ------------------------------------*/

static void Merge(unsigned char **arr_to_sort, int left, int mid, int right, 
													   unsigned char **temp_arr)
{
	unsigned char **left_temp = NULL;
	unsigned char **right_temp = NULL;
	
	size_t left_temp_size = 0;
	size_t right_temp_size = 0;
	
	size_t left_temp_index = 0;
	size_t right_temp_index = 0;
	size_t arr_start_index = 0;
	
	assert(NULL != arr_to_sort);
	assert(NULL != temp_arr);
	
	left_temp_size = mid - left + 1;
	right_temp_size = right - mid;
	
	left_temp = temp_arr;
	right_temp = left_temp + left_temp_size;
	
	InitTempLeftHalf(left_temp, arr_to_sort, left_temp_size, left);
	InitTempRightHalf(right_temp, arr_to_sort, right_temp_size, mid + 1);
	
	arr_start_index = left;
	
	PlaceCorrectInArr(arr_to_sort, left_temp, right_temp, left_temp_index, 
			right_temp_index, left_temp_size, right_temp_size, arr_start_index);
	
}

/*---------------------------- InitTempLeftHalf ------------------------------*/

static void InitTempLeftHalf(unsigned char **left_temp, unsigned char **arr, 
											size_t left_temp_size, int left_arr)
{
	size_t i = 0;
	
	assert(NULL != left_temp);
	assert(NULL != arr);
	
	for (i = 0; i < left_temp_size; ++i)
	{
		left_temp[i] = arr[left_arr + i];
	}
}

/*---------------------------- InitTempRightHalf -----------------------------*/

static void InitTempRightHalf(unsigned char **right_temp, unsigned char **arr, 
											size_t right_temp_size, int mid_arr)
{
	size_t i = 0;
	
	assert(NULL != right_temp);
	assert(NULL != arr);
	
	for (i = 0; i < right_temp_size; ++i)
	{
		right_temp[i] = arr[mid_arr + i];
	}
}

/*---------------------------- PlaceCorrectInArr -----------------------------*/

static void PlaceCorrectInArr(unsigned char **arr_to_sort, 
	   unsigned char **left_temp, unsigned char **right_temp, 
	   size_t left_temp_index, size_t right_temp_index, size_t left_temp_size, 
								 size_t right_temp_size, size_t arr_start_index)
{
	assert(NULL != arr_to_sort);
	assert(NULL != left_temp);
	assert(NULL != right_temp);

	while ((left_temp_index < left_temp_size) && 
		   (right_temp_index < right_temp_size))
	{
		if (0 >= strcmp((const char *)left_temp[left_temp_index],
								    (const char *)right_temp[right_temp_index]))
		{
			arr_to_sort[arr_start_index] = left_temp[left_temp_index];
			++left_temp_index;
		}
		else
		{
			arr_to_sort[arr_start_index] = right_temp[right_temp_index];
			++right_temp_index;
		}
		
		++arr_start_index;
	}
	
	while (left_temp_size > left_temp_index)
	{
		arr_to_sort[arr_start_index] = left_temp[left_temp_index];
		++left_temp_index;
		++arr_start_index;
	}
	
	while (right_temp_size > right_temp_index)
	{
		arr_to_sort[arr_start_index] = right_temp[right_temp_index];
		++right_temp_index;
		++arr_start_index;
	}
}

/*----------------------------- RecMergeSort ---------------------------------*/

static void RecMergeSort(unsigned char **arr_to_sort, int left, int right, 
													   unsigned char **temp_arr)
{
	int mid = 0;
	
	assert(NULL != arr_to_sort);
	assert(NULL != temp_arr);
	
	if (left < right)
	{
		mid = (left + right) / 2;
		
		RecMergeSort(arr_to_sort, left, mid, temp_arr);
		RecMergeSort(arr_to_sort, mid + 1, right, temp_arr);
		Merge(arr_to_sort, left, mid, right, temp_arr);
	}
}

/*------------------------------- Selection Sort --------------------------------*/

void SelectionSort(unsigned char **arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	size_t min_index = 0;
	
	assert(arr);
	
	for (i = 0; i < size - 1; ++i)
	{
		min_index = i;
		
		for (j = i + 1; j < size; ++j)
		{
			if (arr[j] < arr[min_index])
			{
				min_index = j;
			}
		}
		
		if (i != min_index)
		{
			Swap((arr + i), (arr + min_index));
		}
	}
}

/*-------------------------------- SwapInts ----------------------------------*/

static void Swap(unsigned char **elem1, unsigned char **elem2)
{
	unsigned char *temp = NULL;
	
	assert(elem1);
	assert(elem2);
	
	temp = *elem1;
	*elem1 = *elem2;
	*elem2 = temp;
}

