/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Subject: C Basics			      *
**************************************/

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* malloc */
#include <assert.h>		/* assert */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) (bool ^ 1)

enum status
{
	SUCCESS,
	FAIL
};

/*--------------------------- FUNCS DECLARATIONS -----------------------------*/					   			        
int ResTests(void);					   			        
int IsBitTwoAndSixOn(unsigned char num);
int IsBitTwoOrSixOn(unsigned char num);
void Swap3_5Bits(unsigned char *ch);
void SwapInts(int *a, int *b);
void SwapPInts(int **a, int **b);
void SwapVoids(void **a, void **b);
int *CreateArr(size_t size);
int *Create2DArr(size_t n, size_t m);
void ResetArr(int *arr, size_t size);
void Reset2DArr(int *arr, size_t n, size_t m);

/*------------------------------------ main ----------------------------------*/

int main(void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tC Basics Tests: \t%s\n", TEST_RES(ResTests()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

/*---------------------------------- ResTests --------------------------------*/

int ResTests(void)
{
	int status_bits = SUCCESS;
	int status_swaps = SUCCESS;
	int status_arrays = SUCCESS;
	
	int x = 5;
	int y = 2;
	unsigned char ch = 'l';
	int *px = &x;
	int *py = &y;
	
	void *p_name = "Eitan";
	void *f_name = "Bell";
	
	int *arr = NULL;
	int *arr2d = NULL;
	size_t i = 0;
	size_t j = 0;
	
	status_bits += BOOL_TO_STATUS(IsBitTwoAndSixOn(255));
	status_bits += BOOL_TO_STATUS(!IsBitTwoAndSixOn(0));
	status_bits += BOOL_TO_STATUS(IsBitTwoOrSixOn(254));
	status_bits += BOOL_TO_STATUS(!IsBitTwoOrSixOn(0));
	Swap3_5Bits(&ch);
	status_bits += (ch ^ 'x');
	
	SwapInts(&x, &y);
	status_swaps += BOOL_TO_STATUS((x == 2) && (y == 5));
	SwapPInts(&px, &py);
	status_swaps += BOOL_TO_STATUS((*px == 5) && (*py == 2));
	SwapVoids(&p_name, &f_name);
	status_swaps += BOOL_TO_STATUS((*(int *)p_name == 'B') && (*(int *)f_name == 'E'));
	
	arr = CreateArr(3);
	for (i = 0; 3 > i; ++i)
	{
		arr[i] = i;
	}
	status_arrays += BOOL_TO_STATUS((arr[0] == 0) && (arr[1] == 1) && (arr[2] == 2));
	ResetArr(arr, 3);
	status_arrays += BOOL_TO_STATUS((arr[0] == 0) && (arr[1] == 0) && (arr[2] == 0));
	free(arr);
	
	arr2d = Create2DArr(2, 2);
	for (i = 0; 2 > i; ++i)
	{
		for (j = 0; 2 > j; ++j)
		{
			arr2d[2 * i + j] = i + j + 1;
		}
	}

	status_arrays += BOOL_TO_STATUS((arr[0] == 1) && (arr[1] == 2) && 
												(arr[2] == 2) && (arr[3] == 3));
	Reset2DArr(arr2d, 2, 2);
	status_arrays += BOOL_TO_STATUS((arr[0] == 0) && (arr[1] == 0) && 
												(arr[2] == 0) && (arr[3] == 0));
	free(arr2d);
	
	return(status_bits + status_swaps + status_arrays);
}

/*------------------------------ IsBitTwoAndSixOn ----------------------------*/

int IsBitTwoAndSixOn(unsigned char ch)
{
	return (((ch >> 1) & 1) & ((ch >> 5) & 1));
}

/*------------------------------ IsBitTwoOrSixOn ----------------------------*/

int IsBitTwoOrSixOn(unsigned char ch)
{
	return (((ch >> 1) & 1) | ((ch >> 5) & 1));
}

/*-------------------------------- Swap3_5Bits -------------------------------*/

void Swap3_5Bits(unsigned char *ch)
{
	unsigned char bit3 = 0;
	unsigned char bit5 = 0;
	unsigned char xor = 0;
	
	assert(NULL != ch);
	
	bit3 = (*ch >> 2) & 1;
	bit5 = (*ch >> 4) & 1; 
	xor = bit3 ^ bit5; 
	xor = (xor << 2) | (xor << 4);
	
	*ch = (*ch ^ xor);
}

/*-------------------------------- SwapInts -------------------------------*/

void SwapInts(int *a, int *b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

/*-------------------------------- SwapPInts -------------------------------*/

void SwapPInts(int **a, int **b)
{
	int *tmp = NULL;
	
	assert(NULL != a);
	assert(NULL != b);
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*-------------------------------- SwapVoids -------------------------------*/

void SwapVoids(void **a, void **b)
{
	size_t tmp = 0;
	
	assert(NULL != a);
	assert(NULL != b);
	
	tmp = *(size_t *)a;
	*(size_t *)a = *(size_t *)b;
	*(size_t *)b = tmp;
}

/*-------------------------------- CreateArr -------------------------------*/

int *CreateArr(size_t size)
{
	int *arr = NULL;
	
	assert(0 < size);
	
	arr = (int *)malloc(size * sizeof(int));
	if (NULL == arr)
	{
		return NULL;
	}
	
	return arr;
}

/*-------------------------------- Create2DArr -------------------------------*/

int *Create2DArr(size_t n, size_t m)
{
	int *arr = NULL;
	
	assert(0 < n);
	assert(0 < m);
	
	arr = (int *)malloc(n * m * sizeof(int));
	if (NULL == arr)
	{
		return NULL;
	}
	
	return arr;
}

/*-------------------------------- ResetArr -------------------------------*/

void ResetArr(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	assert(0 < size);
	
	for (i = 0; size > i; ++i)
	{
		arr[i] = 0;
	}
}

/*-------------------------------- Reset2DArr -------------------------------*/

void Reset2DArr(int *arr, size_t n, size_t m)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != arr);
	assert(0 < n);
	assert(0 < m);
	
	for (i = 0; n > i; ++i)
	{
		for (j = 0; m > j; ++j)
		{
			arr[n * i + j] = 0;
		}
	}
}

