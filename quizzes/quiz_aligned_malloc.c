/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 02/03/23	                  *
*	Subject: Aligned Malloc			  *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <string.h> /* strcmp, strlen */
#include <stdlib.h> /* malloc, memcpy */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

static int Test(void);
static void *AlignedMalloc(size_t bytes, size_t align);
void AlignedFree(void *ptr);

int main (void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tAlignedMalloc All_Tests: \t%s\n", TEST_RES(Test()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	Test();
	
	return 0;
}

static int Test(void)
{
	int status = 0;
	int *test1 = NULL;
	int *test2 = NULL;
	int *test3 = NULL;
	int *test4 = NULL;
	int *test5 = NULL;
	int *test6 = NULL;
	
	test1 = (int *)AlignedMalloc(10, 2);
	status += (size_t)test1 % 2;
	
	printf("%d\n", (size_t)test1 % 2);
	test2 = (int *)AlignedMalloc(5, 8);
	status += (size_t)test1 % 8;
	test3 = (int *)AlignedMalloc(50, 16);
	status += (size_t)test1 % 16;
	test4 = (int *)AlignedMalloc(100, 32);
	status += (size_t)test1 % 32;
/*	test5 = (int *)AlignedMalloc(16, 1024);*/
/*	status += (size_t)test1 % 1024;*/
/*	test6 = (int *)AlignedMalloc(16, 32);*/
/*	status += (size_t)test1 % 32;*/
	
	AlignedFree(test1);
	AlignedFree(test2);
	AlignedFree(test3);
	AlignedFree(test4);
/*	AlignedFree(test5);*/
/*	AlignedFree(test6);*/
	
	return status;
}

static void *AlignedMalloc(size_t bytes, size_t align)
{
	int add_to_align = 0;
	void *alloc = NULL;
	
	add_to_align = align - bytes;
	
	while (0 > add_to_align)
	{
		add_to_align += align;
	}
	
	alloc = malloc(bytes + add_to_align);
	if (NULL == alloc)
	{
		return NULL;
	}
	
	return alloc;
}

void AlignedFree(void *ptr)
{
	free(ptr);
	ptr = NULL;
}


