/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli	              *
*	Date: 14/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Knight Tour  	  		  *
*				                	  *
**************************************/

#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

#include <stddef.h> /* size_t */

/*---------------------------------------------------------------------------
Description- 


NOTE!
	The 'path' argument should be a pointer to a bit-array in size of 64 bytes.
	Otherwise an undefined behavior may happend.
---------------------------------------------------------------------------*/

int KnightTour(size_t *path, size_t start_position);

#endif /*__KNIGHT_TOUR_H__*/


