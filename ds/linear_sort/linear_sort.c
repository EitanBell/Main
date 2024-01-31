/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Tal                     *
*	Date: 16/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Linear sort   			  *
*				                	  *
**************************************/

#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */
#include <stdlib.h> /* calloc, free */	

#include "linear_sort.h" /* linear_sort */

enum	
{
	SUCCESS = 0,
	FAIL
};

/*------------- Helper Funcs -------------*/
static size_t GetMaxVal(int *arr, size_t size);
static void SetAccumArr(int *accum_arr, int *arr, size_t max_val, size_t size);
static void SetSortedArr(int *sorted_arr, int *accum_arr, int *arr,
																   size_t size);
static void CopyArrays(int *from_arr, int *to_arr, size_t size);
static size_t PowOfTen(size_t power);
static void SetAccumArrRadix(int *accum_arr, int *arr, size_t base, size_t size,
																int chunk_step);
static void SetSortedArrRadix(int *sorted_arr, int *accum_arr, int *arr,
									  size_t size, size_t base, int chunk_step);
int CountingSortForRadix(int *arr, size_t size, size_t base, int chunk_step);
										 
/*------------------------------ CountingSort --------------------------------*/

int CountingSort(int *arr, size_t size)
{
	size_t max_val = 0;
	int *accum_arr = NULL;
	int *sorted_arr = NULL;
	
	assert(arr);
	assert(0 < size);
	
	max_val = GetMaxVal(arr, size);
	
	accum_arr = (int *)calloc(max_val + 1, sizeof(size_t));
	if (NULL == accum_arr)
	{
		return FAIL;
	}
	
	sorted_arr = (int *)calloc(size, sizeof(int));
	if (NULL == sorted_arr)
	{
		free(accum_arr);
		accum_arr = NULL;
		
		return FAIL;
	}
	
	SetAccumArr(accum_arr, arr, max_val, size);
	
	SetSortedArr(sorted_arr, accum_arr, arr, size);
	
	CopyArrays(sorted_arr, arr, size);
	
	free(sorted_arr);
	sorted_arr = NULL;
	
	free(accum_arr);
	accum_arr = NULL;
	
	return SUCCESS;
}

/*-------------------------------- RadixSort ---------------------------------*/

int RadixSort(int *arr, size_t size, size_t exp_of_ten)
{
	int max_val = 0;
	int base = 0;
	int chunk_step = 0;
	
	assert(arr);
	assert(0 < size);
	assert(0 < exp_of_ten);

	max_val = GetMaxVal(arr, size);
	base = PowOfTen(exp_of_ten);
	
	for (chunk_step = 1; (max_val / chunk_step) > 0; chunk_step *= base)
	{
		if (FAIL == CountingSortForRadix(arr, size, base, chunk_step))
		{
			return FAIL;
		}
	}
	
	return SUCCESS;
}

/*--------------------------- CountingSortForRadix ---------------------------*/

int CountingSortForRadix(int *arr, size_t size, size_t base, int chunk_step)
{
	int *accum_arr = NULL;
	int *sorted_arr = NULL;
	
	assert(arr);
	assert(0 < size);
	assert(0 < base);

	accum_arr = (int *)calloc(base, sizeof(int));
	if (NULL == accum_arr)
	{
		return FAIL;
	}
	
	sorted_arr = (int *)calloc(size, sizeof(int));
	if (NULL == sorted_arr)
	{
		free(accum_arr);
		accum_arr = NULL;
		
		return FAIL;
	}
	
	SetAccumArrRadix(accum_arr, arr, base, size, chunk_step);
	
	SetSortedArrRadix(sorted_arr, accum_arr, arr, size, base, chunk_step);
	
	CopyArrays(sorted_arr, arr, size);
	
	free(sorted_arr);
	sorted_arr = NULL;
	
	free(accum_arr);
	accum_arr = NULL;
	
	return SUCCESS;
}


/*---------------------------- HELPER FUNCTION -------------------------------*/

/*-------------------------------- GetMaxVal ---------------------------------*/

static size_t GetMaxVal(int *arr, size_t size)
{
	int max = 0;
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	
	return max;
}

/*------------------------------- SetAccumArr --------------------------------*/

static void SetAccumArr(int *accum_arr, int *arr, size_t max_val, size_t size)
{
	size_t i = 0;
	
	assert(accum_arr);
	assert(arr);
	assert(0 < size);
	assert(0 < max_val);
	
	for (i = 0; i < size; ++i)
	{
		accum_arr[arr[i]] += 1;
	}
	
	for (i = 1; i <= max_val; ++i)
	{
		accum_arr[i] += accum_arr[i - 1];
	}
}

/*------------------------------- SetAccumArrRadix --------------------------------*/

static void SetAccumArrRadix(int *accum_arr, int *arr, size_t base, size_t size,
																 int chunk_step)
{
	size_t i = 0;
	
	assert(accum_arr);
	assert(arr);
	assert(0 < size);	
	
	for (i = 0; i < size; ++i)
	{
		accum_arr[(arr[i] / chunk_step) % base] += 1;
	}
	
	for (i = 1; i < base; ++i)
	{
		accum_arr[i] += accum_arr[i - 1];
	}
}

/*------------------------------- SetSortedArr -------------------------------*/

static void SetSortedArr(int *sorted_arr, int *accum_arr, int *arr, size_t size)
{
	size_t i = 0;
	
	assert(sorted_arr);
	assert(accum_arr);
	assert(arr);
	assert(0 < size);
	
	for (i = size - 1; i > 0; --i)
	{
		sorted_arr[accum_arr[arr[i]] - 1] = arr[i];
		--accum_arr[arr[i]];
	}
	sorted_arr[accum_arr[arr[0]] - 1] = arr[0];
}

/*---------------------------- SetSortedArrRadix -----------------------------*/

static void SetSortedArrRadix(int *sorted_arr, int *accum_arr, int *arr,
									   size_t size, size_t base, int chunk_step)
{
	int i = 0;
	
	assert(sorted_arr);
	assert(accum_arr);
	assert(arr);
	
	for (i = (size - 1); i >= 0; --i)
	{
		sorted_arr[accum_arr[(arr[i] / chunk_step) % base] - 1] = arr[i];
		--accum_arr[(arr[i] / chunk_step) % base];
	}
}

/*------------------------------- SetAccumArr --------------------------------*/

static void CopyArrays(int *from_arr, int *to_arr, size_t size)
{
	size_t i = 0;
	
	assert(from_arr);
	assert(to_arr);
	assert(0 < size);
	
	for (i = 0; i < size; ++i)
	{
		to_arr[i] = from_arr[i];
	}
}

/*------------------------------- PowOfTen --------------------------------*/

static size_t PowOfTen(size_t power)
{
	size_t base = 1;
	
	while (0 < power)
	{
		base *= 10;
		--power;
	}
	
	return base;
}

 
