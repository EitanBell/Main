/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  Aviv                    	   *
*	Date: 12/03/23	             	       *
*	Subject: Shuffle and Sort dict files   *
*				                		   *
*******************************************/

#ifndef  __SHUFFLE_N_SORT_DICT__
#define  __SHUFFLE_N_SORT_DICT__

/*-------------------------------------------------------------------------
General Description:



---------------------------------------------------------------------------*/
/*   Includes & Macros   */

#include <stdlib.h>		/* size_t */

/*   Function Declarations   */
/*-------------------------------------------------------------------------

---------------------------------------------------------------------------*/
int MTMergeSortDictionary(unsigned char **dict, size_t size, size_t threads_num);

#endif /* __SHUFFLE_N_SORT_DICT__ */
