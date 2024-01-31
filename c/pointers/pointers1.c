/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Tal               *
*	Date: 18/11/22	             *
*	valgrind OK    		     *
*				     *
**************************************/


#include <stdio.h>/*for printf tests*/
#include <stdlib.h>/*for malloc*/
#include <assert.h>/*for asserts*/

void Swap(int*, int*);		
int *CopyArray(int *arr, int size);	
void SwapSizeT(size_t *, size_t *);		
void SwapTwo(size_t **a, size_t **b);


int main()
{
	/*----------- QUESTION 1 ------------*/
	
	int a = 2, b = 123456;
	int size, i;
	int *copy_array = NULL;
	static int s_i = 7;
	size_t x = 2, y = 123456;
	int *array = NULL;
	int j;
	int *ptr = NULL;
	int *ptr2 = NULL;
	int **ptr3 = NULL;
	size_t c, d;				
	size_t *ptr1 = NULL, *ptr4 = NULL;
	
	
	printf("Before swap: a = %d, b = %d\n", a, b);
	Swap(&a, &b);
	printf("After swap: a = %d, b = %d\n", a, b);
	
		
	/*----------- QUESTION 2 ------------*/
	
	printf("Please enter array's size:\n");
	scanf("%d", &size);
	
	
	
	array = (int*)malloc(size * sizeof(int));
	
	for(i = 0; i < size; ++i)
	{
		scanf("%d", &array[i]);
	}
	
	copy_array = CopyArray(array, size);
	free(array);
	
	for(i = 0; i < size; i++)
	{
		printf("%d ", *(copy_array + i));
	}
	
	free(copy_array);	
		
	/*----------- QUESTION 3 ------------*/
	
	printf("%p \n", (void *)&s_i);
	j = 7;
	
	printf("%p \n", (void *)&j);
	ptr = (void *)&j;
	
	printf("%p \n", (void *)ptr);
	ptr2 = (int *)malloc(sizeof(int));
	
	printf("%p \n", (void *)ptr2);
	
	if(ptr)
	{
		ptr3 = (void *)&ptr;
		printf("%p \n", (void *)ptr3);
	}
	free(ptr2);
		
	/*----------- QUESTION 4 ------------*/
	
	printf("Before swap: x = %lu, y = %lu\n", x, y);/*QUESTION swap*/
	SwapSizeT(&x, &y);
	printf("After swap: x = %lu, y = %lu\n", x, y);
	
	
	c = 44;
	d = 66;				/*QUESTION swapTwo*/
	ptr1 = (void *)&c, ptr4 = (void *)&d;
	
	printf("Before swap: c = %lu, d = %lu\n", c, d);
	SwapTwo(&ptr1, &ptr4);
	printf("After swap: c = %lu, d = %lu\n", c, d);
		
		
		
	return 0;
}


void SwapSizeT(size_t *a, size_t *b)
{	
	size_t temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void SwapTwo(size_t **a, size_t **b)
{	
	SwapSizeT(*a, *b);
}

void Swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int *CopyArray(int *arr, int size)
{
	int *copy_array = (int*)malloc(size * sizeof(int));
	int i;
	
	for(i = 0; i < size; ++i)
	{
		*(copy_array + i) = *(arr + i);
	}
	
	return copy_array;
}
