

#include <stdio.h>	 /* printf */
#include <assert.h>	 /* assert */

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" :\
							    "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
							    
static long *SumIntPairsToLongs(int *ints, size_t elements_size);

int main(void)
{
	int ints[6] = {2, 9, 77, -7, 4, 16};
	long *long_arr = NULL;
	size_t size = 0;
	int status = 0;
	
	size = sizeof(ints) / sizeof(int);
	long_arr = SumIntPairsToLongs(ints, size);

	if (11 != long_arr[0] || 70 != long_arr[1] || 20 != long_arr[2])
	{
		status = 1;
	}
	printf("\n******************* TESTS BEGIN ********************\n\n");
    
     printf("\tTest Result: \t\t%s\n", TEST_RES(status));
    
     printf("\n****************** TESTS FINISHED ******************\n\n");
     return status;
}

static long *SumIntPairsToLongs(int *ints, size_t size)
{
	long *long_ptr = NULL;
	size_t i = 0;
	
	assert(ints);
	assert(size);
	
	long_ptr = (long *)ints;
	
	for (i = 0; i < size; i += 2)
	{
		*long_ptr = ((long)ints[i] + ints[i+1]);
		++long_ptr;
	}
	
	long_ptr = (long *)ints;
	
	return long_ptr;
}


