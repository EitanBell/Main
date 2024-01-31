/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Eliran	              *
*	Date: 06/03/23	                  *
*	Subject: Watchdog				  *
*				                	  *
**************************************/

#include <stdio.h>	/* printf */
#include <unistd.h>	/* sleep */

#include "watchdog.h"	/* watchdog API */

int main(int argc, char **argv)
{
    unsigned int time = 40;
    (void)argc;
    
    WDStart(argc, (const char **)argv);
    
    while (0 != (time = sleep(time)));
    printf("Appling WDstop()\n");
    WDStop();
	printf("WD stopped\n");
    return 0;
}


