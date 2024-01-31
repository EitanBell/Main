/*************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Part_1: Itay            *
*			  Part_2: Shauli     	  *
*	Date: 03/01/23	                  *
*	Valgrind & gdb passed             *
*	Subject: Comparison Sort   		  *
*				                	  *
**************************************/

#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* srand(), rand(), qsort() */
#include <stddef.h>  /* size_t */
#include <time.h>	 /* time(), clock() */
#include <assert.h>  /* assert */

#include "cmp_sorts.h" /* cmp_sorts */

#define ARR_SIZE 5000

/*------------- Print Funcs -------------*/
static void PrintArrays(int *arr, size_t size);
static void BubblePrint(int *arr, size_t size);
static void SelectionPrint(int *arr, size_t size);
static void InsertionPrint(int *arr, size_t size);
static void SortingBasicChecks(void);
static void IterBinarySearchPrint(int *arr, size_t arr_size, int element_to_find);
static void RecBinarySearchPrint(int *arr, size_t arr_size, int element_to_find);
static void MergeSortPrint(int *arr, size_t size);
static void QsortPrint(void *base, size_t nmemb, size_t size, compar_func_t cmp);

/*------------- Helper Funcs -------------*/
static void InitRandArr(int *arr, size_t len);
static int CompFunc(const void * p1, const void * p2);

int main(void)
{
	clock_t start_time = 0; 
	clock_t end_time = 0;
	double total_time = 0;
	int arr1[ARR_SIZE] = {0};
	int arr2[ARR_SIZE] = {0};
	int arr3[ARR_SIZE] = {0};
	int arr4[ARR_SIZE] = {0};
	int arr5[ARR_SIZE] = {0};
	int arr6[ARR_SIZE] = {0};
	
	srand(time(NULL));
	
	SortingBasicChecks();
	
	InitRandArr(arr1, ARR_SIZE);
	InitRandArr(arr2, ARR_SIZE);
	InitRandArr(arr3, ARR_SIZE);
	InitRandArr(arr4, ARR_SIZE);
	InitRandArr(arr5, ARR_SIZE);
	InitRandArr(arr6, ARR_SIZE);
	
	start_time = clock();
	BubbleSort(arr1, ARR_SIZE);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\n\t\t--------- \033[1;36m Time Measures \033[0m ---------\n");
	
	printf("\n\tDuration of BubbleSort of 5000 elements: %f\n", total_time);
	
	start_time = clock();
	SelectionSort(arr2, ARR_SIZE);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\tDuration of SelectionSort of 5000 elements: %f\n", total_time);
	
	start_time = clock();
	InsertionSort(arr3, ARR_SIZE);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\tDuration of InsertionSort of 5000 elements: %f\n\n", total_time);
	
	start_time = clock();
	MergeSort(arr6, ARR_SIZE);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\tDuration of MergeSort of 5000 elements: %f\n", total_time);
	
	start_time = clock();
	qsort(arr4, ARR_SIZE, sizeof(int), &CompFunc);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\tDuration of qsort of 5000 elements: %f\n", total_time);
	
	start_time = clock();
	Qsort(arr5, ARR_SIZE, sizeof(int), &CompFunc);
	end_time = clock();
	total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	
	printf("\tDuration of Qsort of 5000 elements: %f\n\n", total_time);

	
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
	printf("\n");
}


/*------------------------------- BubblePrint --------------------------------*/

static void BubblePrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	BubbleSort(arr, size);

	printf("\tBubbled array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*------------------------------- SelectionPrint --------------------------------*/

static void SelectionPrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	SelectionSort(arr, size);

	printf("\tSelectioned array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*---------------------------- InsertionPrint --------------------------------*/

static void InsertionPrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	InsertionSort(arr, size);

	printf("\tInsertioned array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*---------------------------- IterBinarySearchPrint --------------------------------*/

static void IterBinarySearchPrint(int *arr, size_t arr_size, int element_to_find)
{
	int *found = NULL;
	
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, arr_size);

	found = IterBinarySearch(arr, arr_size, element_to_find);

	(found == NULL)? printf("\t%d was not found\n", element_to_find) : 
					 printf("\tThe found element is: %d\n", *found);

	printf("\n");
}

/*---------------------------- RecBinarySearchPrint --------------------------*/

static void RecBinarySearchPrint(int *arr, size_t arr_size, int element_to_find)
{
	int *found = NULL;
	
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, arr_size);

	found = RecBinarySearch(arr, arr_size, element_to_find);

	(found == NULL)? printf("\t%d was not found\n", element_to_find) : 
					 printf("\tThe found element is: %d\n", *found);

	printf("\n");
}

/*----------------------------- MergeSortPrint -------------------------------*/

static void MergeSortPrint(int *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	MergeSort(arr, size);

	printf("\tMerged Sorted array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*----------------------------- QsortPrint -------------------------------*/

static void QsortPrint(void *base, size_t nmemb, size_t size, compar_func_t cmp)
{
	
	printf("\tOriginal array: ");
	PrintArrays((int *)base, nmemb);

	Qsort((int *)base, nmemb, size, cmp);

	printf("\tQuick sorted array: ");
	PrintArrays((int *)base, nmemb);

	printf("\n");
}

/*----------------------------- ArrInitialize --------------------------------*/

static void InitRandArr(int *arr, size_t len)
{
	size_t i = 0;
	
	assert(arr);
	
	for (i = 0; i < len; ++i)
	{
		arr[i] = rand();
	}
}

/*----------------------------- CompFunc --------------------------------*/

static int CompFunc(const void * p1, const void * p2)
{
	return (*(int*)p1 - *(int*)p2);
}


/*--------------------------- SortingBasicChecks ----------------------------*/

static void SortingBasicChecks(void)
{
	int arr1[8] = {5, 4, 7, 1, 6, 5, 10, -1};
	int arr2[11] = {1, 2, -1, -2, -3, 3, -200, 0, 40, 77, 5};
	int arr3[2] = {5, 4};
	int arr4[8] = {5, 4, 7, 1, 6, 5, 10, -1};
	int arr5[11] = {1, 2, -1, -2, -3, 3, -200, 0, 40, 77, 5};
	int arr6[2] = {5, 4};
	int arr7[8] = {5, 4, 7, 1, 6, 5, 10, -1};
	int arr8[11] = {1, 2, -1, -2, -3, 3, -200, 0, 40, 77, 5};
	int arr9[2] = {5, 4};
	int arr10[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	int arr11[11] = {2, 4, 7, 88, 89, 101, 102, 555, 561, 777, 6000};
	int arr12[2] = {4, 5};
	int arr13[8] = {5, 4, 7, 1, 6, 5, 10, -1};
	int arr14[11] = {1, 2, -1, -2, -3, 3, -200, 0, 40, 77, 5};
	int arr15[2] = {5, 4};
	int arr16[8] = {5, 4, 7, 1, 6, 5, 10, -1};
	int arr17[11] = {1, 2, -1, -2, -3, 3, -200, 0, 40, 77, 5};
	int arr18[2] = {5, 4};

	printf("\n\t\t--------- \033[1;36m Bubble Sort \033[0m ---------\n\n");
	BubblePrint(arr1, 8);
	BubblePrint(arr2, 11);
	BubblePrint(arr3, 2);
	
	printf("\n\t\t--------- \033[1;36m Selection Sort \033[0m ---------\n\n");
	SelectionPrint(arr4, 8);
	SelectionPrint(arr5, 11);
	SelectionPrint(arr6, 2);
	
	printf("\n\t\t--------- \033[1;36m Insertion Sort \033[0m ---------\n\n");
	InsertionPrint(arr7, 8);
	InsertionPrint(arr8, 11);
	InsertionPrint(arr9, 2);
	
	printf("\n\t\t--------- \033[1;36m IterBinarySearch \033[0m ---------\n\n");
	IterBinarySearchPrint(arr10, 8, 8);
	IterBinarySearchPrint(arr11, 11, 561);
	IterBinarySearchPrint(arr12, 2, 42);
	
	printf("\n\t\t--------- \033[1;36m RecBinarySearch \033[0m ---------\n\n");
	RecBinarySearchPrint(arr10, 8, 1);
	RecBinarySearchPrint(arr11, 11, 777);
	RecBinarySearchPrint(arr12, 2, 3);
	
	printf("\n\t\t--------- \033[1;36m Merge Sort \033[0m ---------\n\n");
	MergeSortPrint(arr13, 8);
	MergeSortPrint(arr14, 11);
	MergeSortPrint(arr15, 2);
	
	printf("\n\t\t--------- \033[1;36m Quick Sort \033[0m ---------\n\n");
	QsortPrint(arr17, 11, sizeof(int), CompFunc);
	QsortPrint(arr16, 8, sizeof(int), CompFunc);
	QsortPrint(arr18, 2, sizeof(int), CompFunc);
}



