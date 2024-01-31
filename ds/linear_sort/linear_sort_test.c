/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Tal                     *
*	Date: 16/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Linear sort   			  *
*				                	  *
**************************************/
					   			        
#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* calloc, free*/
#include <stddef.h>  /* size_t */
#include <assert.h>  /* assert */

#include "linear_sort.h" /* linear_sort */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define ARR_SIZE 5000

/*------------- Print Funcs -------------*/
static void PrintArrays(int *arr, size_t size);
static void CountingSortPrint(int *arr, size_t size);
static void SortingBasicChecks(void);
static void RadixSortPrint(int *arr, size_t size);

/*------------- Helper Funcs -------------*/
static void InitRandArr(int *arr, size_t len);
static void InitRandArrRadix(int *arr, size_t len);
int IsSorted(int *arr, int len);

int main(void)
{
	
	int arr1[ARR_SIZE] = {0};
	int arr2[ARR_SIZE] = {0};
	int arr3[ARR_SIZE] = {0};
	int arr4[ARR_SIZE] = {0};
	
	SortingBasicChecks();
	
	InitRandArr(arr1, ARR_SIZE);
	InitRandArr(arr2, ARR_SIZE);
	
	CountingSort(arr1, ARR_SIZE);
	CountingSort(arr2, ARR_SIZE);
	
	InitRandArrRadix(arr3, ARR_SIZE);
	InitRandArrRadix(arr4, ARR_SIZE);

	RadixSort(arr3, ARR_SIZE, 2);
	RadixSort(arr4, ARR_SIZE, 3);
	
	printf("\t------- Test 5k randomize elements in range of 1 - 100 -------\n\n");

	printf("\t\tCounting Sort Test_1: \t\t%s\n", TEST_RES(IsSorted(arr1, ARR_SIZE)));
	printf("\t\tCounting Sort Test_2: \t\t%s\n\n", TEST_RES(IsSorted(arr2, ARR_SIZE)));
	printf("\t\tRadix Sort Test_1: \t\t%s\n", TEST_RES(IsSorted(arr3, ARR_SIZE)));
	printf("\t\tRadix Sort Test_2: \t\t%s\n\n", TEST_RES(IsSorted(arr4, ARR_SIZE)));

	return 0;
}


/*------------------------------- PrintArrays --------------------------------*/

static void PrintArrays(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(arr);
	
	for (i = 0; i < size - 1; ++i)
	{
		printf("%d, ", arr[i]);
	}
	printf("%d", arr[size - 1]);
/*	printf("\n");*/
}


/*------------------------------- CountingSortPrint --------------------------------*/

static void CountingSortPrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	CountingSort(arr, size);
	
	printf("\n");
	
	printf("\tCountingSort array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*------------------------------- RadixSortPrint --------------------------------*/

static void RadixSortPrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	RadixSort(arr, size, 2);
	
	printf("\n");
	
	printf("\tRadixSort array: ");
	PrintArrays(arr, size);

	printf("\n");
}



/*----------------------------- ArrInitialize --------------------------------*/

static void InitRandArr(int *arr, size_t len)
{
	size_t i = 0;
	
	assert(arr);
	
	for (i = 0; i < len; ++i)
	{
		arr[i] = (rand() % 100) + 1;
	}
}

/*----------------------------- InitRandArrRadix --------------------------------*/

static void InitRandArrRadix(int *arr, size_t len)
{
	size_t i = 0;
	
	assert(arr);
	
	for (i = 0; i < len; ++i)
	{
		arr[i] = (rand() % 1000000) + 2000000;
	}
}

/*--------------------------- SortingBasicChecks ----------------------------*/

static void SortingBasicChecks(void)
{
	int arr1[11] = {7, 1, 3, 1, 2, 4, 5, 3, 2, 4 ,3};
	int arr2[11] = {1, 2, 1, 2, 3, 3, 15, 0, 20, 7, 5};
	int arr3[2] = {5, 4};
	int arr4[11] = {7, 1, 3, 1, 2, 4, 5, 3, 2, 4 ,3};
	int arr5[11] = {1, 2, 1, 2, 3, 3, 15, 0, 20, 7, 5};
	int arr6[2] = {5, 4};

	printf("\n\t\t--------- Counting Sort ---------\n\n");
	CountingSortPrint(arr1, 11);
	printf("\n");
	CountingSortPrint(arr2, 11);
	printf("\n");
	CountingSortPrint(arr3, 2);
	printf("\n");
	printf("\n\t\t--------- Radix Sort ---------\n\n");
	RadixSortPrint(arr4, 11);
	printf("\n");
	RadixSortPrint(arr5, 11);
	printf("\n");
	RadixSortPrint(arr6, 2);
	printf("\n");

}

/*--------------------------- CheckIfSorted ----------------------------*/

int IsSorted(int *arr, int len)
{			
	int status = 0;											
	int i = 0;
	
	for (i = 0; i < len - 1; ++i) 
	{
		if (arr[i] > arr[i + 1]) 
		{
            status = 1;
		}    
	} 	
	return status;
}




