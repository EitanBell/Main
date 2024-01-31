/**************************************
*				   			 		  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Gali                	  *
*	Date: 17/12/22	                  *
*	Valgrind & gdb passed   	      *
*	Subject: doubly Linked List	 	  *
*				              	      *
**************************************/


#ifndef __DLIST_H__
#define __DLIST_H__


typedef struct dlist dlist_t;
typedef struct dlist_node *dlist_iter_t;

typedef int (*dlist_do_action_t)(void *data, void *param);
typedef int (*dlist_is_match_t)(const void *data, void *param);

/**
 * The function creates a doubly linked list.
 * @return The function returns pointer to the created singly linked list or 
 * NULL on fail.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_t *DlistCreate(void); 


/**
 * The function frees the allocated memory of the doubly linked list.
 * passing a NULL will cause an undefined behavior.
 * @param list: the dlist to free.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
void DlistDestroy(dlist_t *list);


/**
 * The function returns the first iter in the passed argument list. 
 * @param list: the list from which the first iter will be returned.
 * @return the first iter in the list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistBegin(const dlist_t *list);


/**
 * The function returns the last iter in the passed argument list. 
 * @param list: the list from which the last iter will be returned.
 * @return the last iter in the argument list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistEnd(const dlist_t *list);


/**
 * The function returns the next iter from the passed iter. 
 * @param current: the iter from which the next iter will be returned.
 * @return the next iter from the argument current.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistNext(dlist_iter_t current);


/**
 * The function returns the previous iter from the passed iter. 
 * @param current: the iter from which the previous iter will be returned.
 * @return the previous iter from the argument current.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistPrev(dlist_iter_t current);


/**
 * The function inserts the argument data to the start of the doubly linked 
 * list.
 * @param list: the list to insert data to.
 * @param data: the new data to insert to the list.
 * @return the new created first iter that contains the passed argument data.
 * @time_complexity O(1) (worst: O(n))
 * @space_complexity O(1)
 */
dlist_iter_t DlistPushFront(dlist_t *list, void *data);


/**
 * The function inserts the argument data to the end of the doubly linked list.
 * @param list: the list to insert data to.
 * @param data: the new data to insert to the list.
 * @return the new created last iter that contains the passed argument data.
 * @time_complexity O(1) (worst: O(n))
 * @space_complexity O(1)
 */
dlist_iter_t DlistPushBack(dlist_t *list, void *data);


/**
 * The function removes the first iter from the doubly linked list.
 * The first remove must come after at least one insert - trying to remove 
 * before that will cause an undefined behavior. 
 * @param list: the list to remove from.
 * @return the data from the removed iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *DlistPopFront(dlist_t *list);


/**
 * The function removes the last iter from the doubly linked list.
 * The first remove must come after at least one insert - trying to remove 
 * before that will cause an undefined behavior. 
 * @param list: the list to remove from.
 * @return the data from the removed iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *DlistPopBack(dlist_t *list);


/**
 * The function inserts the argument data to the doubly linked list, before the 
 * where argument.
 * @param where: the location to insert data
 * @param data: the new data to insert to the list
 * @return the new created iter that contains the passed argument data, or end
 * of list in case of failure.
 * @time_complexity O(1), worst: O(n)
 * @space_complexity O(1)
 */
dlist_iter_t DlistInsertBefore(dlist_iter_t where, void *data);


/**
 * The function removes the passed argument iter from the doubly linked list.
 * The first remove must come after at least one insert. trying to remove 
 * before that will cause an undefined behavior. 
 * @param where: the location to remove iter from list.
 * @return the iter after the removed one in the list.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistRemove(dlist_iter_t where);


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
dlist_iter_t DlistFind(const dlist_iter_t from, const dlist_iter_t to, 
					   dlist_is_match_t is_match, void *param);
					   

/**
 * The function finds in which iters the passed argument data is matching, 
 * searching in the scope between the passed arguments from and to (not 
 * including to), using the passed funcion, and updates tne result_list with
 * the relevant data.
 * @param result_list: the list to update.
 * @param from: the location to start the search.
 * @param to: the location to stop the search, not including.
 * @param is_match: a pointer to a comparison function 
 * @param param: the parametar to search.
 * @return status, 1 for success or 0 for fail. 
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
int DlistMultiFind(dlist_t *result_list ,const dlist_iter_t from,
 		   const dlist_iter_t to, dlist_is_match_t is_match, void *param);


/**
 * The function doing an action according to the passed argument function, 
 * on different iters along the list that placed between the passed 
 * arguments from and to.
 * @param from: the location to start the action.
 * @param to: the location to stop the action, not including.
 * @param do_action: a pointer to an action function 
 * @param param: a parametar used in the action function.
 * @return a status for failure or success.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
int DlistForEach(dlist_iter_t from, dlist_iter_t to, 
			     dlist_do_action_t do_action, void *param); 


/**
 * The function moves a section of iters placed between the passed 
 * arguments from and to, to a new location, before the passed argument where.
 * @param from: the location to start the section.
 * @param to: the location to stop the section, not including.
 * @param where: indicats the new location of the spliced iters.
 * @return the last node in the splice.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
dlist_iter_t DlistSplice(dlist_iter_t from, dlist_iter_t to,
														    dlist_iter_t where);


/**
 * The functions returns value that is placed in the passed argument iter.
 * @param iter: the location to get the value from.
 * @return the data that is in the specified iter.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
void *DlistGetVal(const dlist_iter_t iter);


/**
 * The function sets the value of the passed argument iter to the value of the 
 * passed argument data.
 * @param iter: the location to set the value of.
 * @param data: the data to set.
 * @return the specified iter with his new value.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
dlist_iter_t DlistSetVal(dlist_iter_t iter, void *data);


/**
 * The function counts the number of iters that exist in the passed 
 * argument list.
 * @param list: the list to count.
 * @return the number of iters in the specified list.
 * @time_complexity O(n)
 * @space_complexity O(1)
 */
size_t DlistCount(const dlist_t *list);


/**
 * The function checks if two passed iters share the same place. 
 * @param one: one iter to compere.
 * @param other: second iter to compere.
 * @return 1 for true, 0 for false.

 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int DlistIterIsEqual(const dlist_iter_t one, const dlist_iter_t other);


/**
 * The function checks if the passed list is empty. 
 * @param list - the list to check if empty.
 * @return 1 for true, 0 for false.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int DlistIsEmpty(const dlist_t *list);




#endif /* __DLIST_H__ */

