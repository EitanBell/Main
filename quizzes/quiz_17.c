

#include <stdio.h>		/* printf */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 FAIL" : "\xE2\x9C\x93 SUCCESS")

int IsNumInArray(int *arr, size_t size, int num);

int main(void)
{
	int arr1[10] = {11, 22, 53, 46, 5, 62, 77, 0, 5, 1200};
	int arr2[7] = {1, 2, 3, 4, 8888, 6, 7};
	int arr3[6] = {5555, 5, 55, 55555, 555555, 555};
	
	if (IsNumInArray(arr1, 10, 5))
	{
		printf("5 is in arr1\n");
	}
	else
	{
		printf("5 is not in arr1\n");
	}
	
	if (IsNumInArray(arr2, 7, 5))
	{
		printf("5 is in arr2\n");
	}
	else
	{
		printf("5 is not in arr2\n");
	}

	if (IsNumInArray(arr3, 6, 5))
	{
		printf("5 is in arr3\n");
	}
	else
	{
		printf("5 is not in arr3\n");
	}
	
	return 0;
}

int IsNumInArray(int *arr, size_t size, int num)
{
	int status = 1;
	size_t i = 0;
	
	for (i = 0; size > i; ++i)
	{
		status = status * (arr[i] ^ num);
	}
	
	return (!status);
}


