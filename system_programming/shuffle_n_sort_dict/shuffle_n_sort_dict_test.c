/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Aviv                    	   *
*	Date: 12/03/23	             	       *
*	Subject: Shuffle and Sort dict files   *
*				                		   *
*******************************************/
#define _GNU_SOURCE					   			        
#include <stdio.h>	 /* printf */
#include <stdlib.h>	 /* calloc, free*/
#include <stddef.h>  /* size_t */
#include <assert.h>  /* assert */
#include <time.h> 	 /* time */
#include <string.h>  /* memcpy */

#include "shuffle_n_sort_dict.h" /* mt_counting_sort API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define DICT_SIZE (985084)
#define DICT_WORDS_NUM (104334)
#define NULL_TERM (1)
#define COPIES_NUM (10)
#define MAX_THRD_NUM (32750)

enum status	
{
	SUCCESS = 0,
	FAIL
};

/*----------------------------------------------------------------------------*/
/*                         STATIC FUNCTIONS DECLATATION                       */
/*----------------------------------------------------------------------------*/
static unsigned char *ReadDict(void);
static unsigned char **CreateWordsArray(unsigned char *dict);
static unsigned char **CreateCopiesArr(unsigned char **words_arr, 
														   	 size_t copies_num);
static void ShuffleWords(unsigned char **words_arr, size_t copies_num);
static int ShuffleFunc(const void *word1, const void *word2);
														   	 
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTION                               */
/*----------------------------------------------------------------------------*/
int main(void)
{
	unsigned char *dict = NULL;
	unsigned char **words_arr = NULL;
	unsigned char **arr_copies = NULL;
	unsigned char **arr_copies_run = NULL;
	struct timespec start;
	struct timespec end;
	double elapse = 0;
	size_t i = 0;
	
	dict = ReadDict();
	if (NULL == dict)
    {
    	return FAIL;
    }
    
	words_arr = CreateWordsArray(dict);
	if (NULL == words_arr)
    {
    	return FAIL;
    }
    
    arr_copies = CreateCopiesArr(words_arr, COPIES_NUM);
	if (NULL == arr_copies)
    {
    	return FAIL;
    }
    
    for (i = 1; i < MAX_THRD_NUM; i <<= 1)
	{
		ShuffleWords(arr_copies, COPIES_NUM);
		
		clock_gettime(CLOCK_MONOTONIC, &start);
		MTMergeSortDictionary(arr_copies, DICT_WORDS_NUM * COPIES_NUM, i);
		clock_gettime(CLOCK_MONOTONIC, &end);
    
/*	arr_copies_run = arr_copies;*/
/*	for (i = 0; (DICT_WORDS_NUM * COPIES_NUM) > i; ++i)*/
/*	{*/
/*		printf("%s, ", *arr_copies_run);*/
/*		++arr_copies_run;*/
/*	}*/
/*	printf("\n");*/
	
		elapse = (end.tv_sec - start.tv_sec);
		elapse += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

		printf("\t\tSort duration of %ld threads: %.4f sec \n", i, elapse);
	}
	
	free(dict);
	dict = NULL;
	
	free(words_arr);
	words_arr = NULL;
	
	free(arr_copies);
	arr_copies = NULL;
	
	return 0;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*--------------------------------- ReadDict ---------------------------------*/

static unsigned char *ReadDict(void)
{
	unsigned char *dict = NULL;
	char *dir_path = "/usr/share/dict/american-english";
	FILE *fp = NULL;
	
	fp = fopen(dir_path, "r");
	if (NULL == fp)
	{
		return NULL;
	}
	
	dict = (unsigned char *)calloc(sizeof(unsigned char), (DICT_SIZE + NULL_TERM));
	if (NULL == dict)
	{
		fclose(fp);
		fp = NULL;
		
		return NULL;
	}
	
	if (DICT_SIZE != fread(dict, 1, DICT_SIZE, fp))
	{
		return NULL;
	}
	
	*(dict + DICT_SIZE) = '\0';
	
	fclose(fp);
	fp = NULL;
	
	return dict;
}

/*----------------------------- CreateWordsArray -----------------------------*/

static unsigned char **CreateWordsArray(unsigned char *dict)
{
	unsigned char **words_arr = NULL;
	unsigned char **words_arr_run = NULL;
	unsigned char *dict_run = NULL;
	
	assert(NULL != dict);
    
    words_arr = (unsigned char **)calloc(sizeof(unsigned char *), DICT_WORDS_NUM);
    if (NULL == words_arr)
    {
    	return NULL;
    }
	
	dict_run = dict;
	words_arr_run = words_arr;
	
	while ('\0' != *dict_run)
    {
        *words_arr_run = dict_run;
        dict_run = (unsigned char *)strchr((char *)dict_run, '\n');
        *dict_run = '\0';
        
        ++dict_run;
        ++words_arr_run;
    }
	    
    return words_arr;
}

/*------------------------------ CreateArrCopies -----------------------------*/

static unsigned char **CreateCopiesArr(unsigned char **words_arr, 
														   	  size_t copies_num)
{
	unsigned char **arr_copies = NULL;
	size_t i = 0;
	
	assert(NULL != words_arr);
	assert(0 < copies_num);
	
	arr_copies = (unsigned char **)malloc(sizeof(unsigned char *) *
												 copies_num * DICT_WORDS_NUM);
	if (NULL == arr_copies)
	{
		return NULL;
	}
	
	for (i = 0; copies_num > i; ++i)
	{
		memcpy(arr_copies + (i * DICT_WORDS_NUM), words_arr, DICT_WORDS_NUM * 
													   sizeof(unsigned char *));
	}
	
	return arr_copies;
}

/*------------------------------ CreateArrCopies -----------------------------*/

static void ShuffleWords(unsigned char **words_arr, size_t copies_num)
{
	assert(NULL != words_arr);
	
	srand(time(NULL));
	
	qsort(words_arr, copies_num * DICT_WORDS_NUM, sizeof(unsigned char *), &ShuffleFunc);
}

/*------------------------------ CreateArrCopies -----------------------------*/

static int ShuffleFunc(const void *word1, const void *word2)
{
	(void)word1;
	(void)word2;
	
	return ((rand() % 2) ? 1 : -1);
} 

/*
Using Selection-Sort with 1 Thread:
	Sort Time: 32.810695 sec.
	
Using Quick-Sort in Thread Funtion:
		Sort duration of 1 threads: 0.9212 sec 
		Sort duration of 2 threads: 0.6231 sec 
		Sort duration of 4 threads: 0.4746 sec 
		Sort duration of 8 threads: 0.4390 sec ---> FASTEST (optimal)
		Sort duration of 16 threads: 0.4581 sec 
		Sort duration of 32 threads: 0.4950 sec 
		Sort duration of 64 threads: 0.5043 sec 
		Sort duration of 128 threads: 0.5332 sec 
		Sort duration of 256 threads: 0.5521 sec 
		Sort duration of 512 threads: 0.5833 sec 
		Sort duration of 1024 threads: 0.6252 sec 
		Sort duration of 2048 threads: 0.6570 sec 
		Sort duration of 4096 threads: 0.7525 sec 
		Sort duration of 8192 threads: 0.8917 sec 
		Sort duration of 16384 threads: 1.1995 sec

		-) The optimal num of threads depends on the number of cores
	 	   is used in the CPU, and depends on the algorithm is used.
*/

