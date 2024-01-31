/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:      	            *
*	Date: 07/12/22	                 *
*	     				       *
*	Subject: c_buff	 		  *
*				                 *
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <stdio.h>

#include "c_buff.h" /*Circular buffer API*/

#define OFFSET(type,member) ((size_t)&((type *)0)->member)
#define EOBUFF(cbuff) (cbuff->data + cbuff->capacity)
#define BYTES_TO_END(place) (EOBUFF(cbuff) - place)

struct c_buff 
{
	size_t capacity;
	size_t size;
	char *p_read;
	char data[1];
};

static char *WriteLocation(c_buff_t *cbuff);
static void UpdateReadVariables(c_buff_t *cbuff, void **dest, size_t *count,
										      ssize_t *written_count);
static void UpdateWriteVariables(c_buff_t *cbuff, const void **src_runner,
					size_t *count, ssize_t *written_count, char **p_write);
/*------------------------------CbuffCreate-----------------------------------*/

c_buff_t *CbuffCreate(size_t capacity)
{
	c_buff_t *buffer = NULL;
	
	buffer = (c_buff_t *)malloc(sizeof(c_buff_t) + sizeof(char) * capacity);
	if (NULL == buffer)
	{
		return NULL;
	}
	
	buffer->capacity = capacity;
	buffer->size = 0;
	buffer->p_read = NULL;
	buffer->p_read = (char *)buffer + OFFSET(c_buff_t, data);

	return buffer;
}

/*------------------------------CbuffDestroy----------------------------------*/

void CbuffDestroy(c_buff_t *cbuff)
{
	assert(cbuff);
	
	free(cbuff);
	cbuff = NULL;
}

/*------------------------------CbuffCapacity---------------------------------*/

size_t CbuffCapacity(const c_buff_t *cbuff)
{
	assert(cbuff);
	
	return cbuff->capacity;
}

/*----------------------------CbuffGetFreeSpace-------------------------------*/

size_t CbuffGetFreeSpace(const c_buff_t *cbuff)
{
	assert(cbuff);
	
	return (cbuff->capacity - cbuff->size);
}

/*-------------------------------CbuffIsEmpty---------------------------------*/

int CbuffIsEmpty(const c_buff_t *cbuff)
{
	assert(cbuff);
	
	return (0 == cbuff->size);
}

/*---------------------------------CbuffWrite---------------------------------*/

ssize_t CbuffWrite(c_buff_t *cbuff, const void *src, size_t count)
{
	char *write = NULL;
	ssize_t written_count = 0;
	
	assert(NULL != cbuff);
	assert(NULL != src);
	
	write = WriteLocation(cbuff);
		
	if (count > CbuffGetFreeSpace(cbuff))
	{
		count = CbuffGetFreeSpace(cbuff);
	}
	
	if ((write + count) > EOBUFF(cbuff))
	{
		memcpy(write, src, BYTES_TO_END(write));
		UpdateWriteVariables(cbuff, &src, &count, &written_count, &write);
	}
	
	memcpy(write, src, count);
	cbuff->size += count;
	written_count += count;
	
	return written_count;
}


/*---------------------------------CbuffRead---------------------------------*/

ssize_t CbuffRead(c_buff_t *cbuff, void *dest, size_t count)
{
	ssize_t written_count = 0;
	
	assert(NULL != cbuff);
	assert(NULL != dest);
	
	if (count > cbuff->size)
	{
		count = cbuff->size;
	}

	if ((cbuff->p_read + count) > EOBUFF(cbuff))
	{
		memcpy(dest, cbuff->p_read, BYTES_TO_END(cbuff->p_read));
		UpdateReadVariables(cbuff, &dest, &count, &written_count);
	}
	
	memcpy(dest, cbuff->p_read, count);
	cbuff->p_read += count;
	cbuff->size -= count;
	written_count += count;
	
	return written_count;
}



/*----------------------------- SUPPORT FUNCTIONS  ---------------------------*/

/*---------------------------------WriteLocation--------------------------------*/

static char *WriteLocation(c_buff_t *cbuff)
{
	assert(NULL != cbuff);
	
	return ((cbuff->data) + 
			(cbuff->p_read - cbuff->data + cbuff->size) % (cbuff->capacity));
}


/*---------------------------UpdateWriteVariables-----------------------------*/

static void UpdateWriteVariables(c_buff_t *cbuff, const void **src_runner,
					size_t *count, ssize_t *written_count, char **p_write)
{
	size_t bytes_to_end = 0;
	
	assert(NULL != cbuff);
	assert(NULL != src_runner);
	assert(NULL != count);
	assert(NULL != p_write);
	assert(NULL != written_count);
	
	bytes_to_end = BYTES_TO_END(*p_write);
	
	*count -= bytes_to_end;
	cbuff->size += bytes_to_end;
	*written_count += bytes_to_end;
	*(char *)src_runner += bytes_to_end;
	*p_write = cbuff->data;	
}


/*---------------------------UpdateReadVariables-----------------------------*/

static void UpdateReadVariables(c_buff_t *cbuff, void **dest, size_t *count,
							  			       ssize_t *written_count)
{
	size_t bytes_to_end = 0;
	
	assert(NULL != cbuff);
	assert(NULL != dest);
	assert(NULL != count);
	assert(NULL != written_count);
	
	bytes_to_end = BYTES_TO_END(cbuff->p_read);
	
	*count -= bytes_to_end;
	cbuff->size -= bytes_to_end;
	*written_count += bytes_to_end;
	*(char **)dest += bytes_to_end;
	cbuff->p_read = cbuff->data;
}













