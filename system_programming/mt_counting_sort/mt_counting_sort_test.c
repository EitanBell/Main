/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Yuval                   	   *
*	Date: 09/03/23	             	       *
*	Subject: Multi Threaded Counting Sort  *
*				   						   *
*******************************************/
#define _GNU_SOURCE					   			        
#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* calloc, free*/
#include <stddef.h>  /* size_t */
#include <assert.h>  /* assert */
#include <time.h> 	 /* time */
#include <string.h>  /* memcpy */

#include "mt_counting_sort.h" /* mt_counting_sort API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define DICT_SIZE (985084)
#define NUM_OF_DICTS (10)
#define MAX_THRD_NUM (32750)

enum status	
{
	SUCCESS = 0,
	FAIL
};

/*------------- Print Funcs -------------*/
static void PrintArrays(unsigned char *arr, size_t size);
static void CountingSortPrint(unsigned char *arr, size_t size);
static void SortingBasicChecks(unsigned char *arr1, unsigned char *arr2, unsigned char *arr3);
static int IsSorted(unsigned char *arr, int len);
static int BigDataDictSorts(void);

/*------------- Helper Funcs -------------*/
int IsSorted(unsigned char *arr, int len);

int main(void)
{
	unsigned char arr1[11] = {'r', 'a', 'Z', 'A', '2', '1', '5', 'H', '2', '4', '3'};
	unsigned char arr2[11] = {'z', 'y', 't', 's', 'm', 'l', 'g', 'f', 'c', 'b', 'a'};
	unsigned char arr3[2] = {'B', 'A'};
	
	SortingBasicChecks(arr1, arr2, arr3);
	
	printf("\n\t******************** TESTS BEGIN *******************\n\n");
	
	printf("\t\tMTCounting Sort Test_1:   %s\n", TEST_RES(IsSorted(arr1, 11)));
	printf("\t\tMTCounting Sort Test_2:   %s\n", TEST_RES(IsSorted(arr2, 11)));
	printf("\t\tMTCounting Sort Test_3:   %s\n", TEST_RES(IsSorted(arr3, 2)));
	
	printf("\n\t\tBigData Counting Sort Test:\n");
	;
	if (SUCCESS != BigDataDictSorts())
	{
		return 1;
	}
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");

	return 0;
}


/*------------------------------- PrintArrays --------------------------------*/

static void PrintArrays(unsigned char *arr, size_t size)
{
	size_t i = 0;
	
	assert(arr);
	
	for (i = 0; i < size - 1; ++i)
	{
		printf("%c, ", arr[i]);
	}
	printf("%c", arr[size - 1]);
}


/*------------------------------- CountingSortPrint --------------------------------*/

static void CountingSortPrint(unsigned char *arr, size_t size)
{
	assert(arr);
	
	printf("\tOriginal array: ");
	PrintArrays(arr, size);

	MTCountingSort(arr, size, 8);
	
	printf("\n");
	
	printf("\tMTCountingSort array: ");
	PrintArrays(arr, size);

	printf("\n");
}

/*--------------------------- SortingBasicChecks ----------------------------*/

static void SortingBasicChecks(unsigned char *arr1, unsigned char *arr2, 
															unsigned char *arr3)
{
	printf("\n\t\t--------- MTCounting Sort ---------\n\n");
	CountingSortPrint(arr1, 11);
	printf("\n");
	CountingSortPrint(arr2, 11);
	printf("\n");
	CountingSortPrint(arr3, 2);
	printf("\n");
}

/*------------------------------- CheckIfSorted ------------------------------*/

static int IsSorted(unsigned char *arr, int len)
{			
	int status = 0;											
	int i = 0;
	
	for (i = 0; i < len - 1; ++i) 
	{
		if (arr[i] > arr[i + 1]) 
		{
            status += 1;
		}    
	} 	
	return status;
}

/*----------------------------- BigDataDictSorts -----------------------------*/

static int BigDataDictSorts(void)
{
	FILE *fp = NULL;
	unsigned char *dict = NULL;
	unsigned char *big_data = NULL;
	size_t i = 1; 
	size_t j = 0;
	struct timespec start = {0};
	struct timespec end = {0};
	double elapsed = 0;
	int status = SUCCESS;
	
	if (NULL == (fp = fopen("/usr/share/dict/words", "r")))
	{
		printf("Can't read the file.\n");
	}
	
	dict = (unsigned char *)malloc((DICT_SIZE + 1) * sizeof(unsigned char));
	if (NULL == dict)
	{
		return FAIL;
	}
	
	*(dict + DICT_SIZE) = '\0';
	
	big_data = (unsigned char *)malloc(NUM_OF_DICTS * (DICT_SIZE + 1) * sizeof(unsigned char));
	if (NULL == big_data)
	{
		return FAIL;
	}
	
	for (i = 1; i < MAX_THRD_NUM; i <<= 1)
	{
		for (j = 0; j < NUM_OF_DICTS; ++j)
		{
			memcpy(big_data + (j * (DICT_SIZE + 1)), dict, (DICT_SIZE + 1));
		}
		clock_gettime(CLOCK_MONOTONIC, &start);
		MTCountingSort(big_data, NUM_OF_DICTS * DICT_SIZE + 1, i);
		clock_gettime(CLOCK_MONOTONIC, &end);
		
		elapsed = (end.tv_sec - start.tv_sec);
		elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

		printf("\t\tBig Data duration of %ld threads: %.4f sec \n", i, elapsed);
	}

	fclose(fp);
	fp = NULL;
	
	free(big_data);
	big_data = NULL;
	
	return status;

}

