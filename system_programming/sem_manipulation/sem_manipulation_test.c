/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 27/02/23	                  *
*	Subject: Sem Manipulation		  *
*				                	  *
**************************************/

#include "sem_manipulation.h"		/* sem_manipulation API */

int main(int argc, char **argv)
{
	int status = 0;
	
	status = SemManipulations(argv[(argc - 1)]);
	
	return status;
}


