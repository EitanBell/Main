/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 18/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Binary Search Tree   	  *
*				                	  *
**************************************/
	
#ifndef __ILRD_BST_H__
#define __ILRD_BST_H__

#include <stddef.h> /* size_t */

/*------------------------------------------------------------------------------
General Description|
-------------------
- Binary Search Tree is a tree data structure where elements are sorted by a key,
  and each node has two child nodes at most.
- For each node, its right subtree contains elements which have greater key
  value, and its left subtree contains elements which have lesser key value.
- This implementation supports iteration over the tree, using an iterator.
- The BST allows insertion, search and removal of elements in low time
  complexity, using binary search.

Attributes
	Tree, Binary, Sorted.
Main Functions
	Insert, Remove, Find.
Iterative Functions
	Next, Prev, Is Equal, Begin, End.
Unique Functions
	For Each, Count, Is Empty, Get Val.
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
A structure used to manage the BST.
------------------------------------------------------------------------------*/
typedef struct bst bst_t;
/*------------------------------------------------------------------------------
A structure used to hold the data.
------------------------------------------------------------------------------*/
typedef struct bst_node bst_node_t;
/*------------------------------------------------------------------------------
An iterator, used for iterating over the tree.
Please note, a unique EOT (End-Of-Tree) iterator is used to mark failure.
Please see relevant functions under 'General Description'.
------------------------------------------------------------------------------*/
typedef bst_node_t *bst_iter_t;

/*------------------------------------------------------------------------------
Description
	A compare function that defines the sorting of the tree.
Return Value:
	Value > 0 - 'data1' is greater than 'data2'.
	Value < 0 - 'data1' is lesser than 'data2'.
	Value = 0 - 'data1' is equal to 'data2'.
------------------------------------------------------------------------------*/
typedef int (*bst_cmp_t)(const void *data1, const void *data2);

/*------------------------------------------------------------------------------
Description
	A function that defines an action to be done to the data in a tree element.
Parameters
	'data' - The element's data.
	'param' - A parameter used by the function.
Return Value (exit status):
	Value = 0 - SUCCESS.
	Otherwise - FAILURE.
WARNING!
	A callback function that changes the key value of an element may
	harm the sorting of the tree and lead to undefined behavior.
------------------------------------------------------------------------------*/
typedef int (*bst_callback_t)(void *data, void *param);

/*   Function Declarations   */

/*------------------------------------------------------------------------------
Description
	Create a BST.
Parameters
	'cmp' - A compare function that defines the sorting of the tree (see above).
Return Value
	A handle to the created BST.
	NULL is returned on failure.
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
bst_t *BSTCreate(bst_cmp_t cmp);

/*------------------------------------------------------------------------------
Description
	Destroy resources taken by an existing BST.
Parameters
	'bst' - The BST to be destroyed.
Return Value
	NO RETURN VALUE.
Complexity
	Time: O(n) / Space: O(1)
------------------------------------------------------------------------------*/
void BSTDestroy(bst_t *bst);

/*------------------------------------------------------------------------------
Description
	Insert a new data to the BST, in a sorted position.
Parameters
	'bst' - The BST to insert the data into.
	'data' - The data beign inserted to the BST.
Return Value
	An iterator to the inserted data.
Comments
	Inserting data with a key value identical to an element already existing in
	the tree will lead to undefined behavior. 
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTInsert(bst_t *bst, void *data);

/*------------------------------------------------------------------------------
Description
	Remove an element from the BST.
Parameters
	'where' - An iterator to the element to be removed.
Return Value
	NO RETURN VALUE.
Comments
	- Removal of EOT is undefined behavior.
	- Removal of an element from the BST may invalidate the element's InOrder
	  successor (see BSTNext). 
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
void BSTRemove(bst_iter_t where);

/*------------------------------------------------------------------------------
Description
	Find an element in the BST by its key value.
Parameters
	'bst' - The BST to be searched for the value.
	'data' - The key value of the searched element.
Return Value
	If the element exist in the tree, an iterator to is returned.
	Otherwise, an EOT (End-Of-Tree) iterator is returned.
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTFind(const bst_t *bst, const void *data);

/*------------------------------------------------------------------------------
Description
	Return an iterator to the next element, determined by the tree's sorting.
Parameters
	'current' - An iterator.
Return Value
	An iterator to the next element.
Comments
	Calling the function with EOT iterator is undefined behavior.
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTNext(bst_iter_t current);

/*------------------------------------------------------------------------------
Description
	Return an iterator to the previous element, determined by the tree's sorting.
Parameters
	'current' - An iterator.
Return Value
	An iterator to the previous element.
Comments
	Calling the function with iterator to the first element is undefined behavior.
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTPrev(bst_iter_t current);

/*------------------------------------------------------------------------------
Description
	Check if two iterators are equal.
Parameters
	'one' - First iterator.
	'other' - Second iterator.
Return Value
	TRUE (1) if equal, otherwise FALSE (0).
Comments
	Function may be used to check if return value from iterator functions is EOT.
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
int BSTIsEqual(bst_iter_t one, bst_iter_t other);

/*------------------------------------------------------------------------------
Description
	Return an iteratr to the element in the tree with lowest key value. 
Parameters
	'bst' - The BST for the element to be retrieved from.
Return Value
	An iterator to the element with lowest key value.
Comments
	In case of empty tree, EOT is returned.
Complexity
	Time: Average - O(log(n)), Worst - O(n) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTBegin(const bst_t *bst);

/*------------------------------------------------------------------------------
Description
	Return an iteratr to EOT. 
Parameters
	'bst' - The BST for the EOT iterator to be retrieved from.
Return Value
	An iterator to EOT.
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
bst_iter_t BSTEnd(const bst_t *bst);

/*------------------------------------------------------------------------------
Description
	Perform the action defined by 'callback' on every element in the range
	'from' till 'to' (excluding).
Parameters
	'from' - The first element in range to perform the action on.
	'to' - The end of the range (excluding).
	'callback' - A function defining the action to be performed.
	'param' - The parameter used by 'callback'(see callback's description above).
Return Value
	The function's exit status.
Comments
	In a case of failure, the function stops and isn't performed on the
	following elements.
Complexity
	Time: Average -  O(nlog(n)), Worst - O(n^2)/ Space: O(1)
------------------------------------------------------------------------------*/
int BSTForEach(bst_iter_t from, bst_iter_t to, bst_callback_t callback,
			   void *param);
																	
/*------------------------------------------------------------------------------
Description
	Return the number of elements in the tree.
Parameters
	'bst' - The tree to be counted for number of elements.
Return Value
	The number of elements in the tree.
Complexity
	Time: Average -  O(nlog(n)), Worst - O(n^2)/ Space: O(1)
------------------------------------------------------------------------------*/
size_t BSTCount(const bst_t *bst);

/*------------------------------------------------------------------------------
Description
	Check if the tree is empty.
Parameters
	'bst' - The tree to be checked for emptiness.
Return Value
	TRUE (1) if tree is empty, otherwise FALSE (0).
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
int BSTIsEmpty(const bst_t *bst);

/*------------------------------------------------------------------------------
Description
	Get the value held by an iterator.
Parameters
	'iter' - An iterator.
Return Value
	The data held by the iterator.
Comments
	In case function is called with EOT, the behavior is undefined.
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
void *BSTGetVal(bst_iter_t iter);

#endif /* __ILRD_BST_H__ */


