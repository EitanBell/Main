

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */

void SortBinaryArr(int *arr, size_t size);
static void Swap(int *num1, int *num2);

int main (void)
{
	int arr1[12] = {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0};
	int arr2[6] = {0, 1, 1, 0, 0, 1};
	size_t i = 0;
	
	printf("\nOriginal arrays: \n");
	
	for (i = 0; 11 > i; ++i)
	{
		printf("%d, ", arr1[i]);
	}
	printf("%d", arr1[11]);
	
	printf("\n");
	
	for (i = 0; 5 > i; ++i)
	{
		printf("%d, ", arr2[i]);

	}
	printf("%d", arr2[5]);
	
	printf("\n");	
	
	printf("\nSorted arrays: \n");
	SortBinaryArr(arr1, 12);
	SortBinaryArr(arr2, 6);
	
	for (i = 0; 11 > i; ++i)
	{
		printf("%d, ", arr1[i]);
	}
	printf("%d", arr1[11]);
	
	printf("\n");
	
	for (i = 0; 5 > i; ++i)
	{
		printf("%d, ", arr2[i]);

	}
	printf("%d", arr2[5]);
	
	printf("\n\n");	
	
	return 0;
}


void SortBinaryArr(int *arr, size_t size)
{
	int *right = NULL;
	int *left = NULL;
	
	assert(NULL != arr);
	assert(0 < size);
	
	left = arr;
	right = arr + (size - 1);
	
	while (left < right)
	{
		if (1 == *left && 0 == *right)
		{
			Swap(left, right);
		}
		else if (1 == *left && 1 == *right)
		{
			--right;
		}
		
		else if (0 == *left && 0 == *right)
		{
			++left;
		}
		else
		{
			--right;
			++left;
		}
	} 
}

static void Swap(int *num1, int *num2)
{
	assert(NULL != num1);
	assert(NULL != num2);
	
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;
}


