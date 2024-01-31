                                                                                                                                                                                 
/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer: Aviv Tal           *
*	Date: 17/11/22	             *
*	Bitwise worksheet            *
*				     *
**************************************/

#include <stdio.h>

long pow2(unsigned int x, unsigned int y);
int IsPowOfTwoLoop(unsigned int x);
int IsPowOfTwoNoLoop(unsigned int x);
int AddOneNoArithmetic(unsigned int x);
void PrintOnlyThreeBitInArr(unsigned int *arr, int size);

unsigned char ByteMirror(unsigned char num);
unsigned char TwoAndSixAreOn(unsigned char x);
unsigned char TwoOrSixAreOn(unsigned char x);
unsigned char Swap5And3Bits(unsigned char x);
unsigned char ClosestDevidedBy16(unsigned char x);
void SwapNoTemp(unsigned char *x, unsigned char *y);
int CountOnesInInteger(int num);
int bit_value(int num, int bits_to_move);
void PrintFloatBits(float *num);

#define BYTE_SIZE (8)
#define NULL_TER (1)
#define INT_BITS_SIZE (32)

int main()
{
	/*int x = 512, y = 3;*/
	
/*	printf("%ld\n", pow2(x,y));
	printf("%d\n", IsPowOfTwoLoop(x));
	printf("%d\n", IsPowOfTwoNoLoop(x));
	printf("%d\n", AddOneNoArithmetic(x));
	PrintThreeBitInArr(arr, size);*/\
	
	/*printf("--- %c ---\n", *(arr + BYTE_SIZE - NULL_TER));*/
	
	/*int check = 1;			///------Mirror check------////
	check = ByteMirror(check);
	printf("The mirror number is: %d\n", check);*/
	
	/*printf("The original number is: %s\n", arr);
	arr = ByteMirror(arr);
	printf("The mirrored number is: %s\n", arr);*/
	
	/*unsigned char ch = 164;		//------Swap5And3Bits------//
	printf("%d\n", Swap5And3Bits(ch));*/
	
	/*unsigned char ch = 4;			//-------ClosestDevidedBy16-------//
	printf("%d\n", ClosestDevidedBy16(ch));*/
	
	/*unsigned char ch1 = 164, ch2 = 53;	//-------SwapNoTemp---------//
	printf("Before swap: char1 = %d, char2 = %d \n", ch1, ch2);
	SwapNoTemp(&ch1, &ch2);
	printf("After swap: char1 = %d, char2 = %d \n", ch1, ch2);*/

	/*int num = 8174685, ones_count = 0;	//--------CountOnesInInteger---------//
	ones_count = CountOnesInInteger(num);
	printf("There are %d bits turned on at number: %d\n", ones_count, num);*/
	
	float num1 = 12;			/*//--------PrintFloatBits----------//*/
	float num2 = 11.99;
	float num3 = 1;
	float num4 = 1.1;
	float num5 = 99.99;
	
	PrintFloatBits(&num1);
	PrintFloatBits(&num2);
	PrintFloatBits(&num3);
	PrintFloatBits(&num4);
	PrintFloatBits(&num5);

	return 0;
}

long pow2(unsigned int x, unsigned int y)		
{
	return x << y;
}

int IsPowOfTwoLoop(unsigned int x)
{
	int x_copy = x;
	int counter = 0;
	
	while (0 != x_copy)
	{
		if (1 & x_copy)
		{
			++counter;	
		}
		x_copy = x_copy >> 1;
	} 
	if (1 == counter)
	{
		return 1;
	}
	return 0;
}

int IsPowOfTwoNoLoop(unsigned int x)
{
	return (!(x & (x - 1)));
}

int AddOneNoArithmetic(unsigned int x)
{
	int m = 1;

	while(x & m)
	{
		x = x ^ m;
		m <<= 1;
	}
	x = x ^ m;
	return x;
}

void PrintOnlyThreeBitInArr(unsigned int *arr, int size) 
{
	int counter = 0;
	int i = 0;
	int num_copy = 0;
	
	for (i = 0; i < size; ++i)
	{
		num_copy = *(arr + i);
		while (0 != num_copy)
		{
			counter += num_copy & 1;
			num_copy = num_copy >> 1;
		}
		if (3 == counter)
		{
			printf("%d\n", *(arr + i));
		}
		counter = 0;
	}
}


unsigned char ByteMirror(unsigned char num)
{	
	char mirrored = 0;
	int i = 0;
	
	for(i = 0; i < BYTE_SIZE; ++i) 
	{
		mirrored = mirrored << 1;
		mirrored = mirrored | (1 & num);
		num = num >> 1;
	}

	return result;
}

unsigned char TwoAndSixAreOn(unsigned char x)
{
	return ((1 & (x >> 1)) & (1 & (x >> 5)));
}

unsigned char TwoOrSixAreOn(unsigned char x)
{
	return ((1 & (x >> 1)) | (1 & (x >> 5)));
}

unsigned char Swap5And3Bits(unsigned char x)
{
	char tmp3 = 0, tmp5 = 0;
	char num = 0;
	
	num = x;
	tmp3 = 1 & (x >> 2);
	tmp5 = 1 & (x >> 4); 
	
	num = (num & ~(1 << 4)) | (tmp3 << 4);
	num = (num & ~(1 << 2)) | (tmp5 << 2);
	
	return num;
}

unsigned char ClosestDevidedBy16(unsigned char x)
{
	return ((x >> 4) << 4);
}

void SwapNoTemp(unsigned char *x, unsigned char *y)
{
	*x = *x ^ *y;
	*y = *x ^ *y;
	*x = *x ^ *y;
	
}
/*Question asnwer: It won't success if the sum of both numbers is bigger then the value of 11111111, 
  because there is no presentation in one byte. */
  
int CountOnesInInteger(int num)
{
	int counter = 0, i = 0;
	int copy_num = 0;
	
	copy_num = num;
	
	for (i = 0; i < INT_BITS_SIZE; ++i)
	{
		if (1 & copy_num)
		{
			++counter;
		}
		copy_num = copy_num >> 1;
	}
	return counter;
}

void PrintFloatBits(float *num)
{
	int i = 0;
	int *p_num = (int *)num;
	
	printf("The binary value of %.5f is: ", *num);
	
	for (i = ((sizeof(float) * BYTE_SIZE) - NULL_TER); 0 <= i; --i)
	{
		printf("%d", (((*p_num) >> i) & 1));
	}
	printf("\n");
}

