/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad    	     *
*	Date: 01/12/22	             *
*	Valgrind & GDB passed        *
*	Subject: Bit_array	     *
*				     *
**************************************/

#ifndef __BITARR_H__
#define __BITARR_H__

#include <stddef.h>	/* size_t */

/************************struct*****************************/
typedef size_t bit_array_t;

typedef enum bit_val
{
	bit_off = 0,
	bit_on = 1
}bit_val_t;
/******************Function Declarations********************/

/*time: o(1) / space: o(1)*/
bit_array_t BitArrSetOn(bit_array_t bitarr, size_t i);
/*---------------------------------------------------------------------------
Description- 

i larger then bitarr's size will lead to undefined behavior
Return Value

---------------------------------------------------------------------------*/
/*time: o(1) / space: o(1)*/
bit_array_t BitArrSetOff(bit_array_t bitarr, size_t i);
/*-----------------------------------------------------------------------------
Description 

i larger then bitarr's size will lead to undefined behavior	
Return Value

-----------------------------------------------------------------------------*/	 
/*time: o(1) / space: o(1)*/	 	
bit_array_t BitArrFlipBit(bit_array_t bitarr, size_t i);
/*-----------------------------------------------------------------------------
Description 

i larger then bitarr's size will lead to undefined behavior
Return Value

-----------------------------------------------------------------------------*/	 

/*time: o(1) / space: o(1)*/	
int BitArrGetVal(bit_array_t bitarr, size_t i);	
/*-----------------------------------------------------------------------------
Description 

i larger then bitarr's size will lead to undefined behavior
Return Value

-----------------------------------------------------------------------------*/	 

/*time: o(1) / space: o(1)*/	
bit_array_t BitArrSetVal(bit_array_t bitarr, size_t i, bit_val_t value);
/*-----------------------------------------------------------------------------
Description 


Return Value

-----------------------------------------------------------------------------*/	 

/*time: o(1) / space: o(1)*/	
bit_array_t BitArrFlipAll(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	 

/*time: o(1) / space: o(1)*/	
bit_array_t BitArrSetAll(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 


Return Value

-----------------------------------------------------------------------------*/	 
/*time: o(1) / space: o(1)*/	
bit_array_t BitArrUnsetAll(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	
/*time: o(1) / space: o(1)*/	
bit_array_t BitArrRotateLeft(bit_array_t bitarr, size_t n);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	
/*time: o(1) / space: o(1)*/	
bit_array_t BitArrRotateRight(bit_array_t bitarr, size_t n);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	
/*time: o(1) / space: o(1)*/	
bit_array_t BitArrMirror(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

/*time: o(1) / space: o(1)*/	
bit_array_t BitArrMirrorLut(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

/*time: o(1) / space: o(1)*/	
bit_array_t BitArrMirrorO1(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

/*time: o(n) / space: o(1)*/	
size_t BitArrCountSetBits(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

/*time: o(1) / space: o(n)*/	
size_t BitArrCountSetBitsLut(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

/*time: o(1) / space: o(n)*/	
size_t BitArrCountSetBitsHamming(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	


/*time: o(n) / space: o(1)*/	
size_t BitArrCountOffBits(bit_array_t bitarr);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	
/*time: o(1) / space: o(1)*/	
char *BitArrToString(bit_array_t bitarr, char *buffer);
/*-----------------------------------------------------------------------------
Description 

	
Return Value

-----------------------------------------------------------------------------*/	

#endif 		/* __BITARR_H__ */



