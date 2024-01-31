/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Ariel B.    	       *
*	Date: 05/12/22	                 *
*	Valgrind & GDB passed           *
*	Subject: Dynamic Vector	       *
*				                 *
**************************************/
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>	/*size_t*/

/************************struct*****************************/

typedef struct vector vector_t;

/******************Function Declarations********************/

/*time:O(1)  / space:O(n) */
vector_t *VectorCreate(size_t capacity, size_t element_size);
/*---------------------------------------------------------------------------
Description-
The VectorCreate function receives the capacity of the vector of type vector_t 
that is to be allocated. The capacity and element size must be larger than zero
to avoid undefined behavior. 
User must deallocate any memory allocated.

Return Value-
The function returns the initialized vector if successfully allocated.
In case allocation failed, NULL is returned. 

Complexity-
time:O(1)  / space:O(1) 
---------------------------------------------------------------------------*/

void VectorDestroy(vector_t *vector);
/*---------------------------------------------------------------------------
Description-
The VectoryDestroy function free's all memory allocated for the vector, 
and can not fail. 

Complexity-
time:amortized - O(1) / space: amortized - O(n) 
---------------------------------------------------------------------------*/

vector_t *VectorPushBack(vector_t *vector, const void *element);
/*---------------------------------------------------------------------------
Description-
The VectorPushBack function inserts the value of the given argument data int 
to the end of the given ragument vector.

Return Value-
The function returns the new vector with the added value.

Complexity-
time: amortized - O(1)  / space: amortized - O(n) 
---------------------------------------------------------------------------*/

vector_t *VectorPopBack(vector_t *vector);
/*---------------------------------------------------------------------------
Description- 
	The VectorPopBack function removes the last element in the given argument 
	vector.

Return Value -
	The function returns the new vector without the removed elements. 
---------------------------------------------------------------------------

time:O(1)  / space:O(1) */
void *VectorGetAccessElement(const vector_t *vector, size_t index);
/*---------------------------------------------------------------------------
Description- 
	The VectorGetAccessElement function searches the given argument vector for 
	the index number given. 

NOTE!
	index has to be smaller than the size of the 
	vector, and equal or larger then zero to avoid undefined behavior.

Return Value - 
	The function returns the value in the given index in the given vector.

Complexity - time:O(1)  / space:O(1) 
---------------------------------------------------------------------------*/


size_t VectorSize(const vector_t *vector);
/*---------------------------------------------------------------------------
Description- 
The VectorSize function finds the number of elements currently occupying the
give argument vector.

Return Value - 
Returns the number of elements found in the vector.
---------------------------------------------------------------------------

time:O(1)  / space:O(1) */	
size_t VectorCapacity(const vector_t *vector);
/*---------------------------------------------------------------------------
Description- 
The VectorCapacity function finds the maximum number of elements the given
argument vector may contain.

Return Value - 
Returns the maximum capacity of the vector.

Complexity-
time:amortized - O(1)  / space:O(n)
---------------------------------------------------------------------------*/

vector_t *VectorResize(vector_t *vector, size_t new_capacity);
/*---------------------------------------------------------------------------
Description:
The VectorResize function changes the given argument vector's capacity to the
to the given argument new_capacity.

Return Value:
The function returns the new allocated vector with it's new capacity.

Complexity: 
time: amortized - O(1) / space: amortized - O(n)
---------------------------------------------------------------------------*/

#endif 		/* __VECTOR_H__ */




