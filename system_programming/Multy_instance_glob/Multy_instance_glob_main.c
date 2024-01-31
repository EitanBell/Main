

#include <stdio.h> /* printf */
#include <dlfcn.h> /* dlsym */

#include "Multy_instance_glob.h" /* exercise header */

typedef void (* p_func_t)(void);

int main(void)
{
	void *handle1 = NULL;
	void *handle2 = NULL;

	p_func_t p_foo = NULL;	
	p_func_t p_foo2 = NULL;

	printf("static\n");
	PrintGlobalVariableAddress();
		
	handle1 = dlopen("./Multy_instance_glob.so", RTLD_GLOBAL | RTLD_LAZY);
	if (NULL == handle1)
	{
		return -1;
	}

	*(void **)&p_foo = dlsym(handle1, "Foo");
	if (NULL == handle1)
	{
	 return -1;
	}

	printf("Dynamic library 1:\n");
	p_foo();

	handle2 = dlopen("./Multy_instance_glob2.so", RTLD_GLOBAL | RTLD_LAZY);
	if (NULL == handle2)
	{
		return -1;
	}
	*(void **)&p_foo2 = dlsym(handle2, "Foo2");
	if (NULL == handle2)
	{
	 return -1;
	}
    
	printf("Dynamic library 2:\n");
	p_foo2();

	dlclose(handle1);
	dlclose(handle2);
    
	return 0;
}

/*

eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c Multy_instance_glob.c -fPIC
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ ar -rcs libmulti.a Multy_instance_glob.o
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c Foo.c -fPIC
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c Foo2.c -fPIC
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd Foo.o libmulti.a -shared -o libmulti.so
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd Foo2.o libmulti.a -shared -o libmulti2.so
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c FooMain.c -I.
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd FooMain.o -L. -lmulti -o multi -Wl,-rpath=.
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ ./multi

The address of the global variable is: 0x7f9492b92030

eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c Multy_instance_glob.c
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ ar -rcs libstatic.a Multy_instance_glob.o
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd -c Multy_instance_glob_main.c -I.
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ gd Multy_instance_glob_main.o -L. -lstatic
eitan@eitan-Vostro-5590:~/git/system_programming/Multy_instance_glob$ ./a.out

static
The address of the global variable is: 0x5599e5967010
Dynamic library 1:
The address of the global variable is: 0x7f96efde8030
Dynamic library 2:
The address of the global variable is: 0x7f96efde8030

















*/

