/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Subject: Sum of rows in a matrix  *
**************************************/

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* malloc */
#include <assert.h>		/* assert */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)

enum status
{
	SUCCESS,
	FAIL
};

/*--------------------------- FUNCS DECLARATIONS -----------------------------*/					   			        
int ResTests(void);					   			        
int *AllSumsOfRows(const int **mat, size_t m, size_t n);

/*------------------------------------ main ----------------------------------*/

int main(void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tSums Of Rows In Matrix Tests: \t%s\n", TEST_RES(ResTests()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

/*---------------------------------- ResTests --------------------------------*/

int ResTests(void)
{
	int status = SUCCESS;
	size_t i = 0;
	int *sums_arr = NULL;
	int res_arr[4] = {15, 40, 65, 90};
	int mat[4][5] = {{1, 2, 3, 4, 5}, 
					{6, 7, 8, 9, 10},
					{11, 12, 13, 14, 15},
					{16, 17, 18, 19, 20}};

    sums_arr = AllSumsOfRows((const int **)mat, 4, 5);
    
    for (i = 0; 4 > i; ++i)
	{
		status += BOOL_TO_STATUS(sums_arr[i] == res_arr[i]);
	}
	
	free(sums_arr);
	sums_arr = NULL;
	
	return status;
}

/*------------------------------- AllSumsOfRows ------------------------------*/

int *AllSumsOfRows(const int **mat, size_t m, size_t n)
{
	int row_sum = 0;
	size_t i = 0;
	size_t j = 0;
	int *sums_arr = NULL;
	
	assert(NULL != mat);
	
	sums_arr = (int *)malloc(m * sizeof(int));
	if (NULL == sums_arr)
	{
		return NULL;
	}
	
	for (i = 0; m > i; ++i)
	{
		row_sum = 0;
		
		for (j = 0; n > j; ++j)
		{
			row_sum += ((int *)mat)[i * n + j];		//Column Major
		}
		
		sums_arr[i] = row_sum;
	}
	
	return sums_arr;
}


