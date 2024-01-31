/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Matan                *
*	Date: 04/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Fixed Size Allocator   *
*				                 *
**************************************/

#include <assert.h> 	/* assert */
#include <stddef.h> 	/* size_t */
#include <limits.h>		/* CHAR_BIT */

#include "fsa.h" /* Free Size Allocator */

struct fsa
{
	size_t next_offset; 
};

#define MAX_ALIGN ((__WORDSIZE)/(CHAR_BIT))
#define PADDING(block_size) ((MAX_ALIGN - (block_size % MAX_ALIGN)) % MAX_ALIGN)
#define END_OF_BLOCKS (0)

static void SetLastBlock(fsa_t * fsa, char *block_runner, size_t block_size, 
														size_t i);
static char *AlignToWordSize(char *memory_pool);
static char *SetBlock(fsa_t *fsa, size_t i, size_t block_size);

/*-------------------------- FSARequiredPoolSize -----------------------------*/

size_t FSARequiredPoolSize(size_t num_of_blocks, size_t block_size)
{
	size_t pool_size = 0;
	
	assert(num_of_blocks);
	assert(block_size);
	
	pool_size = sizeof(fsa_t) + MAX_ALIGN + 
					  	     num_of_blocks * (block_size + PADDING(block_size));
	return pool_size;  
}

/*-------------------------------- FSAInit -----------------------------------*/

fsa_t *FSAInit(void *memory_pool, size_t num_of_blocks, size_t block_size)
{
	fsa_t *new_fsa = NULL;
	char *block_runner = NULL;
	size_t i = 0;
	
	assert(memory_pool);
	assert(num_of_blocks);
	assert(block_size);

	new_fsa = (fsa_t *)(AlignToWordSize((char *)(memory_pool)));
	
	block_size += PADDING(block_size);
	
	new_fsa->next_offset = sizeof(fsa_t);
	
	for (i = 0; (num_of_blocks - 1) > i; ++i)
	{
		block_runner = SetBlock(new_fsa, i, block_size);
	}
	SetLastBlock(new_fsa, block_runner, block_size, i);
	
	return new_fsa;
}

/*------------------------------ FSAAllocate ---------------------------------*/

void *FSAAllocate(fsa_t *fsa)
{
	char *next_free_node = NULL;
	
	assert(fsa);
	
	if (END_OF_BLOCKS != fsa->next_offset)
	{
		next_free_node = (char *)fsa + fsa->next_offset;
		fsa->next_offset = *(size_t *)(next_free_node);
	}
	
	return (void *)next_free_node;
}

/*-------------------------------- FSAFree -----------------------------------*/

void FSAFree(fsa_t *fsa, void *block)
{
	size_t *temp = NULL;
	size_t temp_offset = 0;
	
	assert(NULL != fsa);
	assert(NULL != block);
	
	temp_offset = fsa->next_offset;
	temp = &temp_offset;
	
	fsa->next_offset = ((char *)block - (char *)fsa);
	*(size_t *)block = *temp; 	
}


/*----------------------------- FSACountFree ---------------------------------*/

size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	size_t next_offset = 0;
	size_t *offset_runner = NULL;

	
	assert(fsa);
	
	next_offset = fsa->next_offset;
	offset_runner = (size_t *)fsa;
	
	while (END_OF_BLOCKS != *offset_runner)
	{
		++counter;
		offset_runner = (size_t *)((char *)fsa + next_offset);
		next_offset = *offset_runner;
	}
	
	return counter;
}

/*--------------------------- HELPER FUNCTION --------------------------------*/

/*----------------------------- SetLastBlock ---------------------------------*/

static void SetLastBlock(fsa_t * fsa, char *block_runner, size_t block_size, 
														 size_t i)
{
	block_runner = (char *)fsa + fsa->next_offset + i * block_size;
	*(size_t *)block_runner = END_OF_BLOCKS;
}

/*---------------------------- AlignToWordSize -------------------------------*/

static char *AlignToWordSize(char *memory_pool)
{
	assert(memory_pool);
	
	return (memory_pool + MAX_ALIGN - ((size_t)(memory_pool) &
							       (size_t)((MAX_ALIGN) - 1)) % MAX_ALIGN);
}

/*------------------------------- SetBlock -----------------------------------*/

static char *SetBlock(fsa_t *fsa, size_t i, size_t block_size)
{
	char *block_runner = NULL;
	assert(fsa);
	
	block_runner = (char *)fsa + fsa->next_offset + i * block_size;
	*(size_t *)block_runner = (i + 1) * block_size + fsa->next_offset;
	
	return block_runner;
}


