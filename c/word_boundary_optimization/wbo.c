/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:                    *
*	Date: 23/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: word boudery opti   *
*				     *
**************************************/

#include "wbo.h"

#define WORD_SIZE (sizeof(size_t))
#define IS_ALIGNED(ptr) !((size_t)ptr & (size_t)(WORD_SIZE - 1))

void SetWord(size_t *str, int c);

void *Memset(void *str, int c, size_t n)
{
	size_t word_of_c = 0;
	size_t *runner = NULL;

	assert(NULL != str);	
	
	runner = str;
	 
	while (!IS_ALIGNED(runner) && 0 < n)
	{
		*(char *)runner = (char)c;
		runner = (size_t *)((char *)runner + 1);	
		--n;
	}

	if (WORD_SIZE <= n)
	{
		SetWord(&word_of_c, c);
		
		while (WORD_SIZE <= n)
		{
			*runner = word_of_c;
			++runner;
			n -= WORD_SIZE;
		}
	}
	while (0 < n)
	{
		*(char *)runner = (char)c;
		runner = (size_t *)((char *)runner + 1);
		--n;
	}	
	return str;
}

void *Memcpy(void *dest, const void *src, size_t n)
{
	char *dest_by_byte = NULL;
	char *src_by_byte = NULL;
	size_t *dest_by_word = NULL;
	size_t *src_by_word = NULL;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	dest_by_word = (size_t *)dest;
	src_by_word = (size_t *)src;
	
	while (WORD_SIZE - 1 < n)
	{
		*dest_by_word = *src_by_word;
		++dest_by_word;
		++src_by_word;
		n -= WORD_SIZE;	
	}
	
	dest_by_byte = (char *)dest_by_word;
	src_by_byte = (char *)src_by_word;
	
	while (0 < n)
	{
		*dest_by_byte = *src_by_byte;
		++dest_by_byte;
		++src_by_byte;
		--n;
	}
	
	return dest;
}

void *Memmove(void *dest, const void *src, size_t n)
{
	char *dest_byte_run = NULL;
	char *src_byte_run = NULL;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	if ((char *)dest < (char *)src || (char *)dest > ((char *)src + n))
	{
		Memcpy(dest, src, n);
	}
	else
	{
		dest_byte_run = (char *)dest + n - 1;
		src_byte_run = (char *)src + n - 1;
		
		while (0 != n)
		{
			*dest_byte_run = *src_byte_run;
			--dest_byte_run;
			--src_byte_run;
			--n;
		}
	}
	return dest;	
}
	
void SetWord(size_t *str, int c)
{
	size_t i = 0;
	
	assert(NULL != str);
	
	for (i = 0; i < WORD_SIZE; ++i)
	{
		*(char *)str = c;	
	}
}












