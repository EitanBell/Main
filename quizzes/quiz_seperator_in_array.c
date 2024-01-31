

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
							     "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

int FindEqualSumInArray(const int *arr, size_t size);
static int Test(void);

int main (void)
{
	printf("\n******************** TESTS BEGIN *******************\n\n");
	
	printf("\tTest Result: \t\t%s\n", TEST_RES(Test()));
	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	return 0;
}

static int Test(void)
{
	int arr1[] = {1, 4, 2, 5};
	int arr2[] = {2, 3, 4, 1, 4, 5};
	int status = 1;
	
	if (2 == FindEqualSumInArray(arr1, 4) 
		&& 1 == FindEqualSumInArray(arr2, 6))
	{
		status = 0;	
	}
	
	return status;
}
int FindEqualSumInArray(const int *arr, size_t size)
{
	int left_sum = 0;
	int right_sum = 0;
	int return_val = -1;
	size_t i = 0;

	assert(arr);
	assert(size);

	for (i = 0; size > i; ++i)
	{
		right_sum += arr[i];
	}	
	
	for (i = 0; size > i; ++i)
	{
		right_sum -= arr[i];
		
		if (right_sum == left_sum)
		{
			return_val = arr[i];
		}
		
		left_sum += arr[i];
	}
	
	return return_val;
}

