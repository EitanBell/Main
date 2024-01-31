/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:                       *
*	Date: 29/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Sorted List   	       *
*				                 *
**************************************/

#ifndef __SORTLIST_H__
#define __SORTLIST_H__

#include <stddef.h>		/* size_t */

#include "dlist.h"      /*doubly linked list*/

/*------------------------------------------------------------------------------
SORTED LIST:
	A sorted list is a data structure that its elements are placed in user's
	order, by given funtion. Insertion of a new element depends only by user's 
	funcion.

MAIN FUNCTIONALITY:
	Insert, remove, pop back, pop front, find, do action for each node, and 
	merge two lists.
------------------------------------------------------------------------------*/

typedef struct sort_list sort_list_t;

typedef int (*sort_list_cmp_t)(const void *data1, const void *data2);
typedef int (*sort_list_do_action_t)(void *data, void *param);
typedef int (*sort_list_is_match_t)(const void *data, void *param);

struct sort_list_iter
{
	dlist_iter_t iter;
    #ifndef NDEBUG
	    sort_list_t *list;
    #endif
};
typedef struct sort_list_iter sort_list_iter_t;

/**
 * The function creates a sorted list.
 * @param cmp: A comparison function that describes the order of the list items.
 * @return Pointer to the created list or NULL if fails.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_t *SortListCreate(sort_list_cmp_t cmp);

/** 	
 * The function release resources of the list. passing a NULL argument will 
 * cause an undefined behavior.
 * @param list: The list to release.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
void SortListDestroy(sort_list_t *list);

/**
 * The function inserts the argument data to the list, in the right location
 * according to the users comparison function of the list.
 * @param list: the list to insert data to.
 * @param data: the new data to insert to the list.
 * @return the new created iter that contains the passed argument data, or end
 * of list in case of failure.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListInsert(sort_list_t *list, void *data);

/**
 * The function removes the passed argument iter from the list. The first remove 
 * must come after at least one insert. Trying to remove before that will cause 
 * an undefined behavior. 
 * @param iter: The iter to remove from list.
 * @return The iter after the removed one in the list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListRemove(sort_list_iter_t iter);

/**
 * The function finds in which iter the passed argument data is, searching in 
 * the scope between the passed arguments from and to (not including to), 
 * using the list comparison function.
 * @param list: The list to search in. 
 * @param from: The location to start the search, included.
 * @param to: The location to stop the search, not included.
 * @param param: The parametar to search.
 * @return The iter that its data matches to the passed one, if there isn't
 * one, the functions returns 'to' iter. 
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListFind(const sort_list_t *list ,const sort_list_iter_t from,
							 const sort_list_iter_t to, const void *param);		 
	 
/**
 * The function finds in which iter the passed argument data is, searching in 
 * the scope between the passed arguments from and to (not including to), 
 * using the passed funcion.
 * @param from: the location to start the search.
 * @param to: the location to stop the search, not including.
 * @param is_match: a pointer to a comparison function 
 * @param param: the parametar to search.
 * @return the iter that its data matches to the passed one, if there isn't
 * one, the functions returns to. 
 * @time_complexity O(n)
 * @space_complexity O(1)
 */									 
sort_list_iter_t SortListFindIf(const sort_list_iter_t from, 
							   const sort_list_iter_t to, 
							   sort_list_is_match_t is_match, void *param);	 					   
							   
/**
 * The function is doing an action according to the passed argument function, 
 * on different data along the list that placed between from and to arguments.
 * If the do_action func changes the datas' hirarchi - an undifined behavior
 * may happend.
 * @param from: The location to start the action.
 * @param to: The location to stop the action, not including.
 * @param do_action: A pointer to an action function.
 * @param param: A parametar used in the action function.
 * @return A status for failure or success.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
int SortListForEach(sort_list_iter_t from, sort_list_iter_t to, 
			        sort_list_do_action_t do_action, void *param);			        
			        
/**
 * The function returns the first iter in the passed argument list. 
 * @param list: the list from which the first iter will be returned.
 * @return the first iter in the list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListBegin(const sort_list_t *list);

/**
 * The function returns the last iter in the passed argument list. 
 * @param list: the list from which the last iter will be returned.
 * @return the last iter in the argument list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListEnd(const sort_list_t *list);

/**
 * The function returns the next iter from the passed iter. 
 * @param current: the iter from which the next iter will be returned.
 * @return the next iter from the argument current.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListNext(sort_list_iter_t current);

/**
 * The function returns the previous iter from the passed iter. 
 * @param current: the iter from which the previous iter will be returned.
 * @return the previous iter from the argument current.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
sort_list_iter_t SortListPrev(sort_list_iter_t current);

/**
 * The functions returns value that is placed in the passed argument iter.
 * @param iter: the location to get the value from.
 * @return the data that is in the specified iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *SortListGetVal(const sort_list_iter_t iter);

/**
 * The function checks if two passed iters share the same place. 
 * @param one: one iter to compere.
 * @param other: second iter to compere.
 * @return 1 for true, 0 for false.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int SortListIterIsEqual(const sort_list_iter_t one, const sort_list_iter_t other);

/**
 * The function counts the number of elements that exist in the passed 
 * argument list.
 * @param list: the list to count.
 * @return the number of elements in the given list.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
size_t SortListCount(const sort_list_t *list);

/**
 * The function checks if the passed list is empty. 
 * @param list - the list to check if empty.
 * @return 1 for true, 0 for false.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int SortListIsEmpty(const sort_list_t *list);

/**
 * The function removes the first data from the list.
 * Popping in an empty list will cause an undefined behavior. 
 * @param list: the list to remove from.
 * @return the data from the removed iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *SortListPopFront(sort_list_t *list);

/**
 * The function removes the last iter from the list.
 * The first remove must come after at least one insert - trying to remove 
 * before that will cause an undefined behavior. 
 * @param list: the list to remove from.
 * @return the data from the removed iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *SortListPopBack(sort_list_t *list);

/**
 * The function unites two lists into dest, according to the order in their  
 * comparison function. Sending lists with different sort conditions
 * (comparison functions) will cuase an undefined behavior.
 * @param dest: the first list to merge.
 * @param src: the second list to merge, will remain empty list after the merge.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
void SortListMerge(sort_list_t *dest, sort_list_t *src); 
/*----------------------------------------------------------------------------*/

#endif /* __SORTLIST_H__ */


