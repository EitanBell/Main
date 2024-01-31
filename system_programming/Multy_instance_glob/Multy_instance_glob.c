

#include <stdio.h>	/* printf */
#include "Multy_instance_glob.h"	/* exercise header */

int x = 126;

void PrintGlobalVariableAddress(void)
{
	printf("The address of the global variable is: %p\n", (void *)&x);	
}

