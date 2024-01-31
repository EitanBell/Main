

#include <stdlib.h> /* malloc */
#include <stdio.h>

#include "mem_map_full.h"

extern int dynam_imp_ext_func(void);
const int dynam_imp_const_global = 12;
static int dynam_imp_static_global = 5;
static int dynam_imp_static_func(void);
int dynam_imp_global = 7;

void DynamicImplicitLibFunc(int argc, char **argv, char **envp)
{
	int d_local_var = 3;
	static int dynam_imp_loc_var = 4;
	int *d_heap_var = NULL;
	const int d_const_var = 67;
	char *d_string = "literali Eitan";
	d_heap_var = (int *)malloc(sizeof(int));
	*d_heap_var = 5;

	(void)argc;
	(void)argv;
	(void)envp;

	printf(" dynam_imp_static_func -       %p\n", (void *)&dynam_imp_static_func); 
	printf(" dynam_imp_ext_func -          %p\n", (void *)&dynam_imp_ext_func);    
	printf(" d_imp_heap_var -              %p\n", (void *)d_heap_var);
	printf(" dynam_imp_const_global -      %p\n", (void *)&dynam_imp_const_global); 
	printf(" dynam_imp_static_global -     %p\n", (void *)&dynam_imp_static_global); 
	printf(" dynam_imp_global -            %p\n", (void *)&dynam_imp_global); 
	printf(" local_var -                   %p\n", (void *)&d_local_var);	
	printf(" dynam_imp_loc_var -           %p\n", (void *)&dynam_imp_loc_var);
	printf(" d_imp_const_var -             %p\n", (void *)&d_const_var);
	printf(" d_imp_string -                %p\n", (void *)d_string);
}



static int dynam_imp_static_func(void)
{
	return 0;
}

extern int dynam_imp_ext_func(void)
{
	return 0;
}





