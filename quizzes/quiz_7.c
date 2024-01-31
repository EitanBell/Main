#include <stdio.h>

int BitPairNum(unsigned int number);
void Quiz7Test();
void swapBitwise(int *c1, int *c2);
void swapPtr(int *a, int *b);
void swapNoTmp(int *x, int *y);
int CountBitsEfficient(int num);



int main()
{
	Quiz7Test();
	

	return 0;
}

int BitPairNum(unsigned int number)
{
	int counter = 0;
	unsigned int num = 0;
	
	num = number;
	
	while (num)
	{
		counter += ((num & 3) == 3);
		num >>= 1;
	}
	return counter;
}

void Quiz7Test()
{
	/*----QUESTION 1----*/
	unsigned int n = 118;
	int pairs_count = 0;
	
	pairs_count = BitPairNum(n);
	printf("Numbers of pairs in %d, is: %d\n", n, pairs_count);
	
	/*----QUESTION 3----*/
	int num = 96;
	int counter = 0;
	
	counter = CountBitsEfficient(num);
	printf("The number of bits in %d is %d\n", num, counter);
	
}

void swapBitwise(int *c1, int *c2)
{
	int *x = NULL;
	int *y = NULL;
	
	x = c1;
	y = c2;
	
	*x = *x ^ *y;
	*y = *x ^ *y;
	*x = *x ^ *y;
}

void swapPtr(int *a, int *b)
{
	int temp = 0;
	
	temp = *a;
	*a = *b;
	*b = temp;
}

void swapNoTmp(int *a, int *b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

int CountBitsEfficient(int num)
{
	int counter = 0;
	
	while (num)
	{
		num &= num - 1;
		++counter;
	}
	return counter;
}



















