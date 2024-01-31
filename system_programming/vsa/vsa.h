/****************************************
*				   			     *
*	Developer: Eitan Bellaiche         *	 
*	Reviewer:  Yuval                   *
*	Date: 08/01/23	                    *
*	Valgrind & gdb passed   	          *
*	Subject: variable Size Allocator   *
*				                    *
*****************************************/

#ifndef __VSA_H__
#define __VSA_H__

/*********************************************************
General Description
-------------------
VSA (Variable-Size Aloocator) is a tool for managing a 
memory pool, using allocations in variable sizes.

Attributes
	Memory management, Variable size.
Main Functions
	Initialize the VSA before use.
	Allocate a block of given size.
	Free an allocated block.
	Get the maximal size available for allocation.
*********************************************************/

#include <stddef.h> /* size_t */

/*******************************************************************
Description
	A handler for the VSA.
*******************************************************************/
typedef struct vsa vsa_t;

/*******************************************************************
Description
	Initialize the VSA before use.
Parameters
	'memory_pool' - A pointer to the memory pool to be managed.
	'pool_size' - The size of the memory pool.
Return Value
	A handle to the VSA.
Comments
	- Part of memory is taken for the management of the pool, i.e.
	  available space for allocation is smaller than 'pool_size'
	  provided by the user.
	- Pool size must be at least 64 bytes, otherwise undefined
	  behaviour may happen.
Complexity
	Time: O(1) / Space: O(1)
*******************************************************************/
vsa_t *VSAInit(void *memory_pool, size_t pool_size);

/*******************************************************************
Description
	Allocate a block of given size.
Parameters
	'vsa' - A handle to the VSA.
	'block_size' - The size of the block to be allocated.
Return Value
	A pointer to the allocated block, or NULL if no enough space is available.

Complexity
	Time: O(n) / Space: O(1)
*******************************************************************/
void *VSAAllocate(vsa_t *vsa, size_t block_size);

/*******************************************************************
Description
	Free an allocated block.
Parameters
	'block' - A pointer to the allocated block.
Return Value
	NO RETURN VALUE.
Comments
	- Trying to free a memory space that is not allocated is 
	  undefined behaviour.
	- Trying to free a memory space that is not part of the memory 
	  pool is undefined behaviour.
Complexity
	Time: O(1) / Space: O(1)
*******************************************************************/
void VSAFree(void *block);

/*******************************************************************
Description
	Get the maximal size available for allocation.
Parameters
	'vsa' - A handle to the VSA.
Return Value
	The size of maximal available space for allocation.
Complexity
	Time: O(1) / Space: O(1)
*******************************************************************/
size_t VSAGetMaxFreeBlockSize(vsa_t *vsa);

#endif /* __VSA_H__ */

