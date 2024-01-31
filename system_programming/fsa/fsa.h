/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Matan                *
*	Date: 04/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Fixed Size Allocator   *
*				                 *
**************************************/

/*------------------------------------------------------------------------------
FIXED SIZE ALLOCATOR:
	Fixed Size Allocator is a tool to manage memory. The FSA get memory pool 
	and dvide the pool to fixed size block that can be allocat and freed.
	The main functions are the FSAAllocate and FSAFree.
	To calculate the size of the pool to sent to FSAInit use
	FSARequiredPoolSize.
	The FSAInit is eager initializtion and therefore in O(n) time complexity. 


MAIN FUNCTIONALITY:
	RequiredPoolSize, Init, Allocate, Free, CountFree.
------------------------------------------------------------------------------*/

#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h> /* size_t */

/*-----------------------------------------------------------------------------
Description: 
	fsa_t is the handel for the Fixed Size Allocator.
	
	NOTE! For the first usage, The user have to call the function
     FSARequiredPoolSize before initialization. otherwise, undifined behavior
     may happan.
-----------------------------------------------------------------------------*/
typedef struct fsa fsa_t;

/*-----------------------------------------------------------------------------
Description: 
	Calculate the minimum size of the pool that needed to FSAInit to support
	num_of_blocks and block_size.
	
Parameters:
	num_of_blocks -	the number of block in the FSA.
	block_size - 	the size of each block in the FSA.
	
Return Value:
	The minimum size of the pool in bytes.
	
Comments:
	0 as num_of_blocks or block_size will lead to undefine behavior.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
size_t FSARequiredPoolSize(size_t num_of_blocks, size_t block_size);

/*-----------------------------------------------------------------------------
Description: 
	Init the memory_pool for first usage. All blocks are free to use.
	
	NOTE! The user have to call the function FSARequiredPoolSize before
    initialization. otherwise, undifined behavior may happan.
Parameters:
	memory_pool - 	pointer to memory area in the size that recived from 
	FSARequiredPoolSize.
	num_of_blocks - the number of block in the FSA.
	block_size - 	the size of each block in the FSA.
	
Return Value:
	pointer to the handel of the FSA. this is not nessecrily be the same
	addressas memory_pool.
	
Comments:
	memory_pool that doesn't calculate with FSARequiredPoolSize will lead to
	undefine behavior.
	num_of_blocks or block_size that are not the same as the num_of_blocks and
	block_size that sent to FSARequiredPoolSize will lead to undefine behavior.
	Sending a memory_pool that already was FSAInit will free all the blocks in
	the FSA, diffrent num_of_blocks or block_size will lead to undefine 
	behavior.
	
Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
fsa_t *FSAInit(void *memory_pool, size_t num_of_blocks, size_t block_size);

/*-----------------------------------------------------------------------------
Description: 
	Allocate block of memory from fsa in the block_size that sent to FSAInit. 
	
Parameters:
	fsa -	Pointer to the FSA to allocet block from.
	
Return Value:
	Pointer to the new allocated block. NULL if there is no more free block.
	 
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void *FSAAllocate(fsa_t *fsa);

/*-----------------------------------------------------------------------------
Description:
	Free block of memory that was previously FSAAllocate from the fsa. 
	
Parameters:
	fsa -	The FSA that the block is belong to.
	block -	The block to free.
	
Return Value:
	None.
	
Comments:
	Sending a diffrent FSA than the FSA that the block was allocated from will
	lead to undefine behavior.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void FSAFree(fsa_t *fsa, void *block);

/*-----------------------------------------------------------------------------
Description: 
	Count the free blocks that remain in the FSA.
	
	NOTE! Calling FSAFree for freed block, may lead to an undifined behavior.
Parameters:
	fsa - FSA to count.
	
Return Value:
	The number of the free block in fsa.
	
Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
size_t FSACountFree(const fsa_t *fsa);

#endif /* __FSA_H__ */

