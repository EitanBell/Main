

#include <stdio.h> /* printf, NULL */
#include <dlfcn.h> /* dlopen, dlclose, dlsys */

typedef void *(p_func_t)(int);

int main(void)
{
	int num = 127;
	p_func_t *Foo = NULL;
	
	void *handle_explicit = dlopen("./lib_dynam_ex.so", RTLD_GLOBAL | RTLD_LAZY);
	if (NULL == handle_explicit)
	{
		return 1;
	}
	    
    *(void **)(&Foo) = dlsym(handle_explicit, "Foo");
    
    Foo(num);
    
    dlclose(dynamic_lib);
    
    return 0;
}



