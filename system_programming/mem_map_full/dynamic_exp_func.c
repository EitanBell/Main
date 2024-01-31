

#include <stdlib.h> /* malloc */
#include <stdio.h>

#include "mem_map_full.h"

extern int dynam_exp_ext_func(void);
const int dynam_exp_const_global = 10;
static int dynam_exp_static_func(void);
static int dynam_exp_static_global = 15;
int dynam_exp_global = 20;

void DynamicExplicitLibFunc(int argc, char **argv, char **envp)
{
	int de_local_var = 3;
	static int dynam_exp_loc_var = 4;
	int *de_heap_var = NULL;
	const int de_const_var = 67;
	char *dy_string = "Eitan literali";
	de_heap_var = (int *)malloc(sizeof(int));
	*de_heap_var = 5;

	(void)argc;
	(void)argv;
	(void)envp;

	printf(" dynam_exp_static_func -        %p\n", (void *)&dynam_exp_static_func); 
	printf(" dynam_exp_ext_func -           %p\n", (void *)&dynam_exp_ext_func);    
    printf(" d_exp_heap_var -               %p\n", (void *)de_heap_var);
    printf(" dynam_exp_const_global -       %p\n", (void *)&dynam_exp_const_global); 
    printf(" dynam_exp_static_global -      %p\n", (void *)&dynam_exp_static_global); 
    printf(" dynam_exp_global -             %p\n", (void *)&dynam_exp_global); 
    printf(" local_var -                    %p\n", (void *)&de_local_var);	
    printf(" dynam_exp_loc_var -            %p\n", (void *)&dynam_exp_loc_var);
    printf(" d_exp_const_var -              %p\n", (void *)&de_const_var);
    printf(" d_exp_string -                 %p\n", (void *)dy_string);
}

static int dynam_exp_static_func(void)
{
	return 0;
}

extern int dynam_exp_ext_func(void)
{
	return 0;
}





