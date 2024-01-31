/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:      	            *
*	Date: 07/12/22	                 *
*	     				       *
*	Subject: Signle Linked List	  *
*				                 *
**************************************/
#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>		/* size_t */

typedef struct slist slist_t;
typedef struct slist_node *slist_iter_t;

typedef int (*slist_do_action_t)(void *data, void *param);
typedef int (*slist_is_match_t)(const void *data, void *param);

/*---------------------------------------------------------------------------
Description:
SlistCreate create a slist 
user need to use destroy to free the list
Return Value:
pointer to slist
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_t *SlistCreate(void); 

/*---------------------------------------------------------------------------
Description:
SlistDestroy relase memory who was allocated to the list.
Complexity: 
	time: O(n)  / space: O(1) 
---------------------------------------------------------------------------*/
void SlistDestroy(slist_t *list);

/*---------------------------------------------------------------------------
Description:
SlistInsertBefore insert the data to an argument before the iter who was sent,
Return Value:
The srgument where the data was insert. If FAIL return NULL.
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistInsertBefore(slist_iter_t where, void *data);

/*---------------------------------------------------------------------------
Description:
SlistRemove remove the iter who was sent.
Return Value:
The next iter
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistRemove(slist_iter_t where);

/*---------------------------------------------------------------------------
Description:
SlistFind get range of where to check as iterators, a pointer to a function who can compare the param to what she checks and a pointer to the function.
Return Value:
The iter where it was found. if FAIL return to											
Complexity: 
	time: O(n)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistFind(const slist_iter_t from, const slist_iter_t to, 
					   slist_is_match_t is_match, void *param);
					   
/*---------------------------------------------------------------------------
Description:
SlistForEach do a action to each node in the list. Get a pointer to the function who do the action.
Return Value:
return status.
Complexity: 
	time: O(n)  / space: O(1) 
---------------------------------------------------------------------------*/
int SlistForEach(slist_iter_t from, slist_iter_t to, 
			     slist_do_action_t do_action, void *param); 

/*---------------------------------------------------------------------------
Description:
SlistGetVal get an iter.
Return Value:
pinter to the data who stored in the iter
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
void *SlistGetVal(const slist_iter_t iter);

/*---------------------------------------------------------------------------
Description:
SlistSetVal set the val to the iter who was sent.
Return Value:
Return the iter who was set.	
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistSetVal(slist_iter_t iter, void *data);

/*---------------------------------------------------------------------------
Description:
SlistCount counts number of members in the list.
Return Value:
Number of members.	
Complexity: 
	time: O(n)  / space: O(1) 
---------------------------------------------------------------------------*/
size_t SlistCount(const slist_t *list);

/*---------------------------------------------------------------------------
Description:
SlistIterIsEqual sheck if two iters are equals
Return Value:
Return bool if yes (1) or no (0).  
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
int SlistIterIsEqual(const slist_iter_t one, const slist_iter_t other);

/*---------------------------------------------------------------------------
Description:
SlistIsEmpty check if a list is empty.
Return Value:
Return bool if yes (1) or no (0). 
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
int SlistIsEmpty(const slist_t *list);

/*---------------------------------------------------------------------------
Description:
SlistBegin returns iter for the first member in a list.	
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistBegin(const slist_t *list);

/*---------------------------------------------------------------------------
Description:
SlistEnd returns iter for the last member in a list.		
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_iter_t SlistEnd(const slist_t *list);

/*---------------------------------------------------------------------------
Description:
SlistAppend returns iter for the dest list was appdnded.
note! the src list is empty and can be reused by the user.		
Complexity: 
	time: O(1)  / space: O(1) 
---------------------------------------------------------------------------*/
slist_t *SlistAppend(slist_t *dest, slist_t *src);
int SlistIterIsEqual(const slist_iter_t one, const slist_iter_t other);

#endif /* __SLIST_H__ */
