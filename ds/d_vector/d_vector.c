/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Ariel B.    	       *
*	Date: 05/12/22	                 *
*	Valgrind & GDB passed           *
*	Subject: Dynamic Vector	       *
*				                 *
**************************************/

#include <stdlib.h> /* malloc, realloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "d_vector.h" /* my_vector */

#define GROWTH_FACTOR (2)
#define SHRINK_LIMIT (0.25)

struct vector
{
	size_t capacity;
	size_t size;
	size_t element_size;
	char *buffer;
};

vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	vector_t *p_vector = (vector_t *)malloc(sizeof(vector_t) +
		                element_size * capacity);
	if (NULL == p_vector)
	{
		return NULL;
	}
	p_vector->capacity = capacity;
	p_vector->size = 0;
	p_vector->element_size = element_size;
	p_vector->buffer = (char *)p_vector + sizeof(vector_t);
	
	return p_vector;
}	

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	
	free(vector);
	vector = NULL;
}

vector_t *VectorPushBack(vector_t *vector, const void *element)
{
	char *last_elem_in_buffer = NULL;

	assert(NULL != vector);
	assert(NULL != element);
	
	last_elem_in_buffer = vector->buffer + 
					  sizeof(char) * vector->size * vector->element_size;
	
	memcpy((void *)last_elem_in_buffer, element ,vector->element_size);
	
	++(vector->size);
	
	if (vector->size == vector->capacity)
	{
		vector = VectorResize(vector, GROWTH_FACTOR * vector->capacity);
	}
	return vector;
}

vector_t *VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);
	
	--(vector->size);
	
	if ((0 != vector->size) && vector->size <= SHRINK_LIMIT * vector->capacity)
	{
		vector = VectorResize(vector, vector->capacity / GROWTH_FACTOR);
	}
	return vector;
	
}

void *VectorGetAccessElement(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	
	return ((vector->buffer) + (sizeof(char) * vector->element_size * index));
}

vector_t *VectorResize(vector_t *vector, size_t new_capacity)
{

	vector_t *tmp = NULL;
	size_t new_size = 0;

	assert(NULL != vector);

	new_size = sizeof(vector_t) + (new_capacity * vector->element_size);

	tmp = (vector_t*)realloc(vector, new_size);
	if (NULL == tmp)
	{
		return NULL;
	}

	vector = tmp;
	vector->capacity = new_capacity;
	vector->buffer = (char *)(vector) + sizeof(vector_t);
	return vector;
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->capacity;
}
size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->size;
}











