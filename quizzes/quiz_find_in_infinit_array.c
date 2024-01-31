/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 12/06/23	                  *
*	Subject: Find in infinit array    *
**************************************/

#include <stdio.h>		/* printf */
#include <stddef.h>		/* ssize_t */
#include <assert.h>		/* assert */	

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

enum status
{
	SUCCESS,
	FAIL
};

int Test(void);
ssize_t FindInInfinitArray(size_t *arr, size_t target);
ssize_t BinarySearch(size_t *arr, size_t size, size_t target);

int main(void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	
	printf("\t\tFindInInfinitArray Test: \t%s\n", TEST_RES(Test()));
	
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

int Test(void)
{
	size_t arr[2000];
	ssize_t res = 0;
	
	for (int i = 0; i < 2000; i++)
	{
		arr[i] = i + 2;
	}
	
	res = (1998 == FindInInfinitArray(arr, 2000));
	res += (886 == FindInInfinitArray(arr, 888));
	res += (124 == FindInInfinitArray(arr, 126));
	
	if (3 == res)
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

ssize_t FindInInfinitArray(size_t *arr, size_t target)
{
	size_t left = 1;
	size_t right = 1;
	
	assert(NULL != arr);
	
	while (1)
	{
		if (target > arr[right - 1])
		{
			left = right;
			right *= 2; 
		}
		else
		{
			return BinarySearch(arr, right, target);
		}
	}	
}

ssize_t BinarySearch(size_t *arr, size_t size, size_t target)
{
	size_t left = 0;
	size_t right = size - 1;
	size_t middle = right / 2;
	
	assert(NULL != arr);
	assert(0 < size);
	
	while (left <= right)
	{
		if (target == arr[middle])
		{
			return middle;
		}
		else if (target > arr[middle])
		{
			left = middle;
			middle = (right + left) / 2;
		}
		else
		{
			right = middle;
			middle = (right + left) / 2;
		}
	}
	
	return -1;
}




