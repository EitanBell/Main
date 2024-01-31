/****************************************
*				   			     *
*	Developer: Eitan Bellaiche         *	 
*	Reviewer:  Yuval                   *
*	Date: 08/01/23	                    *
*	Valgrind & gdb passed   	          *
*	Subject: variable Size Allocator   *
*				                    *
*****************************************/

#include <assert.h> 	/* assert */
#include <stddef.h> 	/* size_t */
#include <limits.h>		/* CHAR_BIT */

#include "vsa.h" /* variable Size Allocator */

struct vsa
{
	size_t vsa_size;
};

typedef struct block
{
	size_t block_size;
	#ifndef NDEBUG
		vsa_t *vsa;
    	#endif
}block_t;

enum boolean
{
	FALSE = 0,
	TRUE
};

#define WORDSIZE ((__WORDSIZE)/(CHAR_BIT))
#define MIN_POOL_SIZE (WORDSIZE * WORDSIZE)

#define END_OF_BLOCKS(vsa) (((char *)vsa) + (vsa->vsa_size))
#define POOL_START(vsa) (((char *)vsa) + (sizeof(vsa_t)))

#define FREE_BLOCK_SIZE(block) (((block >> 1) << 1))
#define ALLIGNMENT(block_size) ((WORDSIZE - (block_size % WORDSIZE)) % WORDSIZE)
#define SIZE_ALIGN(size) (size % WORDSIZE)
#define IS_EQUAL_ADDRESS(add1, add2) ((char *)add1 == (char *)add2)

static void *AlignToWordSize(void *memory_pool);
static int IsFreeBlock(block_t *block);
static void SetToFreeBlock(block_t *block);
static void SetToAllocatedBlock(block_t *block);
static void MergeFreeBlocks(block_t *block1, block_t *block2);
static void AllocAndUpdateBlocks(block_t *block, size_t new_size, vsa_t *vsa);			
					     
static block_t *NextBlock(block_t *block);
static int IsBlockInVSA(block_t *block);
int NotEndAndFree(block_t *block_runner, vsa_t *vsa);

/*-------------------------------- VSAInit -----------------------------------*/

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	vsa_t *new_vsa = NULL;
	block_t *new_block = NULL;
	size_t vsa_ALLIGNMENT_size = 0;
	
	assert(memory_pool);
	assert(MIN_POOL_SIZE <= pool_size);
	
	new_vsa = (vsa_t *)AlignToWordSize(memory_pool);	
	
	new_block = (block_t *)((char *)new_vsa + sizeof(vsa_t));
	vsa_ALLIGNMENT_size = (char *)new_vsa - (char *)memory_pool;
	
	pool_size -= vsa_ALLIGNMENT_size;
	pool_size -= SIZE_ALIGN(pool_size);
			    
	new_vsa->vsa_size = pool_size;				    
	
	new_block->block_size = pool_size - sizeof(vsa_t) - sizeof(block_t);
	
	#ifndef NDEBUG
		new_block->vsa = new_vsa;
     #endif
	
	return new_vsa;
}

/*------------------------------ VSAAllocate ---------------------------------*/

void *VSAAllocate(vsa_t *vsa, size_t block_size)
{
	block_t *block_runner = NULL;
	block_t *alloc_block = NULL;
	block_t *block_runner_next = NULL;
	
	assert(vsa);
	
	block_size += ALLIGNMENT(block_size);

	block_runner = (block_t *)((char *)vsa + sizeof(vsa_t));
	if (NULL == block_runner)
	{
		return NULL;
	}
	
		
	while ((char *)block_runner < END_OF_BLOCKS(vsa) && (NULL == alloc_block))
	{
		if ((IsFreeBlock(block_runner)) &&
		    (block_runner->block_size >= block_size))
		{
			AllocAndUpdateBlocks(block_runner, block_size, vsa);
			
			alloc_block = (void *)((char *)block_runner + sizeof(block_t));
		}
		
		if (NULL == NextBlock(block_runner))
		{
			return NULL;
		}
		block_runner_next = NextBlock(block_runner);
		
		if (NotEndAndFree(block_runner, vsa))
		{
			MergeFreeBlocks(block_runner, block_runner_next);
		}
		
		else
		{
			block_runner = NextBlock(block_runner);
			block_runner_next = NextBlock(block_runner_next);
		}
	}

	return (void *)alloc_block;
}

/*-------------------------------- VSAFree -----------------------------------*/

void VSAFree(void *block)
{
	block_t *block_to_free = NULL;
	
	assert(block);
	
	block_to_free = (block_t *)((char *)block - sizeof(block_t));
	assert(IsBlockInVSA(block_to_free));

	SetToFreeBlock(block_to_free);
}

/*------------------------- VSAGetMaxFreeBlockSize ---------------------------*/

size_t VSAGetMaxFreeBlockSize(vsa_t *vsa)
{
	block_t *block_runner = NULL;
	block_t *block_runner_next = NULL;
	size_t max_size = 0;
	
	assert(vsa);
	
	block_runner = (block_t *)POOL_START(vsa);
	
	while ((char *)block_runner < END_OF_BLOCKS(vsa))
	{
		block_runner_next = NextBlock(block_runner);
		
		if (NotEndAndFree(block_runner, vsa))
		{
			MergeFreeBlocks(block_runner, block_runner_next);
		}
		
		else if (max_size < block_runner->block_size &&
			    IsFreeBlock(block_runner))
		{
			max_size = block_runner->block_size;
		}
		else
		{
			block_runner = NextBlock(block_runner);
		}	
	}

	return max_size;
}

/*----------------------------- HELPER FUNCTION ------------------------------*/

/*-------------------------- AllocAndUpdateBlocks ----------------------------*/

static void AllocAndUpdateBlocks(block_t *block, size_t new_size, vsa_t *vsa)
{
	block_t *right_block = NULL;
	
	assert(block);
	
	if (block->block_size > new_size + sizeof(block_t))
	{
		right_block = (block_t *)((char *)block + sizeof(block_t) +
													     new_size);
		right_block->block_size = block->block_size - (new_size + 
												   sizeof(block_t));
		#ifndef NDEBUG
			right_block->vsa = vsa;
		#endif
		
		block->block_size = new_size;	
	}
	
	#ifndef NDEBUG
		block->vsa = vsa;
	#endif
	
	SetToAllocatedBlock(block);
}

/*---------------------------- AlignToWordSize -------------------------------*/

static void *AlignToWordSize(void *memory_pool)
{
	size_t ALLIGNMENT_size = 0;

	assert(memory_pool);
	
	ALLIGNMENT_size = ALLIGNMENT((size_t )memory_pool);	
	memory_pool = (char *)memory_pool + ALLIGNMENT_size;
	
	return (void *)memory_pool;
}

/*------------------------------ IsFreeBlock ---------------------------------*/

static int IsFreeBlock(block_t *block)
{
	assert(block);
		
	return !(block->block_size & 1);
}

/*-------------------------- SetToAllocatedBlock -----------------------------*/

static void SetToAllocatedBlock(block_t *block)
{
	assert(block);
	
	block->block_size |= 1;
}

/*----------------------------- SetToFreeBlock -------------------------------*/

static void SetToFreeBlock(block_t *block)
{
	assert(block);
	
	block->block_size >>= 1;
	block->block_size <<= 1;
}

/*---------------------------- MergeFreeBlocks -------------------------------*/

static void MergeFreeBlocks(block_t *block1, block_t *block2)
{
	assert(block1);
	assert(block2);
	
	block1->block_size += sizeof(block_t) + block2->block_size;
}

/*------------------------------- NextBlock ----------------------------------*/

static block_t *NextBlock(block_t *block)
{
	block_t *NextBlock = NULL;

	assert(block);
	
	NextBlock = (block_t *)((char *)block + sizeof(block_t) +
									    FREE_BLOCK_SIZE(block->block_size));
	return NextBlock;
}

#ifndef NDEBUG
/*----------------------------- IsBlockInVSA ---------------------------------*/

	static int IsBlockInVSA(block_t *block)
	{	
		int status = FALSE;

		assert(block);
		
		if (((char *)block >= (char *)block->vsa) &&
			  	  (char *)block < ((char *)END_OF_BLOCKS(block->vsa)))
		{
			status = TRUE;
		}
		
		return status;
	}
#endif

/*----------------------------- NotEndAndFree --------------------------------*/
int NotEndAndFree(block_t *block_runner, vsa_t *vsa)
{
	block_t *block_runner_next = NULL;
	
	assert(vsa);
	assert(block_runner);
	
	block_runner_next = NextBlock(block_runner);
	
	return (((char *)block_runner_next < END_OF_BLOCKS(vsa) && 
		    IsFreeBlock(block_runner) &&
		    IsFreeBlock(block_runner_next)));
}



