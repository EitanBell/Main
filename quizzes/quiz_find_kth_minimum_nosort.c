

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static int KthSmallest(int *arr, size_t len, size_t k);

int main()
{
	int arr[] = {4, 6, 1, 11, 111, 1111, -5};
	int arr_len = sizeof(arr) / sizeof(*arr);
	int k = 6;
	int res = 0;
	
	res = KthSmallest(arr, arr_len, k);
	
	printf("The k's minimun element is - %d. Should be 111.\n", res);
	return 0;
}

static int KthSmallest(int *arr, size_t len, size_t k)
{
	size_t i = 0;
	size_t j = 0;
     size_t counter = 0;
	int is_found = 0;
	
	assert(NULL != arr);
    
    	while (!is_found && i < len)
    {
        j = 0;
        counter = 0;
        
        while (j < len)
        {
            if (arr[i] > arr[j])
            {	
                ++counter;
            }
            
            ++j;
        }
        
        if ((k - 1) == counter)
        {
        	 is_found = 1;
        }
        
        else
        {
    		 ++i;
    	   }
    }
    
    return arr[i];
}



