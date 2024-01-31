

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int FindMinKVal(int *arr, int len, int k);

int main()
{
	int arr[] = {4, 6, 1, 11, 111, 1111, -5};
	int arr_len = sizeof(arr) / sizeof(*arr);
	int k = 6;
	int res = 0;
	
	res = FindMinKVal(arr, arr_len, k);
	
	printf("The k's minimun element is - %d. Should be 111.\n", res);
	return 0;
}

int FindMinKVal(int *arr, int len, int k)
{
	int i = 0;
	int j = 0;
	int cur = 0;
	int ans = 0;
	
	int *arr2 = NULL;

	assert(NULL != arr);
	
	arr2 = (int *)malloc(sizeof(int) * len);
	if (NULL == arr)
	{
		return -99999999;
	}
	
	for (i = 0; i < len; ++i)
	{
		arr2[i] = arr[i];
	}

    for (i = 1; i < len; ++i) 
    {
        cur = arr2[i];
        j = i - 1;
        
        while (j >= 0 && arr2[j] > cur) 
        {
            arr2[j + 1] = arr2[j];
            j = j - 1;
        }
        arr2[j + 1] = cur;
    }
    ans = arr2[k];
    
    free(arr2);
    arr2 = NULL;
    
    return ans;
}

