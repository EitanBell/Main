/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 20/02/23	                  *
*	Subject: Simple Shell		 	  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */ 
#include <unistd.h>		/* sleep */ 

int main(void)
{
	size_t x = 5;

	printf("5\n");
	sleep(1);

	printf("4\n");
	sleep(1);
	
	printf("3\n");
	sleep(1);
	
	printf("2\n");
	sleep(1);
	
	printf("1\n");
	sleep(1);
	
	x = x / 0;
	
	printf("Please don't print me'\n");
	
	return 0;
}


