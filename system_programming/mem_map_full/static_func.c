#include <stdlib.h> /* malloc */
#include <stdio.h>

#include "mem_map_full.h"

static int stat_static_func(void);
extern int stat_ext_func(void);
const int stat_const_global = 10;
static int stat_static_global = 15;
int stat_global = 20;

void StaticLibFunc(int argc, char **argv, char **envp)
{
    int ret = 34;
    int local_var = 13;
    static int stat_loc_var = 55;
    int *heap_var = NULL;
    const int const_var = 1;
    char *string = "Eitan literali";
    heap_var = (int *)malloc(sizeof(int));
    *heap_var = 5;

	(void)argc;
	(void)argv;
	(void)envp;

	printf(" stat_static_func -               %p\n", (void *)&stat_static_func); 
	printf(" stat_ext_func -                  %p\n", (void *)&stat_ext_func);    
    printf(" s_heap_var -                     %p\n", (void *)heap_var);
    printf(" stat_const_global -              %p\n", (void *)&stat_const_global); 
    printf(" stat_static_global -             %p\n", (void *)&stat_static_global); 
    printf(" stat_global -                    %p\n", (void *)&stat_global); 
    printf(" local_var -                      %p\n", (void *)&local_var);
    printf(" stat_local_var -                 %p\n", (void *)&stat_loc_var);
    printf(" s_const_var -                    %p\n", (void *)&const_var);
    printf(" s_string -                       %p\n", (void *)string);
}

static int stat_static_func(void)
{
	return 0;
}

extern int stat_ext_func(void)
{
	return 0;
}





