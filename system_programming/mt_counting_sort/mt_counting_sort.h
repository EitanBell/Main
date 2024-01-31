/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Yuval                   	   *
*	Date: 09/03/23	             	       *
*	Subject: Multi Threaded Counting Sort  *
*				                		   *
*******************************************/

#ifndef __MT_COUNTING_SORT__
#define __MT_COUNTING_SORT__

/*   Includes & Macros   */

#include <stdlib.h>		/* size_t */

/*   Function Declarations   */

int MTCountingSort(unsigned char *arr, size_t size, size_t threads_num);

#endif /* __MT_COUNTING_SORT__ */
