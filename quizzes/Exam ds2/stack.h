/*****************************************/
/*   Developers: Eitan Bellaiche	 */
/*   Project: Stack API		 	 */
/*   Date: 27/11/22			 */
/*					 */
/*****************************************/


#ifndef STACK_H_
#define STACK_H_

#include <stddef.h> /*size_t*/

/************************struct*****************************/
typedef struct stack stack_t;
 

/******************Function Declarations********************/		

/*-----------------------------------------------------------------------------
   Description -- StackCreate --
   Receives: capacity of stack to allocate.
   
   Note! The capacity should be larger than zero. Otherwise, undefined behavior
   may occur.
	
   Return Value: Initialized stack with capacitys' size.
   If the allocation fails, the return value is NULL.
   
   Note! It is Users' responsibility to Destroy the stack after finishing.
   complexity: time: o(1) / space: o(n)
-----------------------------------------------------------------------------*/
stack_t *StackCreate(size_t capacity);
	 
/*-----------------------------------------------------------------------------
   Description -- StackDestroy --
   Recieves a stack and frees all allocated memory in it.
	
   Return Value: Nothing, it cant fail (Using free).
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/
void StackDestroy(stack_t *stack);
	 	
/*-----------------------------------------------------------------------------
   Description -- StackPush -- 
   Recieves data argument and stack. Pushes the data to the stack.
	
   Return Value: Nothing.
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/
void StackPush(stack_t *stack, void *data);

/*-----------------------------------------------------------------------------
   Description -- StackPop --  
   Removes the top element of the received argument stack.
	
   Return Value: Nothing. 
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/	 	
void *StackPop(stack_t *stack);	
	
/*-----------------------------------------------------------------------------
   Description -- StackPeek --
   Shows the top element of the received argument stack.
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
   
   Return Value: Returns the value of the top element of the received argument 
   stack.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/
void *StackPeek(const stack_t *stack);	

/*-----------------------------------------------------------------------------
   Description -- StackSize -- 
   Gives the amount of occupied elements in the stack.
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
	
   Return Value: Returns the amount of occupied elements in the stack.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/	
size_t StackSize(const stack_t *stack);	

/*-----------------------------------------------------------------------------
   Description -- StackCapacity -- 
   Gives the max capacity of the received argument stack. 
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
	
   Return Value: Max capacity of the stack.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/		
size_t StackCapacity(const stack_t *stack);
	
/*-----------------------------------------------------------------------------
   Description -- StackIsEmpty -- 
   Checks if the stack is empty - with no element inside. 
   Note! It is Users' responsibility NOT to send a NULL arguments.
   Otherwise, undefined behavior may occur.
	
   Return Value: TRUE(=1) if the stack is empty, and FALSE(=0) if not empty.
   complexity: time: o(1) / space: o(1)
-----------------------------------------------------------------------------*/
int StackIsEmpty(const stack_t *stack);		

#endif



