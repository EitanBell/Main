/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Matan                *
*	Date: 04/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Fixed Size Allocator   *
*				                 *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h> /* assert */
#include <stdlib.h>	/* malloc, free */

#include "fsa.h" /* Free Size Allocator */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m": \
							     "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define WORD_SIZE (sizeof(size_t))
#define IS_ALIGNED(loc) (!((size_t)(loc) & (size_t)((WORD_SIZE) - 1)))
#define ADD_PADDING(block) ((block) + ((WORD_SIZE) -  \
                                        ((block) % (WORD_SIZE))) % (WORD_SIZE))

void RunTests(void);
static int FSARequiredPoolSizeTest(void);
static int FSAInitTest(void);
static int FSAAllocateTest(void);
static int FSAFreeAndCountFreeTest(void);
static int FSAIntegrationTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n******************** TESTS BEGIN *******************\n\n");
	
	printf("FSARequiredPoolSize Test: \t%s\n", TEST_RES(FSARequiredPoolSizeTest()));
	printf("FSAInit Test: \t\t\t%s\n", TEST_RES(FSAInitTest()));
	printf("FSAAllocate Test: \t\t%s\n", TEST_RES(FSAAllocateTest()));
	printf("FSAFreeAndCountFree Test: \t%s\n", TEST_RES(FSAFreeAndCountFreeTest()));
	printf("FSAIntegration Test: \t\t%s\n", TEST_RES(FSAIntegrationTest()));

	
	printf("\n****************** TESTS FINISHED ******************\n\n");
	
}

static int FSARequiredPoolSizeTest(void)
{
	int status = 1;
	
	if (80 == FSARequiredPoolSize(4, 12))
	{
		status = 0;
	}
	
	return status;
}


static int FSAInitTest(void)
{
    int status = 2;
    void *memory_pool = NULL;
    size_t *block1 = NULL;
    size_t *block2 = NULL;
    size_t *block3 = NULL;
    size_t *block4 = NULL;
    
    fsa_t *fsa = NULL;
    memory_pool = malloc(FSARequiredPoolSize(4, 12));
    if (NULL == memory_pool)
    {
        return 1;
    }
    if (NULL == memory_pool)
    {
        return 1;
    }
    fsa = FSAInit((char *)memory_pool + 7, 4, 12);
    
    block1 = (size_t *)((char *)fsa + sizeof(fsa));
    block2 = (size_t *)((char *)block1 + ADD_PADDING(12));
    block3 = (size_t *)((char *)block2 + ADD_PADDING(12));
    block4 = (size_t *)((char *)block3 + ADD_PADDING(12));
    
    if (IS_ALIGNED(fsa) && IS_ALIGNED(block1) && IS_ALIGNED(block2)
                         && IS_ALIGNED(block2) && IS_ALIGNED(block4))
    {
        status -= 1;
    }
    
    if (*block1 == (sizeof(fsa) + ADD_PADDING(12)) 
         && *block2 == (sizeof(fsa) + 2 * ADD_PADDING(12)) 
         && *block3 == (sizeof(fsa) + 3 * ADD_PADDING(12))
         && *block4 == 0)
    {
        status -= 1;
    }

    free(memory_pool);
    return status;
}

static int FSAAllocateTest(void)
{
	size_t pool_size = 0;
	size_t num_of_blocks = 4;
	size_t block_size = 8;
	void *pool = NULL;
	fsa_t *fsa = NULL;
	void *address_arr[4] = {0};
	int status = 0;
	size_t i = 0;
	
	pool_size = FSARequiredPoolSize(num_of_blocks, block_size);
	pool = malloc(pool_size);
	if (NULL == pool)
	{
		return 1;
	}
	fsa = FSAInit(pool, num_of_blocks, block_size);
	
	for (i = 0; i < num_of_blocks; ++i)
	{
		address_arr[i] = FSAAllocate(fsa);
	}
	
	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		status += (8 != ((char *)address_arr[i + 1] - (char *)address_arr[i]));
	}
	
	status = (0 == status) ? 0 : 1;
	
	free(pool);
	return status;
}

static int FSAFreeAndCountFreeTest(void)
{
	size_t pool_size = 0;
	size_t num_of_blocks = 4;
	size_t block_size = 12;
	void *pool = NULL;
	fsa_t *fsa = NULL;
	void *address_arr[4] = {0};
	int status = 0;
	size_t i = 0;
	
	pool_size = FSARequiredPoolSize(num_of_blocks, block_size);
	pool = malloc(pool_size);
	if (NULL == pool)
	{
		return 1;
	}
	
	fsa = FSAInit(pool, num_of_blocks, block_size);
	
	for (i = 0; i < num_of_blocks; ++i)
	{
		address_arr[i] = FSAAllocate(fsa);
	}
	
	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		status += (16 != ((char *)address_arr[i + 1] - (char *)address_arr[i]));
	}
		
	for (i = 0; i < num_of_blocks; ++i)
	{
		FSAFree(fsa ,address_arr[num_of_blocks - i - 1]);
		status += ((i + 1) != FSACountFree(fsa)); 
	}
	
	status = (0 == status) ? 0 : 1;

	free(pool);
	return status;
}

static int FSAIntegrationTest(void)
{
	size_t pool_size = 0;
	size_t num_of_blocks = 4;
	size_t block_size = 12;
	void *pool = NULL;
	fsa_t *fsa = NULL;
	void *address_arr[4] = {0};
	int status = 0;
	size_t i = 0;
	
	pool_size = FSARequiredPoolSize(num_of_blocks, block_size);
	pool = malloc(pool_size);
	if (NULL == pool)
	{
		return 1;
	}
	fsa = FSAInit(pool, num_of_blocks, block_size);
	
	for (i = 0; i < num_of_blocks; ++i)
	{
		address_arr[i] = FSAAllocate(fsa);
	}
	
	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		status += (16 != ((char *)address_arr[i + 1] - (char *)address_arr[i]));
	}
		
	status += !(NULL == FSAAllocate(fsa));
	FSAFree(fsa ,address_arr[3]);
	status += !(1 == FSACountFree(fsa));
	
	address_arr[3] = FSAAllocate(fsa);
	status += !(0 == FSACountFree(fsa));
	status += !(NULL == FSAAllocate(fsa));
	
	for (i = 0; i < num_of_blocks; ++i)
	{
		FSAFree(fsa ,address_arr[num_of_blocks - i - 1]);
		status += ((i + 1) != FSACountFree(fsa)); 
	}
	
	status = (0 == status) ? 0 : 1;
	
	free(pool);
	return status;
}





