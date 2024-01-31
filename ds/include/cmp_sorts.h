/*************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 03/01/23	                  *
*	Valgrind & gdb passed             *
*	Subject: Comparison Sort   		  *
*				                	  *
**************************************/

#ifndef __CMP_SORT_H__
#define __CMP_SORT_H__

#include <stdlib.h>		/* size_t */

typedef int (*compar_func_t)(const void *, const void *);

/*---------------------------------------------------------------------------
Description:
	Insertion Sort is a comparison based sorting algorithm.
	It inserts every array element into its proper position.
	In the i-th iteration, all previous (i-1) elements are sorted,
	and the i-th element is inserted into its proper place
	in the previously sorted subarray.
	
Return Value:
	None. Sorting inplace.
	
Complexity: 
	time: O(n^2)  / space: O(1) 
---------------------------------------------------------------------------*/
void InsertionSort(int *arr, size_t size);

/*---------------------------------------------------------------------------
Description:
	Selection sort is a comparison based sorting algorithm.
	In every iterarion it selects the smallest element and places it at ths 
	smallest position. This algorithm divides the array into two parts:  
	sorted (left) and unsorted (right) subarray.
	It repeatedly selects the next smallest element.

Return Value:
	None. Sorting inplace.
	
Complexity: 
	time: O(n^2)  / space: O(1) 
---------------------------------------------------------------------------*/
void SelectionSort(int *arr, size_t size);

/*---------------------------------------------------------------------------
Description:
	Bubble sort is a comparison based sorting algorithm.
	It repeatedly compares successive elements and swaps them (if needed)
	until the largest element is in its right place.
Return Value:
	None. Sorting inplace.
	
Complexity: 
	time: O(n^2)  / space: O(1) 
---------------------------------------------------------------------------*/
void BubbleSort(int *arr, size_t size);

/*---------------------------------------------------------------------------
Complexity: 
	time: O(log(n))  / space: O(1) 
---------------------------------------------------------------------------*/
int *IterBinarySearch(int *arr, size_t arr_size, int element_to_find);

/*---------------------------------------------------------------------------
Complexity: 
	time: O(log(n))  / space: O(log(n)) 
---------------------------------------------------------------------------*/
int *RecBinarySearch(int *arr, size_t arr_size, int element_to_find);

/*---------------------------------------------------------------------------
Complexity: 
	time: O(nlogn)  / space: O(n) 
---------------------------------------------------------------------------*/
int MergeSort(int *arr_to_sort, size_t num_elements);

/*---------------------------------------------------------------------------
Complexity: 
	time: avg: O(nlogn) worst: O(n^2)  / space: O(logn) 
---------------------------------------------------------------------------*/
void Qsort(void *base, size_t nmemb, size_t size, compar_func_t cmp);

#endif /* __CMP_SORT_H__ */

