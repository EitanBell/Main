/*************************************
*	Developer: Eitan Bellaiche        *	
*	Date: 19/01/23	                  *
*	Find Two Misiing Nums In Array	  *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

void FindTwoMissingMath(int *arr, size_t size);
void FindTwoMissingXor(int *arr, size_t size);

int main(void)
{
	int arr[] = {2, 9, 7, 1, 4, 11, 8, 6, 5};
	size_t size = 9;
	size_t i = 0;
	
	printf("\nThe original array is: \n");
	for (i = 0; i < size - 1; ++i)
	{
		printf("%d, ", arr[i]);
	}
	printf("%d\n", arr[8]);
	
	FindTwoMissingMath(arr, size);
	
	FindTwoMissingXor(arr, size);
	
	return 0;
}

void FindTwoMissingMath(int *arr, size_t size)
{
	int sum_arr = 0;
	int missing_sum = 0;
	int full_sum = 0;
	int average_val = 0;
	int num1 = 0;
	int num2 = 0;
	size_t i = 0;
	
	assert(arr);
	assert(0 < size);
	
	for (i = 0; i < size; ++i)
	{
		sum_arr += arr[i];
	}
	
	full_sum = (size + 2) * (size + 3) / 2;

	missing_sum = full_sum - sum_arr;
	average_val = missing_sum / 2;
	
	sum_arr = 0;
	for (i = 0; i < size; ++i)
	{
		if (arr[i] <= average_val)
		{
			sum_arr += arr[i];
		}
	}
	num1 = ((average_val * (average_val + 1)) / 2) - sum_arr;
	num2 = missing_sum - num1; 
	
	printf("\nThe missing elements in MATH way are : %d, %d\n", num1, num2);
}

void FindTwoMissingXor(int *arr, size_t size)
{
	int xor_arr = 0;
	int set_bit_on = 0;
	int num1 = 0;
	int num2 = 0;
	size_t i = 0;
	
	for (i = 0; size > i; ++i)
	{
		xor_arr ^= arr[i];
	}
	
	for (i = 1; i <= size + 2; ++i)
	{
		xor_arr ^= i;
	}
	
	set_bit_on = xor_arr & ~(xor_arr - 1);
	
	for (i = 0; size > i; ++i)
	{
		(arr[i] & set_bit_on) ? (num1 ^= arr[i]) : (num2 ^= arr[i]);
	}
	
	for (i = 1; (size + 2) >= i; ++i)
	{
		(i & set_bit_on) ? (num1 ^= i) : (num2 ^= i);
	}
	
	printf("\nThe missing elements in XOR way are : %d, %d\n\n", num1, num2);
}

