/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 06/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Heap  	  				  *
*				                	  *
**************************************/

#include <assert.h>  	/* assert */
#include <stdlib.h> 	/* malloc, free */

#include "heap.h" 		/* Heap API */
#include "heap_pq.h"	/* Heap_pq API */

struct ilrd_pq
{
	heap_t *heap;
};

/*------------- Helper Funcs -------------*/
static int CmpHeapFunc(const void *heap_data, const void *new_data,
																   void *param);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*-------------------------------- PQCreate ----------------------------------*/

ilrd_pq_t *PQCreate(pq_cmp_t cmp)
{
	ilrd_pq_t *new_pq = NULL;
	
	assert(NULL != cmp);

	new_pq = (ilrd_pq_t *)malloc(sizeof(ilrd_pq_t));
	if(NULL == new_pq)
	{
		return NULL;
	}
	
	new_pq->heap = HeapCreate(CmpHeapFunc, *(void **)&cmp);
	if(NULL == new_pq->heap)
	{
		free(new_pq);
		new_pq = NULL;
		
		return NULL;
	}
	
	return new_pq;
}

/*------------------------------- PQDestroy ----------------------------------*/

void PQDestroy(ilrd_pq_t *pq)
{
	assert(NULL != pq);
	
	HeapDestroy(pq->heap);	
	
	free(pq);
	pq = NULL;
}

/*------------------------------- PQEnqueue ----------------------------------*/

int PQEnqueue(ilrd_pq_t *pq, void *data)
{
	assert(NULL != pq);
	
	return HeapPush(pq->heap, data);
}

/*------------------------------- PQDequeue ----------------------------------*/

void *PQDequeue(ilrd_pq_t *pq)
{
	assert(NULL != pq);

	return HeapPop(pq->heap);
}

/*------------------------------- PQIsEmpty ----------------------------------*/

int PQIsEmpty(const ilrd_pq_t *pq)
{
	assert(NULL != pq);
	
	return HeapIsEmpty(pq->heap);
}

/*-------------------------------- PQPeek ------------------------------------*/

void *PQPeek(const ilrd_pq_t *pq)
{
	assert(NULL != pq);

	return HeapPeek(pq->heap);
}

/*--------------------------------- PQSize -----------------------------------*/

size_t PQSize(const ilrd_pq_t *pq)
{
	assert(NULL != pq);

	return HeapSize(pq->heap);
}

/*-------------------------------- PQClear -----------------------------------*/

void PQClear(ilrd_pq_t *pq)
{
	assert(NULL != pq);

	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

/*-------------------------------- PQErase -----------------------------------*/

void *PQErase(ilrd_pq_t *pq, void *data, pq_is_match_t is_match)
{
	void *erased_data = NULL;

	assert(NULL != pq);
	assert(NULL != is_match);
	
	erased_data = HeapRemove(pq->heap, data, is_match, NULL);
	
	return erased_data;	 	
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------- CmpHeapFunc --------------------------------*/

static int CmpHeapFunc(const void *heap_data, const void *new_data, void *param)
{
	pq_cmp_t pq_cmp_func = NULL;

	assert(NULL != heap_data);
	assert(NULL != new_data);

	pq_cmp_func = *(pq_cmp_t *)&param;

	return pq_cmp_func(heap_data, new_data);
} 


