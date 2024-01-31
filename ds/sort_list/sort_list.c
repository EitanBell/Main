/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 29/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Sorted List   	       *
*				                 *
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "sort_list.h" /* sort_list */

struct sort_list
{
	dlist_t *dlist;
	sort_list_cmp_t cmp_func;
};

static dlist_iter_t SortIterToDlistIter(sort_list_iter_t sort_iter);
static sort_list_iter_t DlistIterToSortIter(dlist_iter_t dlist_iter,
												 sort_list_t *list);
static sort_list_iter_t FindWhereToInsert(sort_list_iter_t where,
									    sort_list_t *list, void *data);
static int IterIsEnd(sort_list_t *list, sort_list_iter_t iter);


/*---------------------------- SortListCreate --------------------------------*/

sort_list_t *SortListCreate(sort_list_cmp_t cmp)
{
	sort_list_t *new_list = NULL;
	
	assert(cmp);
	
	new_list = (sort_list_t *)malloc(sizeof(sort_list_t));
	if(NULL == new_list)
	{
		return NULL;
	}
	
	new_list->dlist = DlistCreate();
	if(NULL == new_list->dlist)
	{
		free(new_list);
		new_list = NULL;
		return NULL;
	}
	
	new_list->cmp_func = cmp;
	
	return new_list;
}

/*---------------------------- SortListDestroy -------------------------------*/

void SortListDestroy(sort_list_t *list)
{
	assert(list);
		
	DlistDestroy(list->dlist);
	
	free(list);
	list = NULL;
}

/*----------------------------- SortListInsert -------------------------------*/

sort_list_iter_t SortListInsert(sort_list_t *list, void *data)
{
	sort_list_iter_t where = {0};
	
	assert(list);
	assert(data);

	where = FindWhereToInsert(where, list, data);
	
	where = DlistIterToSortIter(DlistInsertBefore(SortIterToDlistIter(where),
												  	  data), list);
	return where;
}

/*------------------------------- SortListFind -------------------------------*/

sort_list_iter_t SortListFind(const sort_list_t *list, 
 	 const sort_list_iter_t from, const sort_list_iter_t to, const void *param)
{
	sort_list_iter_t runner = {0};
	
	assert(list);
	
	runner.iter = from.iter;
	#ifndef NDEBUG
		assert(from.list == to.list);
		runner.list = from.list;
	#endif

	while (!SortListIterIsEqual(runner, to) &&
					    0 != list->cmp_func(SortListGetVal(runner), param))
	{
		runner = SortListNext(runner);
	}
	
	return runner;
}

/*------------------------------ SortListFindIf ------------------------------*/

sort_list_iter_t SortListFindIf(const sort_list_iter_t from, 
		const sort_list_iter_t to, sort_list_is_match_t is_match, void *param)
{
	sort_list_iter_t runner = {0};
	
	assert(is_match);
	#ifndef NDEBUG
		assert(from.list == to.list);
		runner.list = from.list;
	#endif

	runner.iter = DlistFind(SortIterToDlistIter(from),
 							   SortIterToDlistIter(to), is_match, param);
 							   
 	return runner;
}

/*------------------------------ SortListForEach -----------------------------*/

int SortListForEach(sort_list_iter_t from, sort_list_iter_t to, 
			        sort_list_do_action_t do_action, void *param)
{	
	assert(do_action);
	
	return (DlistForEach(SortIterToDlistIter(from), SortIterToDlistIter(to), 
												 do_action, param));
}
						
						 
/*----------------------------- SortListRemove -------------------------------*/

sort_list_iter_t SortListRemove(sort_list_iter_t iter)
{	
	assert(NULL != iter.iter);
	assert(NULL != iter.list);
	
	iter.iter = DlistRemove(SortIterToDlistIter(iter));
	
	return iter;
}

/*---------------------------- SortListIsEmpty -------------------------------*/

int SortListIsEmpty(const sort_list_t *list)
{
	return DlistIsEmpty(list->dlist);
}

/*---------------------------- SortListGetVal -------------------------------*/

void *SortListGetVal(const sort_list_iter_t iter)
{
	return DlistGetVal(SortIterToDlistIter(iter));
}

/*--------------------------- SortListIterIsEqual ----------------------------*/

int SortListIterIsEqual(const sort_list_iter_t one,
										 const sort_list_iter_t other)
{
	return DlistIterIsEqual(SortIterToDlistIter(one),
										  SortIterToDlistIter(other));
}

/*------------------------------ SortListCount -------------------------------*/

size_t SortListCount(const sort_list_t *list)
{
	assert(list);
	
	return DlistCount(list->dlist);
}

/*------------------------------ SortListBegin -------------------------------*/

sort_list_iter_t SortListBegin(const sort_list_t *list)
{
	assert(list);
	
	return DlistIterToSortIter(DlistBegin(list->dlist), (sort_list_t *)list);
}

/*------------------------------- SortListEnd --------------------------------*/

sort_list_iter_t SortListEnd(const sort_list_t *list)
{
	assert(list);
	
	return DlistIterToSortIter(DlistEnd(list->dlist), (sort_list_t *)list);
}

/*------------------------------- SortListNext -------------------------------*/

sort_list_iter_t SortListNext(sort_list_iter_t current)
{
	current.iter = DlistNext(SortIterToDlistIter(current));
	
	return current;
}

/*------------------------------- SortListPrev -------------------------------*/

sort_list_iter_t SortListPrev(sort_list_iter_t current)
{
	current.iter = DlistPrev(SortIterToDlistIter(current));
	
	return current;
}

/*----------------------------- SortListPopFront -----------------------------*/

void *SortListPopFront(sort_list_t *list)
{
	assert(list);
	
	return DlistPopFront(list->dlist);
}

/*------------------------------ SortListPopBack -----------------------------*/

void *SortListPopBack(sort_list_t *list)
{
	assert(list);
	
	return DlistPopBack(list->dlist);
} 	

/*------------------------------- SortListMerge ------------------------------*/

void SortListMerge(sort_list_t *dest, sort_list_t *src)
{
	sort_list_iter_t where = {0};
	sort_list_iter_t from = {0};
	sort_list_iter_t to = {0};
	 
	assert(dest);
	assert(src);
	assert(dest->cmp_func == src->cmp_func);

	
	where = SortListBegin(dest);
	from = SortListBegin(src);
	to = SortListNext(SortListBegin(src));
	
	while (!IterIsEnd(src, from))
	{
		while (!IterIsEnd(dest, where) && 
		          (0 < dest->cmp_func(SortListGetVal(from),
										 SortListGetVal(where))))
		{
			where = SortListNext(where);
		}
		
		if (IterIsEnd(src, to) || (0 < dest->cmp_func(SortListGetVal(to), 
									   	  	 SortListGetVal(where))))
		{
			DlistSplice(SortIterToDlistIter(from), SortIterToDlistIter(to), 
										  SortIterToDlistIter(where));
			from.iter = to.iter;
		}
		
		if (!IterIsEnd(src, to))
		{
			to = SortListNext(to);
		}

	}
}

/*------------------------------HELPER FUNCTIONS------------------------------*/

/*----------------------------- SortIterToDlistIter --------------------------*/

static dlist_iter_t SortIterToDlistIter(sort_list_iter_t sort_iter)
{
	return sort_iter.iter;
}

/*-------------------------- DlistIterToSortIter -----------------------------*/

static sort_list_iter_t DlistIterToSortIter(dlist_iter_t dlist_iter,
												  sort_list_t *list)
{
	sort_list_iter_t new_sort_iter = {0};
	
	assert(list);
	
	new_sort_iter.iter = dlist_iter;
	#ifndef NDEBUG
		new_sort_iter.list = list;
	#endif
	
	return new_sort_iter;
}

/*--------------------------- FindWhereToInsert -----------------------------*/

static sort_list_iter_t FindWhereToInsert(sort_list_iter_t where, 
									     sort_list_t *list, void *data)
{
	where = SortListBegin(list);

	if (!SortListIsEmpty(list))
	{
		while (!DlistIterIsEqual(SortIterToDlistIter(where), 
											DlistEnd(list->dlist)) && 
		    0 < list->cmp_func(data, DlistGetVal(SortIterToDlistIter(where))))
		{
			where = SortListNext(where);
		}
	}
	
	return where;
}

/*------------------------------- IterIsEnd ----------------------------------*/

static int IterIsEnd(sort_list_t *list, sort_list_iter_t iter)
{
	assert(list);
	
	return SortListIterIsEqual(iter, SortListEnd(list));
}





