
#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <stdlib.h> 	/* malloc, calloc */
#include <stddef.h> 	/* size_t */
#include <string.h>		/* strlen */

#define NULL_TERM (1)

enum bool_status
{
	FALSE = 0,
	TRUE
};

enum status
{
	SUCCESS = 0,
	FAIL
};

static char *GetMinAddress(char **add_arr, size_t len);
static char *GetMaxAddress(char **add_arr, size_t len);
static int SetAddressArr(char **add_arr, char *big, char *small, 
				   size_t small_len, size_t start_index, size_t *char_is_found);
char *GetMinSubStr(char *big, char *small);

int main (void)
{   
    char *big = "ombabadugunda";
    char *small = "nau";
    char *min_sub = NULL;

	char *big2 = "EiitttntaittEitantiititEiEaitin";
    char *small2 = "En";
    char *min_sub2 = NULL;
	
	printf("\n\t---------------------- TEST 1 ----------------------\n");
    
    min_sub = GetMinSubStr(big, small);
    if (NULL == min_sub)
	{
		printf("Error in memory allocation.\n");
		return 1;
	}
    
    printf("\n\t\tbig array: %s\n", big);
    printf("\t\tsmall array: %s\n", small);
    printf("\n\t\tThe result should be: 'unda'.\n");
    printf("\t\tThe result is: \t       %s.\n", min_sub);
    
    free(min_sub);
	min_sub = NULL;
	
	printf("\n\t---------------------- TEST 2 ----------------------\n");
	
    
    min_sub2 = GetMinSubStr(big2, small2);
    if (NULL == min_sub2)
	{
		printf("Error in memory allocation.\n");
		return 1;
	}
	
	printf("\n\t\tbig array: %s\n", big2);
    printf("\t\tsmall array: %s\n", small2);
    printf("\n\t\tThe result should be: 'Eitan'.\n");
    printf("\t\tThe result is: \t       %s.\n\n", min_sub2);
    
    free(min_sub2);
	min_sub2 = NULL;

    return 0;
}

char *GetMinSubStr(char *big, char *small)
{
	size_t min_len = 0;
	size_t tmp_len = 0;
	size_t small_len = 0;
	size_t char_is_found = TRUE;
	size_t i = 0;
	
	char **add_arr = NULL;
	
	char *min_add_run = NULL;
	char *max_add_run = NULL;
	
	char *min_add = NULL;
	char *res_str = NULL;
	
	assert(NULL != big);
	assert(NULL != small);
	
	small_len = strlen(small);
	add_arr = (char **)malloc(small_len * sizeof(char *));
	if (NULL == add_arr)
	{
		return NULL;	
	}
	
	i = 0;
	min_len = strlen(big);
	
	while (char_is_found)
	{	
		if (FAIL == SetAddressArr(add_arr, big, small, small_len, i++, 
																&char_is_found))
		{
			continue;
		}
		
		min_add_run = GetMinAddress(add_arr, small_len);
		max_add_run = GetMaxAddress(add_arr, small_len);

		tmp_len = max_add_run - min_add_run;
		
		if (tmp_len < min_len)
		{
			min_len = tmp_len;
			min_add = min_add_run;
		}
	}
	
	res_str = (char *)malloc(min_len);
	if (NULL == res_str)
	{
		free(add_arr);
		add_arr = NULL;
		
		return NULL;
	}
	
	memcpy(res_str, min_add, min_len + NULL_TERM);
	
	free(add_arr);
	add_arr = NULL;
	
	return res_str;
}

static char *GetMinAddress(char **add_arr, size_t len)
{
	size_t i = 0;
	char *min_add = NULL;
	
	assert(NULL != add_arr);

	min_add = add_arr[0];
	for (i = 1; len > i; ++i)
	{
		if (add_arr[i] < min_add)
		{
			min_add = add_arr[i];
		}
	}
	return min_add;
}

static char *GetMaxAddress(char **add_arr, size_t len)
{
	size_t i = 0;
	char *max_add = NULL;
	
	assert(NULL != add_arr);

	max_add = add_arr[0];
	for (i = 1; len > i; ++i)
	{
		if (add_arr[i] > max_add)
		{
			max_add = add_arr[i];
		}
	}
	return max_add;
}

static int SetAddressArr(char **add_arr, char *big, char *small, 
					size_t small_len, size_t start_index, size_t *char_is_found)
{
	size_t i = 0;
	
	assert(NULL != big);
	assert(NULL != small);
	
	for (i = 0; small_len > i; ++i)
	{
		add_arr[i] = strchr(big + start_index, small[i]);
		if (NULL == add_arr[i])
		{
			*char_is_found = FALSE;
			return FAIL;
		}
	}
	return SUCCESS;
}

