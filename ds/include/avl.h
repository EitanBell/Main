/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Itay                    *
*	Date: 29/01/23	                  *
*	Valgrind & gdb tested             *
*	Subject: AVL  	  				  *
*				                	  *
**************************************/

#ifndef __AVL_H__
#define __AVL_H__

/*------------------------------------------------------------------------------
General Description|
-------------------
AVL is a balanced binary tree data structure. 
In balanced tree the difference between the height of the left sub-tree and the 
height of the right sub-tree is at most 1.
The balance allows the basic tree operations (Insert, Remove and Find) to be
in time complexity of O(log(n)), both on average and worst case.

Attributes
	Tree, Binary, Self-Balance
Main Functions
	Insert, Remove, Find.
Other Functions
	Get Height, Count, Is Empty, For Each.
------------------------------------------------------------------------------*/

/*   Includes & Macros   */

#include <sys/types.h> /* ssize_t, size_t */

/*   typedef   */

/*------------------------------------------------------------------------------
Description
	A handle to the AVL, used by the functions for performing operations on the
	data structure.
------------------------------------------------------------------------------*/
typedef struct avl avl_t;

/*------------------------------------------------------------------------------
Description
	Possible traversal orders, used by the function ForEach.
------------------------------------------------------------------------------*/		
typedef enum traversal_order
{
	PRE = 0,
	IN,
	POST
}order_t;

/*------------------------------------------------------------------------------
Description
	A compare function that defines the sorting of the tree.
Return Value:
	Value > 0 - 'data1' is greater than 'data2'.
	Value < 0 - 'data1' is lesser than 'data2'.
	Value = 0 - 'data1' is equal to 'data2'.
------------------------------------------------------------------------------*/
typedef int (*avl_cmp_t)(const void *data1, const void *data2);
/*------------------------------------------------------------------------------	
Description
	A function that defines an action to be done to the data in a tree element.
Parameters
	'data' - The element's data.
	'param' - A parameter used by the function.
Return Value (exit status):
	0 means SUCCESS
	Otherwise - FAILURE.
WARNING!
	A callback function that changes the key value of an element may
	harm the sorting of the tree and lead to undefined behavior.
------------------------------------------------------------------------------*/
typedef int (*avl_callback_t)(void *data, void *param);

/*   Function Declarations   */

/*------------------------------------------------------------------------------
Description
	Create an AVL tree.
Parameters
	'cmp' - A compare function that defines the sorting of the tree (see ablove).
Return Value
	A handle to the AVL tree, NULL on failure.
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
avl_t *AVLCreate(avl_cmp_t cmp);

/*------------------------------------------------------------------------------
Description
	Destroy the resources taken by the AVL tree.
Parameters
	'avl' - The tree to be destroyed.
Return Value
	NO RETURN VALUE.																
Complexity
	Time: O(n) / Space: O(1)
------------------------------------------------------------------------------*/
void AVLDestroy(avl_t *avl);

/*------------------------------------------------------------------------------
Description
	Insert a new element to the tree, in a sorted position.
Parameters
	'avl' - The AVL tree.
	'data' - The data to be inserted.
Return Value
	0 on SUCCESS.
	Non-Zero value on FAILURE.
Complexity
	Time: O(log(n)) / Space: O(1)
------------------------------------------------------------------------------*/
int AVLInsert(avl_t *avl, void *data);

/*------------------------------------------------------------------------------
Description
	Remove data from the tree.
Parameters
	'avl' - The AVL tree.
	'data' - The data to be removed.
Return Value
	NO RETURN VALUE.
Complexity
	Time: O(log(n)) / Space: O(1)
------------------------------------------------------------------------------*/
void AVLRemove(avl_t *avl, void *data);

/*------------------------------------------------------------------------------
Description
	Find if an element is present in the tree.
Parameters
	'avl' - The AVL tree.
	'data' - The key of the data to be searched.
	'found_data' - If key is found, the matching data is placed in this variable.
Return Value
	1 - The data is present in the tree, oherwise 0.								
Comments																			
	If data is not found in the tree, the content of 'found_data' is undefined.
Complexity
	Time: O(log(n)) / Space: O(1)
------------------------------------------------------------------------------*/
int AVLFind(const avl_t *avl, const void *data, void **found_data);

/*------------------------------------------------------------------------------
Description
	Perform the action defined by 'callback' on every element in the tree.
Parameters
	'avl' - The AVL tree.
	'callback' - A function defining the action to be performed.
	'param' - The parameter used by 'callback'(see callback's description above).
	'order' - The order of traversal on the elements (see traversal_order's
	description above)
Return Value
	The function's exit status.
Comments
	In a case of failure, the function stops and isn't performed on the
	following elements.
Complexity
	Time: O(n) / Space: O(1)
------------------------------------------------------------------------------*/
int AVLForEach(avl_t *avl, avl_callback_t callback, void *param, order_t order);

/*------------------------------------------------------------------------------
Description
	Return the height of the tree.
Parameters
	'avl' - The AVL tree.
Return Value
	The height of the tree.
Comments
	If the tree is empty, -1 is returned.											
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
ssize_t AVLGetHeight(const avl_t *avl);

/*------------------------------------------------------------------------------
Description
	Return the number of elements in the tree.
Parameters
	'avl' - The AVL tree.
Return Value
	The number of elements in the tree.												
Complexity
	Time: O(n) / Space: O(1)
------------------------------------------------------------------------------*/
size_t AVLCount(const avl_t *avl);

/*------------------------------------------------------------------------------
Description
	Return if the tree is empty or not.
Parameters
	'avl' - The AVL tree.
Return Value
	1 if the tree is empty, otherwise 0.									
Complexity
	Time: O(1) / Space: O(1)
------------------------------------------------------------------------------*/
int AVLIsEmpty(const avl_t *avl);

#endif /* __AVL_H__ */
