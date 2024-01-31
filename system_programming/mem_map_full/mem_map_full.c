/*----------------------------------------------------------------------------*/
/* DEVELOPER: Or Abadi	                                                      */
/* LAST MODIFIED: 26/12/2022                                                  */
/* WORKSHEET NAME: Multiple instances of Global Variable 	                  */
/*----------------------------------------------------------------------------*/


#include <stdio.h>
#include <dlfcn.h>

#include "mem_map_full.h"

typedef void (* p_func_t)(void);

int main(int argc, char **argv, char **envp)
{
	void *handle = NULL;
	p_func_t p_func = NULL;
	
	printf("\nstatic library:\n");
	StaticLibFunc(argc, argv, envp);
	
	printf("\nDynamic Implicit library:\n");
	DynamicImplicitLibFunc(argc, argv, envp);
		
		
	handle = dlopen("./lib_dynamic_exp.so", RTLD_GLOBAL | RTLD_LAZY);
	if (NULL == handle)
	{
		return -1;
	}
	
	*(void **)&p_func = dlsym(handle, "DynamicExplicitLibFunc");
    if (NULL == handle)
    {
      return -1;
    }
    
	printf("\nDynamic Explicit:\n");
	p_func();
	
    dlclose(handle);
    
    return 0;
}

/*


static library:
 stat_static_func -               0x5567328fb4d6
 stat_ext_func -                  0x5567328fb4e5
 s_heap_var -                     0x5567341356b0
 stat_const_global -              0x5567328fc070
 stat_static_global -             0x5567328fe010
 stat_global -                    0x5567328fe014
 local_var -                      0x7ffd5888b72c
 stat_local_var -                 0x5567328fe018
 s_const_var -                    0x7ffd5888b730
 s_string -                       0x5567328fc074

Dynamic Implicit library:
 dynam_imp_static_func -           0x7f22550042ef
 dynam_imp_ext_func -              0x7f22550042fe
 d_imp_heap_var -                  0x5567341356d0
 dynam_imp_const_global -          0x7f2255005000
 dynam_imp_static_global -         0x7f2255007038
 dynam_imp_global -                0x7f225500703c
 local_var -                       0x7ffd5888b730
 dynam_imp_loc_var -               0x7f2255007040
 d_imp_const_var -                 0x7ffd5888b734
 d_imp_string -                    0x7f2255005004

Dynamic Explicit:
 dynam_exp_static_func -        0x7f2254fff2ef
 dynam_exp_ext_func -           0x7f2254fff2fe
 d_exp_heap_var -               0x556734135da0
 dynam_exp_const_global -       0x7f2255000000
 dynam_exp_static_global -      0x7f2255002038
 dynam_exp_global -             0x7f225500203c
 local_var -                    0x7ffd5888b730
 dynam_exp_loc_var -            0x7f2255002040
 d_exp_const_var -              0x7ffd5888b734
 d_exp_string -                 0x7f2255000004



Mapped address spaces:

          Start Addr           End Addr       Size     Offset  Perms  objfile
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /home/eitan/git/system_programming/mem_map_full/mem_map_full
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /home/eitan/git/system_programming/mem_map_full/mem_map_full
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/mem_map_full
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/mem_map_full
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /home/eitan/git/system_programming/mem_map_full/mem_map_full
      0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]
      0x7ffff7d79000     0x7ffff7d7c000     0x3000        0x0  rw-p   
      0x7ffff7d7c000     0x7ffff7da4000    0x28000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7da4000     0x7ffff7f39000   0x195000    0x28000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f39000     0x7ffff7f91000    0x58000   0x1bd000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f91000     0x7ffff7f95000     0x4000   0x214000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f95000     0x7ffff7f97000     0x2000   0x218000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7f97000     0x7ffff7fa4000     0xd000        0x0  rw-p   
      0x7ffff7fb1000     0x7ffff7fb2000     0x1000        0x0  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_exp.so
      0x7ffff7fb2000     0x7ffff7fb3000     0x1000     0x1000  r-xp   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_exp.so
      0x7ffff7fb3000     0x7ffff7fb4000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_exp.so
      0x7ffff7fb4000     0x7ffff7fb5000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_exp.so
      0x7ffff7fb5000     0x7ffff7fb6000     0x1000     0x3000  rw-p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_exp.so
      0x7ffff7fb6000     0x7ffff7fb7000     0x1000        0x0  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_imp.so
      0x7ffff7fb7000     0x7ffff7fb8000     0x1000     0x1000  r-xp   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_imp.so
      0x7ffff7fb8000     0x7ffff7fb9000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_imp.so
      0x7ffff7fb9000     0x7ffff7fba000     0x1000     0x2000  r--p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_imp.so
      0x7ffff7fba000     0x7ffff7fbb000     0x1000     0x3000  rw-p   /home/eitan/git/system_programming/mem_map_full/lib_dynamic_imp.so
      0x7ffff7fbb000     0x7ffff7fbd000     0x2000        0x0  rw-p   
      0x7ffff7fbd000     0x7ffff7fc1000     0x4000        0x0  r--p   [vvar]
      0x7ffff7fc1000     0x7ffff7fc3000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fc5000     0x7ffff7fef000    0x2a000     0x2000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
--Type <RET> for more, q to quit, c to continue without paging--
      0x7ffff7fef000     0x7ffff7ffa000     0xb000    0x2c000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x37000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x39000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]
  0xffffffffff600000 0xffffffffff601000     0x1000        0x0  --xp   [vsyscall]


*/

