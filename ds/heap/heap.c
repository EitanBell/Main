/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Matan	              *
*	Date: 06/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Heap  	  				  *
*				                	  *
**************************************/

#include <assert.h>  /* assert */
#include <stdlib.h> /* malloc, free */

#include "heap.h" /* HEAP API */
#include "d_vector.h"	/* d_vector API */

#define MIN_CAPACITY (64)

enum bool_status
{
	FALSE = 0,
	TRUE
};

enum status
{
	SUCCESS = 0,
	FAIL
} status_t;

struct heap
{
	vector_t *vector;
	heap_cmp_t cmp;
	void *param;
};

/*------------- Helper Funcs -------------*/
static heap_t *InitHeap(heap_t *heap, heap_cmp_t cmp, void *param);
static void Swap(void *elem1, void *elem2);
static size_t ParentIndex(size_t index);
static size_t LeftIndex(size_t index);
static size_t RightIndex(size_t index);
static void *GetElem(heap_t *heap, size_t index);
static void HeapifyUp(heap_t *heap);
static void HeapifyDown(heap_t *heap, size_t index);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*-------------------------------- HeapCreate --------------------------------*/

heap_t *HeapCreate(heap_cmp_t cmp, void *param)
{
	heap_t *new_heap = NULL;
	
	assert(NULL != cmp);
	
	new_heap = (heap_t *)malloc(sizeof(heap_t));
	if (NULL == new_heap)
	{
		return NULL;
	}
	
	new_heap = InitHeap(new_heap, cmp, param);
	
	return new_heap;
}

/*-------------------------------- HeapDestroy -------------------------------*/

void HeapDestroy(heap_t *heap)
{
	assert(NULL != heap);
	
	VectorDestroy(heap->vector);
	
	free(heap);
	heap = NULL;
}

/*---------------------------------- HeapPush --------------------------------*/

int HeapPush(heap_t *heap, void *data)
{
	vector_t *temp_vec = NULL;
	
	assert(NULL != heap);
	
	temp_vec = VectorPushBack(heap->vector, &data);
	if (NULL == temp_vec)
	{
		return FAIL;
	}
	
	heap->vector = temp_vec;
	
	HeapifyUp(heap);  
	
	return SUCCESS;
}

/*----------------------------------- HeapPop --------------------------------*/

void *HeapPop(heap_t *heap)
{
	vector_t *temp_vec = NULL;
	void *start_elem = NULL;
	void *end_elem = NULL;
	void *pop_elem = NULL;
	
	assert(NULL != heap);
	assert(0 < HeapSize(heap));
	
	start_elem = GetElem(heap, 0);
	end_elem = GetElem(heap, HeapSize(heap) - 1);
	pop_elem = HeapPeek(heap);
	
	Swap(start_elem, end_elem);
	
	temp_vec = VectorPopBack(heap->vector);
	if (NULL == temp_vec)
	{
		return NULL;
	}
	
	heap->vector = temp_vec;
	
	HeapifyDown(heap, 0);	
	
	return pop_elem;
}

/*---------------------------------- HeapPeek --------------------------------*/

void *HeapPeek(const heap_t *heap)
{
	assert(NULL != heap);
	assert(0 < HeapSize(heap));
	
	return (*(void **)GetElem((heap_t *)heap, 0));
}

/*------------------------------- HeapIsEmpty --------------------------------*/

int HeapIsEmpty(const heap_t *heap)
{
	assert(NULL != heap);
	
	return (0 == VectorSize(heap->vector));
}

/*---------------------------------- HeapSize --------------------------------*/

size_t HeapSize(const heap_t *heap)
{
	assert(NULL != heap);
	
	return VectorSize(heap->vector);
}

/*-------------------------------- HeapRemove --------------------------------*/

void *HeapRemove(heap_t *heap, void *data, heap_is_match_t is_match,
																	void *param)
{
	vector_t *temp_vec = NULL;
	void *rem_data = NULL;
	int is_found = 0;
	size_t i = 0;
	void *last_elem = 0;
	
	assert(NULL != heap);
	assert(NULL != is_match);
	(void)param;
	
	for (i = 0; (!is_found) && (HeapSize(heap) > i); ++i)
	{
		is_found = is_match(*(void **)GetElem(heap, i), data);
	}
	
	if (TRUE == is_found)
	{
		last_elem = GetElem(heap, VectorSize(heap->vector) - 1); 
		rem_data = *(void **)GetElem(heap, i - 1);
		Swap(GetElem(heap, i - 1), last_elem);
	
		temp_vec = VectorPopBack(heap->vector);
		if (NULL != temp_vec)
		{
			heap->vector = temp_vec;
			HeapifyDown(heap, i - 1);
		}
	}
	
	return rem_data;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*--------------------------------- InitHeap ---------------------------------*/

static heap_t *InitHeap(heap_t *heap, heap_cmp_t cmp, void *param)
{
	assert(NULL != heap);
	assert(NULL != cmp);
	
	heap->vector = VectorCreate(MIN_CAPACITY, sizeof(size_t));
	if (NULL == heap->vector)
	{
		free(heap);
		heap = NULL;
		
		return NULL;
	}
	
	heap->cmp = cmp;
	heap->param = param;
	
	return heap;
}


/*--------------------------------- HeapifyUp --------------------------------*/

static void HeapifyUp(heap_t *heap)
{
	size_t index = 0;
	
	assert(NULL != heap);
	
	index = HeapSize(heap) - 1;
	
	while ((0 < index) && 
			(0 < heap->cmp(*(void **)(GetElem(heap, ParentIndex(index))),
						   *(void **)(GetElem(heap, index)), heap->param)))
	{
		Swap(GetElem(heap, index), GetElem(heap, ParentIndex(index)));
		index = ParentIndex(index);
	}
}
/*------------------------------- HeapifyDown --------------------------------*/

static void HeapifyDown(heap_t *heap, size_t index)
{
	size_t l_child_index = LeftIndex(index);
	size_t r_child_index = RightIndex(index);
	size_t parent_index = index;
	size_t heap_size = HeapSize(heap);

	assert(NULL != heap);
	
	if ((l_child_index < heap_size) &&
	   (0 < heap->cmp(*(void **)GetElem(heap, index), 
       *(void **)GetElem(heap, l_child_index), heap->param)))
	{
		parent_index = l_child_index;
	}
	
	if ((r_child_index < heap_size) && 
		(0 < heap->cmp(*(void **)GetElem(heap, parent_index), 
					   *(void **)GetElem(heap, r_child_index), heap->param)))
	{
		parent_index = r_child_index;
	}
	
	if (parent_index != index)
	{
		Swap(GetElem(heap, index), GetElem(heap, parent_index));	
		HeapifyDown(heap, parent_index);
	}	
}


/*----------------------------------- Swap -----------------------------------*/

static void Swap(void *elem1, void *elem2)
{
	size_t *elem1_p = NULL;
	size_t *elem2_p = NULL;
	
	assert(NULL != elem1);
	assert(NULL != elem2);
	
	elem1_p = (size_t *)elem1;
	elem2_p = (size_t *)elem2;
	
	*elem1_p = *elem1_p ^ *elem2_p;
	*elem2_p = *elem1_p ^ *elem2_p;
	*elem1_p = *elem1_p ^ *elem2_p;
}

/*-------------------------------- ParentIndex -------------------------------*/

static size_t ParentIndex(size_t index)
{
	assert(0 < index);
	
	return ((index - 1) / 2);
}

/*--------------------------------- LeftIndex --------------------------------*/

static size_t LeftIndex(size_t index)
{
	return (2 * index + 1);
}

/*-------------------------------- RightIndex --------------------------------*/

static size_t RightIndex(size_t index)
{
	return (2 * index + 2);
}

/*--------------------------------- GetElem ----------------------------------*/

static void *GetElem(heap_t *heap, size_t index)
{
	assert(NULL != heap);
	
	return (VectorGetAccessElement(heap->vector, index));
}


