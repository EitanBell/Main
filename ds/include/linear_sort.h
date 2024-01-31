/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Tal                     *
*	Date: 16/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Linear sort   			  *
*				                	  *
**************************************/

#ifndef __LINEAR_SORT_H__
#define __LINEAR_SORT_H__

/*********************************************************
General Description
	Algorithms to sort an array of ints with a linear time
	complexity. Counting sort has a time complexity of O(n+k),
	and Radix sort O(d(n+b)).

Attributes
	Sorting, Linear Time Complexity.
	
Main Functions
	Counting Sort, Radix Sort
*********************************************************/

/*   Includes   */

#include <stddef.h> /* size_t */

/*   Function Declarations   */

/*******************************************************************
Description
	Counting sort is a linear sort algorithm.
	It based on the fact that all the elements have maximum value of k.
	NOTE!
		The elements should be >= 0; Otherwise undifined behavior may happand.
Parameters
	int *arr - Pointer to an array.
	size_t size - The size of the array
Return Value
	status:
		0 for success.
		1 for failure in allocating additional memory.
Complexity
	Time: O(n + k) / Space: O(n + k) 
*******************************************************************/
int CountingSort(int *arr, size_t size);

/*******************************************************************
Description
	Radix sort is a linear sort algorithm.
	It based on the facts:
		- All the elements are integers and has d digits from any base b.
		- The data has a max range of elements.
		- Uses a stable sorting (like counting_sort).
	NOTE! 
		- All the elements in this function are in decimal base.
		- The elements should be >= 0; Otherwise undifined behavior may happand.
Parameters
	int *arr - Pointer to an array.
	size_t size - The size of the array
	size_t exp_of_ten - The number of digits in elements' base.
Return Value
	status:
		0 for success.
		1 for failure.
Comments
	The power of ten will determine how many digits are evaluated
	in each iteration, e.g.:
		exp_of_ten = 1, 10-based, 1 digit evaluated in each iteration.
		exp_of_ten = 2, 100-based, 2 digit evaluated in each iteration.
		etc.
Complexity
	Time: O(d(n + b) / Space: O(n)
	  d - Number of digits in input data
	  b - Base of data representation
*******************************************************************/
int RadixSort(int *arr, size_t size, size_t exp_of_ten);

#endif /* __LINEAR_SORT_H__ */
