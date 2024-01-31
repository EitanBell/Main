/*************************************
*				   				     *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:                        *
*	Date: 02/01/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: priority queue   	  	 *
*				               	     *
**************************************/

#include <stdlib.h> 	/* malloc, free */
#include <assert.h> 	/* assert */
#include <stdio.h>		/* NULL, size_t */

#include "priority_queue.h" 	/* priority_queue */
#include "sort_list.h" /* container to priority_queue*/

struct ilrd_pq
{
	sort_list_t *list;
};

/*---------------------------- SortListCreate --------------------------------*/

ilrd_pq_t *PQCreate(pq_cmp_t cmp)
{
	ilrd_pq_t *new_pq = NULL;
	
	assert(cmp);
	
	new_pq = (ilrd_pq_t *)malloc(sizeof(ilrd_pq_t));
	if (NULL == new_pq)
	{
		return NULL;
	}
	
	new_pq->list = SortListCreate(cmp);
	if (NULL == new_pq->list)
	{
		free(new_pq);
		new_pq = NULL;

		return NULL;
	}
	
	return new_pq;
}

/*------------------------------ PQDestroy -----------------------------------*/

void PQDestroy(ilrd_pq_t *pq)
{
	assert(pq);
	SortListDestroy(pq->list);

	free(pq);
	pq = NULL;
}

/*------------------------------ PQEnqueue -----------------------------------*/

int PQEnqueue(ilrd_pq_t *pq, void *data)
{
	sort_list_iter_t s_iter = {0};
	sort_list_iter_t s_eol = {0};
	
	assert(pq);
	
	s_eol = SortListEnd(pq->list);
	s_iter = SortListInsert(pq->list, data);

	return (SortListIterIsEqual(s_iter, s_eol));
}

/*------------------------------ PQDequeue -----------------------------------*/

void *PQDequeue(ilrd_pq_t *pq)
{
	assert(pq);
	assert(!PQIsEmpty(pq));
	
	return (SortListPopFront(pq->list));
}

/*-------------------------------- PQPeek ------------------------------------*/

void *PQPeek(const ilrd_pq_t *pq)
{
	assert(pq);
	assert(!PQIsEmpty(pq));
	
	return (SortListGetVal(SortListBegin(pq->list)));
}

/*-------------------------------- PQSize ------------------------------------*/

size_t PQSize(const ilrd_pq_t *pq)
{
	assert(pq);
	
	return (SortListCount(pq->list));
}

/*------------------------------ PQIsEmpty -----------------------------------*/

int PQIsEmpty(const ilrd_pq_t *pq)
{
	assert(pq);
	
	return (SortListIsEmpty(pq->list));
}

/*-------------------------------- PQClear -----------------------------------*/

void PQClear(ilrd_pq_t *pq)
{
	assert(pq);
	
	while (!SortListIsEmpty(pq->list))
	{
		SortListPopFront(pq->list);
	}
}

/*-------------------------------- PQErase -----------------------------------*/

void *PQErase(ilrd_pq_t *pq, void *data, pq_is_match_t is_match)
{
	sort_list_iter_t to_erase = {0};
	void *to_erase_data = NULL;
	
	assert(pq);
	assert(is_match);
	
	to_erase = SortListFindIf(SortListBegin(pq->list), SortListEnd(pq->list),
												    is_match, data);
	if (!SortListIterIsEqual(to_erase, SortListEnd(pq->list)))
	{
		to_erase_data = SortListGetVal(to_erase);
		SortListRemove(to_erase);
	}
	
	return to_erase_data;
}




























