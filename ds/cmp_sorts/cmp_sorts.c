/*************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 03/01/23	                  *
*	Valgrind & gdb passed             *
*	Subject: Comparison Sort   		  *
*				                	  *
**************************************/

#include <assert.h>  /* assert */
#include <stddef.h>  /* size_t */

#include "cmp_sorts.h" /* cmp_sorts */

#define OVER_SIZE (1);

enum status
{
	SUCCESS,
	FAIL
};

/*------------- Helper Funcs -------------*/
static void SwapInts(int *a, int *b);
static int *RecBinarySearchHelper(int *arr, int left, int right,
														   int element_to_find);
static void RecMergeSort(int *arr_to_sort, int left, int right, int *temp_arr);		
static void Merge(int *arr_to_sort, int left, int mid, int right, 
																 int *temp_arr);												   
static void InitTempLeftHalf(int *left_temp, int *arr, size_t left_temp_size, 
																  int left_arr);
static void InitTempRightHalf(int *right_temp, int *arr, size_t right_temp_size, 
																   int mid_arr);																  
static void PlaceCorrectInArr(int *arr_to_sort, int *left_temp, int *right_temp, 
		size_t left_temp_index, size_t right_temp_index, size_t left_temp_size, 
								size_t right_temp_size, size_t arr_start_index);																  
static void RecQuickSort(void *base, int left, int right, size_t size,
															 compar_func_t cmp);		
static int Partition(void *base, size_t left, size_t right, size_t size,
															 compar_func_t cmp);															 
static void SwapVoid(void *val1, void *val2, size_t size);															 
															 														  
/*------------------------------- Bubble Sort --------------------------------*/

void BubbleSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(arr);
	
	for (i = 0; i < size - 1; ++i)
	{
		for (j = 0; j < size - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				SwapInts((arr + j), (arr + j + 1));
			}
		}
	}
}

/*------------------------------- Selection Sort --------------------------------*/

void SelectionSort(int *arr, size_t size)
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
			SwapInts((arr + i), (arr + min_index));
		}
	}
}

/*----------------------------- Insertion Sort -------------------------------*/

void InsertionSort(int *arr, size_t size)
{
	size_t i = 0;
	int j = 0;
		
	assert(arr);
	
	for (i = 1; i < size; ++i)
	{
		int i_val = arr[i];
		
		for (j = (i - 1); (j >= 0) && (arr[j] > i_val); --j)
		{
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = i_val;
	}
}

/*---------------------------- IterBinarySearch ------------------------------*/

int *IterBinarySearch(int *arr, size_t arr_size, int element_to_find)
{
	int left = 0;
	int right = 0;
	int mid = 0;
	
	assert(NULL != arr);
	
	right = arr_size - OVER_SIZE;

	while (left <= right)
	{
		mid = (right + left) / 2; 
		
		if (element_to_find == arr[mid])
		{
			return &arr[mid];
		}
		else if (element_to_find < arr[mid])
		{
			right = mid - 1;
		}
		else 
		{
			left = mid + 1;
		}
	}
	
	return NULL;
}

/*----------------------------- RecBinarySearch ------------------------------*/

int *RecBinarySearch(int *arr, size_t arr_size, int element_to_find)
{
	int left = 0;
	int right = 0;

	assert(NULL != arr);
	
	left = 0;
	right = arr_size - OVER_SIZE;
	
	return RecBinarySearchHelper(arr, left, right, element_to_find);
}

/*------------------------------- MergeSort ----------------------------------*/

int MergeSort(int *arr_to_sort, size_t num_elements)
{
	int *temp_arr = NULL;
	
	assert(NULL != arr_to_sort);
	
	temp_arr = (int *)malloc(num_elements * sizeof(int));
	if (NULL == temp_arr)
	{
		return FAIL;
	}
	
	RecMergeSort(arr_to_sort , 0, num_elements - 1, temp_arr);
	
	free(temp_arr);
	temp_arr = NULL;
	
	return SUCCESS;
}

/*-------------------------------- Qsort -------------------------------------*/

void Qsort(void *base, size_t nmemb, size_t size, compar_func_t cmp)
{
	assert(NULL != base);
	assert(NULL != cmp);
	assert(0 < size);
	
	RecQuickSort(base, 0, nmemb - 1, size, cmp);
}


/*---------------------------- HELPER FUNCTION -------------------------------*/

/*-------------------------------- SwapInts ----------------------------------*/

static void SwapInts(int *num1, int *num2)
{
	int temp = 0;
	
	assert(num1);
	assert(num2);
	
	temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

/*------------------------- RecBinarySearchHelper ----------------------------*/

static int *RecBinarySearchHelper(int *arr, int left, int right,
														    int element_to_find)
{
	if (left <= right)
	{
		int mid = (right + left) / 2;
		
		if (element_to_find == arr[mid])
		{
			return arr + mid;
		}
		
		if (element_to_find > arr[mid])
		{
			return RecBinarySearchHelper(arr, mid + 1, right, element_to_find);
		}
		else 
		{
			return RecBinarySearchHelper(arr, left, mid - 1, element_to_find);
		}
	}

	return NULL;
}

/*----------------------------- RecMergeSort ---------------------------------*/

static void RecMergeSort(int *arr_to_sort, int left, int right, int *temp_arr)
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

/*--------------------------------- Merge ------------------------------------*/

static void Merge(int *arr_to_sort, int left, int mid, int right, int *temp_arr)
{
	int *left_temp = NULL;
	int *right_temp = NULL;
	
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

static void InitTempLeftHalf(int *left_temp, int *arr, size_t left_temp_size, 
																   int left_arr)
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

static void InitTempRightHalf(int *right_temp, int *arr, size_t right_temp_size, 
																	int mid_arr)
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

static void PlaceCorrectInArr(int *arr_to_sort, int *left_temp, int *right_temp, 
		size_t left_temp_index, size_t right_temp_index, size_t left_temp_size, 
								 size_t right_temp_size, size_t arr_start_index)
{
	assert(NULL != arr_to_sort);
	assert(NULL != left_temp);
	assert(NULL != right_temp);

	while ((left_temp_index < left_temp_size) && 
		   (right_temp_index < right_temp_size))
	{
		if ((left_temp[left_temp_index]) <= (right_temp[right_temp_index]))
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

/*------------------------------- RecQuickSort -------------------------------*/

static void RecQuickSort(void *base, int left, int right, size_t size,
															  compar_func_t cmp)
{
	int pivot_index = 0;
	
	assert(NULL != base);
	assert(NULL != cmp);
	assert(0 < size);
	
	if (left >= right)
	{
		return;
	}
		pivot_index = Partition(base, left, right, size, cmp);
		
		RecQuickSort(base, left, pivot_index - 1, size, cmp);
		RecQuickSort(base, pivot_index + 1, right, size, cmp);
}

/*-------------------------------- Partition -------------------------------*/

static int Partition(void *base, size_t left, size_t right, size_t size,
															  compar_func_t cmp)
{
	void *pivot_ptr = NULL;
	void *index_val = NULL;
	void *elem_ptr = NULL;			

	size_t pivot_index = 0;
	size_t i = 0;
	
	assert(NULL != base);
	assert(NULL != cmp);
	assert(0 < size);
	
	pivot_ptr = (void *)((char *)base + (right * size));
	pivot_index = left;
	
	for (i = left; i < right; ++i)
	{	
		elem_ptr = (char *)base + (i * size);
		
		if (0 >= cmp(elem_ptr, pivot_ptr))
		{
			index_val = (char *)base + (pivot_index * size);
			elem_ptr = (char *)base + (i * size);
			SwapVoid(index_val, elem_ptr, size);
			
			++pivot_index;
		}
	}
	
	index_val = (char *)base + (pivot_index * size);

	SwapVoid(index_val, pivot_ptr, size);
	
	return pivot_index;
}

/*-------------------------------- SwapVoid -------------------------------*/

static void SwapVoid(void *val1, void *val2, size_t size)
{
	unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	unsigned char temp = '\0';
	size_t i = 0;
	
	assert(NULL != val1);
	assert(NULL != val2);
	
	ptr1 = (unsigned char *)val1;
	ptr2 = (unsigned char *)val2;
	
	for (i = 0; i < size; ++i)
	{
		temp = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = temp;
		
		++ptr1;
		++ptr2;
	}
}




