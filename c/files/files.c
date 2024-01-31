
#include <stdio.h> /*for prints*/

#define  EOF (-1) 
#define SIZE_STRING 80

typedef void (*func_pointer)(int);
typedef struct print_me
{
	int num;
	func_pointer ptr;
}print_me;

void Print(int num);
void PrintMe();

int main()
{
	PrintMe();
	
	return 0;
}

void Print(int num)
{
	printf("%d\n", num);
}

void PrintMe()
{
	size_t i;
	print_me arr[10];
	
	for (i = 0; i < 10; ++i)
	{
		(arr + i) -> num = i*i;
		(arr + i) -> ptr = Print;
		(arr + i) -> ptr((arr + i) -> num);
	}
}
