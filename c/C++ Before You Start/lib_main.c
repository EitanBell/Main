
#include <stddef.h> /* NULL */
#include <dlfcn.h> /* dlopen(), dlsym(), dlclose() */

int main(int argc, const char **argv)
{
	void (*Lib_funcs)(void) = NULL;
	void *dy_so_handle = dlopen("./so1.so", RTLD_LAZY);
	if (NULL == dy_so_handle)
	{
		return -1;
	}
	
	*(void **)(&Lib_funcs) = dlsym(dy_so_handle, argv[argc - 1]);
	if (NULL != Lib_funcs)
	{
		Lib_funcs();
	}
	
	dlclose(dy_so_handle);
	
	return 0;
}

