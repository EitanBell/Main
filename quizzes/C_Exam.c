/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	             
*	Subject: C Exam				   	  *
*				                	  *
**************************************/

#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
					   			        
/*------------- Funcs -------------*/
void RunTests(void);
int FlipDigitsInNum(int number);
unsigned char MirrorBitsInByte(unsigned char byte);
int FlipBit(int val, unsigned int n);
size_t CountSetBits(unsigned char val);
char RotateLeft(char byte, unsigned int nbits);
void SwapPointers(int **p1, int **p2);
size_t StrLen(const char *string);
int StrCmp(const char *string1, const char *string2);
char *StrCpy(char *str1, const char *str2);
char *StrNCpy(char *str1, const char *str2, size_t n);
char *StrCat(char *str1, const char *str2);
int IterFibonacci(int element_index);
char *IntToString(char *dest, int num);
size_t MultiIntByEight(int num);
void SwapWithXor(int *a, int *b);
void SwapWithSum(int *a, int *b);
void SwapWithTemp(int *a, int *b);

/*------------- Tests -------------*/
int FlipDigitsInNumTest(void);
int MirrorBitsInByteTest(void);
int FlipBitTest(void);
int CountSetBitsTest(void);
int RotateLeftTest(void);
int StrLenTest(void);
int StrCmpTest(void);
int StrCpyTest(void);
int StrNCpyTest(void);
int StrCatTest(void);
int IterFibonacciTest(void);
int MultiIntByEightTest(void);
int SwapWithSumTest(void);
int SwapWithXorTest(void);
int SwapWithTempTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n\t****************** TESTS BEGIN ******************\n\n");
	
	printf("\t\tFlipDigitsInNum   \t%s\n", TEST_RES(FlipDigitsInNumTest()));
	printf("\t\tMirrorBitsInByte  \t%s\n", TEST_RES(MirrorBitsInByteTest()));
	printf("\t\tFlipBit   		%s\n", TEST_RES(FlipBitTest()));
	printf("\t\tCountSetBits      \t%s\n", TEST_RES(CountSetBitsTest()));
	printf("\t\tRotateLeft        \t%s\n", TEST_RES(RotateLeftTest()));
	printf("\t\tStrLen     		%s\n", TEST_RES(StrLenTest()));
	printf("\t\tStrCmp            \t%s\n", TEST_RES(StrCmpTest()));
	printf("\t\tStrCpy  		%s\n", TEST_RES(StrCpyTest()));
	printf("\t\tStrNCpy  	      \t%s\n", TEST_RES(StrNCpyTest()));
	printf("\t\tIterFibonacci     \t%s\n", TEST_RES(IterFibonacciTest()));
	printf("\t\tMultiIntByEight   \t%s\n", TEST_RES(MultiIntByEightTest()));
	printf("\t\tSwapWithXor   	  \t%s\n", TEST_RES(SwapWithXorTest()));
	printf("\t\tSwapWithSum   	  \t%s\n", TEST_RES(SwapWithSumTest()));
	printf("\t\tSwapWithTemp  	  \t%s\n", TEST_RES(SwapWithTempTest()));
		
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
}
					   			        
/*----------- question 1 -----------*/
int FlipDigitsInNum(int number)
{
	int res = 0;
	int last_digit = 0;
	int num = 0;
	
	num = number;
	
	while (0 != num)
	{
		last_digit = num % 10;
		num /= 10;
		res *= 10;
		res += last_digit;
	}
	
	return ((number < 0) ? -1 * res : res);
}

/*----------- question 4 -----------*/
unsigned char MirrorBitsInByte(unsigned char byte)
{
	size_t i = 0;
	unsigned char byte_mirror = 0;
	
	for (i = 0; i < CHAR_BIT; ++i)
	{
		byte_mirror += byte & 1;
		byte >>= 1;
		byte_mirror <<= 1;
	}
	
    return byte_mirror;
}

/*----------- question 5 -----------*/
int FlipBit(int val, unsigned int n)
{
	return (1 << n) ^ val;
}

/*----------- question 6 -----------*/
size_t CountSetBits(unsigned char val)
{
	size_t counter = 0;
	
	while (val)
	{
		val &= (val - 1);
		++counter;
	}
	
	return counter;
}

/*----------- question 8 -----------*/
char RotateLeft(char byte, unsigned int nbits)
{
	nbits &= (CHAR_BIT - 1);
	
	return ((byte << nbits) | (byte >> (CHAR_BIT - nbits)));
}

/*----------- question 10 -----------*/
void SwapPointers(int **p1, int **p2)
{
	int *tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

/*----------- question 11 -----------*/
size_t StrLen(const char *string)
{
	size_t counter = 0;
	const char *str = NULL;
	
	assert(NULL != string);
	
	str = string;
	
	while ('\0' != *str)
	{
		++counter;
		++str;
	}
	
	return counter;
}

int StrCmp(const char *s1, const char *s2)
{
	const char *runer1 = s1;
	const char *runer2 = s2;
	
	while (*runer1 == *runer2 && '\0' != *runer1)
	{
		++runer1;
		++runer2;
	}
	
	return (*runer1 - *runer2);
}

char *StrCpy(char *dest, const char *src)
{
	char *dest_run = dest;
	const char *src_run = src;
	
	while ('\0' != *src_run)
	{
		*dest_run = *src_run;
		++dest_run;
		++src_run;
	}
	*dest_run = '\0';
	
	return dest;
}

char *StrNCpy(char *str1, const char *str2, size_t n)
{
	size_t i = 0;
	char *dest = NULL;
	const char *src = NULL;
	
	assert(NULL != str1);
	assert(NULL != str2);
	
	dest = str1;
	src = str2;
	
	if (n > StrLen(str1))
	{
		n = StrLen(str1);
	}
	
	for (i = 0; i < n; ++i)
	{
		*dest = *src;
		++dest;
		++src;
	}
	
	return str1;
}

char *StrCat(char *str1, const char *str2)
{
	char *dest = NULL;
	const char *src = NULL;
	
	assert(NULL != str1);
	assert(NULL != str2);
	
	dest = str1 + StrLen(str1) - 1;
	src = str2;
	
	while ('\0' != *src)
	{
		*dest = *src;
		++dest;
		++src;
	}
	*dest = '\0';
	
	return str1;
}


/*----------- question 12 -----------*/
int IterFibonacci(int element_index)
{
	int num = 1;
	int prev_num = 0;
	int temp = 0;
	int i = 0;
	
	assert(0 < element_index);
	
	for (i = 0; i < element_index; ++i)
	{
		temp = prev_num;
		prev_num = num;
		num += temp;
	}
	return prev_num;
}

/*----------- question 13 -----------*/
char *IntToString(char *dest, int num)
{
	assert(NULL != dest);
	
	sprintf(dest, "%d", num);
	return dest;
}

/*----------- question 14 -----------*/
size_t MultiIntByEight(int num)
{
	return num << 3;
}

/*----------- question 15 -----------*/

void SwapWithXor(int *a, int *b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

void SwapWithSum(int *a, int *b)
{
	assert(NULL != a);
	assert(NULL != b);
	
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void SwapWithTemp(int *a, int *b)
{
	int temp = 0;
	
	assert(NULL != a);
	assert(NULL != b);
			
	temp = *a;
	*a = *b;
	*b = temp;
}


/*--------------------------------- TESTS ------------------------------------*/

int FlipDigitsInNumTest(void)
{
	if (12345 == FlipDigitsInNum(54321) && 12 == FlipDigitsInNum(21) 
		&& 37 == FlipDigitsInNum(73))
	{
		return 0;
	}
		return 1;
}

int MirrorBitsInByteTest(void)
{
	if ('a' == MirrorBitsInByte('W'))
	{
		return 1;
	}
	return 0;
}

int FlipBitTest(void)
{
	if (0 == FlipBit(8, 3) && 1 == FlipBit(0, 0) && 12 == FlipBit(8, 2))
	{
		return 0;
	}
	return 1;
}

int CountSetBitsTest(void)
{
	if (3 == CountSetBits(11) && 1 == CountSetBits(4) 
		&& 4 == CountSetBits(0x0f))
	{
		return 0;
	}
	return 1;
}

int RotateLeftTest(void)
{
	if (8 == RotateLeft(1, 3) && 1 == RotateLeft(64, 2))
	{
		return 0;
	}
	return 1;
}

int StrLenTest(void)
{
	if (3 == StrLen("Let") && 2 == StrLen("It") && 9 == StrLen("Mccartney"))
	{
		return 0;
	}
	return 1;
}

int StrCmpTest(void)
{
	char s1[] = {'1', '2', '3'};
	char s2[] = {'1', '2', '3', '4'};
	
	if (0 != StrCmp(s1, s2) && 0 == StrCmp(s1, s1) && 0 != StrCmp("4Baboker", "4Babokerr"))
	{
		return 0;
	}
	return 1;

}

int StrCpyTest(void)
{
	char *str1 = "yes";
	char str2[] = {'1', '2', '3'};
	
	if (!StrCmp(StrCpy(str2, str1), str1))
	{
		return 0;
	}
	return 1;
}


int StrNCpyTest(void)
{
	char str1[] = {'A', 'n', 'i', '\0'};
	char str2[] = {'1', '2', '3', 'T', 'o', 'L', 'i', 's', 'h', 'n', '\0'};
	char str3[] = {'A', 'n', 'i', 'T', 'o', 'L', 'i', 's', 'h', 'n', '\0'};
		
	if (!StrCmp(str3, StrNCpy(str2, str1, 3)))
	{
		return 0;
	}
	return 1;
}

int StrCatTest(void)
{
	char str1[11] = {'A', 'n', 'i', '\0'};
	char str2[] = {'T', 'o', 'L', 'i', 's', 'h', 'n', '\0'};
	char str3[] = {'A', 'n', 'i', 'T', 'o', 'L', 'i', 's', 'h', 'n', '\0'};
		
	if (!StrCmp(str3, StrCat(str1, str2)))
	{
		return 0;
	}
	return 1;
}

int IterFibonacciTest(void)
{
	if (1 == IterFibonacci(1) && 2 == IterFibonacci(3) 
		&& 8 == IterFibonacci(6) && 610 == IterFibonacci(15))
	{
		return 0;
	}
	return 1;
}

int MultiIntByEightTest(void)
{
	if (8 == MultiIntByEight(1) && 16 == MultiIntByEight(2) 
		&& 128 == MultiIntByEight(16))
	{
		return 0;
	}
	return 1;
} 

int SwapWithSumTest(void)
{
	int a = 7;
	int b = 3;
	
	SwapWithSum(&a, &b);
	
	if (3 == a && 7 == b)
	{
		return 0;
	}
	return 1;
}

int SwapWithXorTest(void)
{
	int a = 7;
	int b = 3;
	
	SwapWithXor(&a, &b);
	
	if (3 == a && 7 == b)
	{
		return 0;
	}
	return 1;
}

int SwapWithTempTest(void)
{
	int a = 7;
	int b = 3;
	
	SwapWithTemp(&a, &b);
	
	if (3 == a && 7 == b)
	{
		return 0;
	}
	return 1;
}

