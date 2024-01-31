/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad    	     *
*	Date: 01/12/22	             *
*	Valgrind & GDB passed        *
*	Subject: Bit_array	     *
*				     *
**************************************/

#include <stdio.h> /* prinft */	

#include "bit_array.h" /* my stack */

void BitArrayTests(void);
void BitArrSetOnTest(void);
void BitArrSetOffTest(void);
void BitArrSetAllTest(void);
void BitArrUnsetAllTest(void);
void BitArrFlipBitTest(void);
void BitArrGetValTest(void);
void BitArrSetValTest(void);
void BitArrCountSetBitsHammingTest(void);
void BitArrCountSetBitsTest(void);
void BitArrFlipAllTest(void);
void BitArrRotateLeftTest(void);
void BitArrRotateRightTest(void);
void BitArrToStringTest(void);
void BitArrMirrorTest(void);
void BitArrCountOffBitsTest(void);
void BitArrMirrorO1Test(void);
void BitArrCountSetBitsLutTest(void);
void BitArrMirrorLutTest(void);

int main(void)
{
	BitArrayTests();
	
	return 0;
}


void BitArrayTests(void)
{
	/*char buffer[64] = {0};
	printf("The bits arr are:\n%s\n", BitArrToString(arr, buffer));
	*/
	printf("\n-------- Bit Array Tests --------\n\n");
	BitArrSetOnTest();
	BitArrSetOffTest();	
	BitArrSetAllTest();
	BitArrUnsetAllTest();
	BitArrFlipBitTest();
	BitArrGetValTest();
	BitArrSetValTest();
	BitArrCountSetBitsHammingTest();/*not working yet*/
	BitArrCountSetBitsTest();
	BitArrFlipAllTest();
	BitArrRotateLeftTest();
	BitArrRotateRightTest();
	BitArrMirrorTest();
	BitArrCountOffBitsTest();
	BitArrMirrorO1Test();
	BitArrCountSetBitsLutTest();
	BitArrMirrorLutTest();
		
	BitArrToStringTest();
	
	
	printf("\n");
	
	
	
}

void BitArrSetOnTest(void)
{
	bit_array_t arr = 0;
	arr = BitArrSetOn(arr, 63);
	arr = BitArrSetOn(arr, 24);
	arr = BitArrSetOn(arr, 0);
	
	if ((((arr >> 63) & 1) == 1) &&
	   (((arr >> 24) & 1) == 1) &&
	   (((arr >> 0) & 1) == 1))
	{
		printf("BitArrSetOn test result: PASSED\n");
	}	
}
 
void BitArrSetOffTest(void)
{
	bit_array_t arr = 0;
	arr = BitArrSetOn(arr, 63);
	arr = BitArrSetOff(arr, 63);
	arr = BitArrSetOn(arr, 24);
	arr = BitArrSetOff(arr, 24);
	arr = BitArrSetOn(arr, 0);
	arr = BitArrSetOff(arr, 0);
	
	if (0 == arr)
	{
		printf("BitArrSetOff test result: PASSED\n");
	}
}

void BitArrSetAllTest(void)
{
	bit_array_t arr = 0;

	arr = BitArrSetAll(arr);
	if ((((arr >> 63) & 1) == 1) &&
	   (((arr >> 52) & 1) == 1) &&
	   (((arr >> 0) & 1) == 1))
	{
		printf("BitArrSetAll test result: PASSED\n");
	}
}

void BitArrUnsetAllTest(void)
{
	bit_array_t arr = 0;

	arr = BitArrUnsetAll(arr);
	if ((((arr >> 63) & 1) == 0) &&
	   (((arr >> 28) & 1) == 0) &&
	   (((arr >> 0) & 1) == 0))
	{
		printf("BitArrUnsetAll test result: PASSED\n");
	}
	
}

void BitArrFlipBitTest(void)
{
	bit_array_t arr = 0;
		
	arr = BitArrFlipBit(arr, 7);
	arr = BitArrFlipBit(arr, 63);
	arr = BitArrFlipBit(arr, 49);
	arr = BitArrFlipBit(arr, 0);
	arr = BitArrFlipBit(arr, 11);
	arr = BitArrFlipBit(arr, 7);
	arr = BitArrFlipBit(arr, 63);

	if ((((arr >> 7) & 1) == 0) &&
	   (((arr >> 63) & 1) == 0) &&
	   (((arr >> 49) & 1) == 1) &&
	   (((arr >> 0) & 1) == 1) &&
	   (((arr >> 11) & 1) == 1))
	{
		printf("BitArrFlipBit test result: PASSED\n");
		
	}
}

void BitArrGetValTest(void)
{
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	int num4 = 0;
	int num5 = 0;
	int num6 = 0;
	
	bit_array_t arr = 266500;

	num1 = BitArrGetVal(arr, 2);
	num2 = BitArrGetVal(arr, 8);
	num3 = BitArrGetVal(arr, 12);
	num4 = BitArrGetVal(arr, 18);
	num5 = BitArrGetVal(arr, 63);
	num6 = BitArrGetVal(arr, 33);
		
		
	if (num1 == 1 && num2 == 1 && num3 == 1 && 
	    num4 == 1 && num5 == 0 && num6 == 0)
	{
		printf("BitArrGetVal test result: PASSED\n");
	}
}

void BitArrSetValTest(void)
{
	bit_array_t arr = 0;
		
	arr = BitArrSetVal(arr, 7, 1);
	arr = BitArrSetVal(arr, 63, 0);
	arr = BitArrSetVal(arr, 49, 1);
	arr = BitArrSetVal(arr, 0, 0);
	arr = BitArrSetVal(arr, 11, 1);

	if ((((arr >> 7) & 1) == 1) &&
	   (((arr >> 63) & 1) == 0) &&
	   (((arr >> 49) & 1) == 1) &&
	   (((arr >> 0) & 1) == 0) &&
	   (((arr >> 11) & 1) == 1))
	{
		printf("BitArrSetVal test result: PASSED\n");
	}
}

void BitArrCountSetBitsHammingTest(void)
{
	bit_array_t arr1 = 0;
	bit_array_t arr2 = 7;
	bit_array_t arr3 = 25963475;
	bit_array_t arr4 = 0;
	arr4 = BitArrSetAll(arr4);
	
	if (BitArrCountSetBitsHamming(arr1) == 0 && 
	    BitArrCountSetBitsHamming(arr2) == 3 && 
	    BitArrCountSetBitsHamming(arr3) == 13 && 
	    BitArrCountSetBitsHamming(arr4) == 64)
	{
		printf("BitArrCountSetBitsHamming test result: PASSED\n");
	}
}
void BitArrCountSetBitsTest(void)
{
	bit_array_t arr1 = 0;
	bit_array_t arr2 = 7;
	bit_array_t arr3 = 25963475;
	bit_array_t arr4 = 0;
	
	arr4 = BitArrSetAll(arr4);
	
	if (BitArrCountSetBits(arr1) == 0 && 
	    BitArrCountSetBits(arr2) == 3 && 
	    BitArrCountSetBits(arr3) == 13 && 
	    BitArrCountSetBits(arr4) == 64)
	{
		printf("BitArrCountSetBits test result: PASSED\n");
	}
}

void BitArrFlipAllTest(void)
{
	bit_array_t arr1 = 7;
	bit_array_t arr2 = BitArrFlipAll(BitArrFlipAll(arr1));
	
	if (arr1 == arr2)
	{
		printf("BitArrFlipAll test result: PASSED\n");
	}
}

void BitArrRotateLeftTest(void)
{
	bit_array_t arr1 = 1;
	bit_array_t arr2 = 0;
	
	arr2 = BitArrSetAll(arr2) >> 63;
	arr1 = BitArrRotateLeft(arr1, 4);
	arr2 = BitArrRotateLeft(arr2, 4);
	
	if (arr1 == 16 && arr2 == 16)
	{
		printf("BitArrRotateLeft test result: PASSED\n");
	}
}

void BitArrRotateRightTest(void)
{
	bit_array_t arr1 = 1;
	bit_array_t arr2 = 7;

	arr1 = BitArrRotateRight(arr1, 60);
	arr2 = BitArrRotateRight(arr2, 63);
	
	if (arr1 == 16 && arr2 == 14)
	{
		printf("BitArrRotateRight test result: PASSED\n");
	}
}

void BitArrToStringTest(void)
{
	bit_array_t arr1 = 0;
	bit_array_t arr2 = 7;
	bit_array_t arr3 = 105;
	bit_array_t arr4 = 5278844;
	bit_array_t arr5 = 9223372036854775807;
	char buffer[64] = {0};
	
	printf("\n---- BitArrToString Tests ----\n");	
	printf("0, 7, 105, 5278844, 9223372036854775807 are:\n");
	printf("%s\n", BitArrToString(arr1, buffer));	
	printf("%s\n", BitArrToString(arr2, buffer));		
	printf("%s\n", BitArrToString(arr3, buffer));	
	printf("%s\n", BitArrToString(arr4, buffer));
	printf("%s\n", BitArrToString(arr5, buffer));
}

void BitArrMirrorTest(void)
{
	bit_array_t arr1 = 7;
	bit_array_t arr2 = 0;
	
	arr2 = BitArrMirror(BitArrMirror(arr1));
	if (arr1 == arr2)
	{
		printf("BitArrMirror test result: PASSED\n");
	}
}

void BitArrCountOffBitsTest(void)
{
	bit_array_t arr1 = 0;
	bit_array_t arr2 = 7;
	bit_array_t arr3 = 9223372036875807;
	bit_array_t arr4 = 0;
	
	arr4 = BitArrSetAll(arr4);
	
	if (BitArrCountOffBits(arr1) == 64 && 
	    BitArrCountOffBits(arr2) == 61 && 
	    BitArrCountOffBits(arr3) == 37 && 
	    BitArrCountOffBits(arr4) == 0)
	{
		printf("BitArrCountOffBits test result: PASSED\n");
	}
}

void BitArrMirrorO1Test(void)
{
	bit_array_t arr1 = 7;
	bit_array_t arr2 = 0;
	
	arr2 = BitArrMirrorO1(BitArrMirrorO1(arr1));
	if (arr1 == arr2)
	{
		printf("BitArrMirrorO1 test result: PASSED\n");
	}
}

void BitArrCountSetBitsLutTest(void)
{
	bit_array_t arr1 = 0;
	bit_array_t arr2 = 7;
	bit_array_t arr3 = 25963475;
	bit_array_t arr4 = 0;
	
	arr4 = BitArrSetAll(arr4);
	
	if (BitArrCountSetBitsLut(arr1) == 0 && 
	    BitArrCountSetBitsLut(arr2) == 3 && 
	    BitArrCountSetBitsLut(arr3) == 13 && 
	    BitArrCountSetBitsLut(arr4) == 64)
	{
		printf("BitArrCountSetBitsLut test result: PASSED\n");
	}
}

void BitArrMirrorLutTest(void)
{
	bit_array_t arr1 = 7;
	bit_array_t arr2 = 0;
	
	arr2 = BitArrMirrorLut(BitArrMirrorLut(arr1));
	if (arr1 == arr2)
	{
		printf("BitArrMirrorLut test result: PASSED\n");
	}
}


