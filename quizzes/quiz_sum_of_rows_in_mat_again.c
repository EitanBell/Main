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
int SumRowsInMatTest(void);
int RowMajorTest(void);
void AllSumsOfRows(int arr[][5], size_t m, size_t n, int *sumArr);
void AllSumsOfRowsRowMajor(int **arr, size_t m, size_t n, int *sumArr);

/*------------------------------------ main ----------------------------------*/

int main(void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	
	printf("\t   Sums Of Rows In Matrix: \t\t%s\n", TEST_RES(SumRowsInMatTest()));
	printf("\t   Sums Of Rows In Matrix - Row Major: \t%s\n", TEST_RES(RowMajorTest()));
	
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

/*---------------------------------- ResTests --------------------------------*/

int SumRowsInMatTest(void)
{
	int status = SUCCESS;
	size_t i = 0;
	int sumArr[4] = {0, 0, 0, 0};
	int res_arr[4] = {15, 40, 65, 90};
	int arr[4][5] = {{1, 2, 3, 4, 5}, 
					{6, 7, 8, 9, 10},
					{11, 12, 13, 14, 15},
					{16, 17, 18, 19, 20}};

    AllSumsOfRows(arr, 4, 5, sumArr);
    
    for (i = 0; 4 > i; ++i)
	{
		status += BOOL_TO_STATUS(sumArr[i] == res_arr[i]);
	}
	
	return status;
}

int RowMajorTest(void)
{
	int status = SUCCESS;
	size_t i = 0;
	int sumArr[4] = {0, 0, 0, 0};
	int res_arr[4] = {15, 40, 65, 90};
	int arr[4][5] = {{1, 2, 3, 4, 5}, 
					{6, 7, 8, 9, 10},
					{11, 12, 13, 14, 15},
					{16, 17, 18, 19, 20}};

    AllSumsOfRowsRowMajor((int **)arr, 4, 5, sumArr);
    
    for (i = 0; 4 > i; ++i)
	{
		status += BOOL_TO_STATUS(sumArr[i] == res_arr[i]);
	}
	
	return status;
}

/*------------------------------ AllSumsOfRows -------------------------------*/

void AllSumsOfRows(int arr[][5], size_t m, size_t n, int *sumArr)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != arr);
	assert(NULL != sumArr);
	
	for (i = 0; m > i; ++i)
	{
		for (j = 0; n > j; ++j)
		{
            sumArr[i] += arr[i][j];
		}
	}
}

/*-------------------------- AllSumsOfRowsRowMajor ---------------------------*/

void AllSumsOfRowsRowMajor(int **arr, size_t m, size_t n, int *sumArr)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != arr);
	assert(NULL != sumArr);
	
	for (i = 0; m > i; ++i)
	{
		for (j = 0; n > j; ++j)
		{
            sumArr[i] += ((int *)arr)[i * n + j];		//Row Major
		}
	}
}


