/**************************************
*				   			  		  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  		                  *
*	Date: 28/04/23	                  *
*	Subject: EXT2				      *
*				  	                  *
**************************************/
#include <stdio.h>

#include "ext2.h"

int main(void)
{
	ReadFileFromDevice("/dev/ram0", "testfile.txt");
	
    return 0;
}





