/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:      	            *
*	Date: 07/12/22	                 *
*	     				       *
*	Subject: c_buff	 		  *
*				                 *
**************************************/

#ifndef __C_BUFF__
#define __C_BUFF__
#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* memcpy */
#include <sys/types.h> /* ssize_t */
typedef struct c_buff c_buff_t;

/**
 * The function creates a new circular buffer with the given capacity. 
 * @param capacity: the maximal size the buffer will be able to get to.
 * @return a pointer to a new circular buffer.
 * @time_complexity O(1)
 * @space_complexity O(n)
 */
c_buff_t *CbuffCreate(size_t capacity);

/**
 * The function frees the allocated memory of the buffer.
 * @param cbuff: the buffer to free.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void CbuffDestroy(c_buff_t *cbuff);

/**
 * The function adds count bytes from src to the end of cbuff. 
 * @param cbuff: the buffer to write into.
 * @param src: the source to read from.
 * @param count: the number of bytes to write.
 * @return the number of bytes that were written.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
ssize_t CbuffWrite(c_buff_t *cbuff, const void *src, size_t count); 

/**
 * The function adds count bytes from buffer to src, the counts bytes will be 
 * excluded from buffer. 
 * @param cbuff: the buffer to read from.
 * @param src: the source to write to.
 * @param count: the number of bytes to write.
 * @return the number of bytes that were written.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
ssize_t CbuffRead(c_buff_t *cbuff, void *dest, size_t count);

/**
 * The function returns the free space in the buffer.
 * @param cbuff: the buffer to check free space in.
 * @return The number of free bytes in the buffer
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
size_t CbuffGetFreeSpace(const c_buff_t *cbuff);

/**
 * The function checks if the buffer is empty.
 * @param cbuff: the buffer to check.
 * @return 1 if the buffer is empty, 0 otherwise
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int CbuffIsEmpty(const c_buff_t *cbuff);

/**
 * The function gets the capacity of the buffer.
 * @param cbuff: the buffer to check.
 * @return the capacity of the buffer.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
size_t CbuffCapacity(const c_buff_t *cbuff);

#endif  /*__C_BUFF__*/

