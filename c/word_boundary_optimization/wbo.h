/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:                    *
*	Date: 23/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: word boudery opti   *
*				     *
**************************************/

#ifndef __WBO_H__
#define __WBO_H__

#include <stdio.h> /* print */
#include <string.h> /* strcmp */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#define NULL_TER_SIZE (1)

enum operate_func 
{
	FAIL = 1,
	SUCCESS = 0
};

void *Memset(void *str, int c, size_t n);
void *Memcpy(void *dest, const void *src, size_t n);
void *Memmove(void *dest, const void *src, size_t n);


#endif 		/* __WBO_H__ */
