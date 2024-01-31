/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad     *
*	Date: 28/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: atoi & itoa         *
*				     *
**************************************/

#include <string.h>	/* strlen */
#include <assert.h>	/* assert */
#include <stdio.h>	/* printf */	
#include "atit.h"


#define LETTERS_NUM 26
#define ASCII_SIZE 256
#define DIGIT_NUM 10
#define BASE_10 10
#define NOT_DIGIT -1

static char base_LUT[ASCII_SIZE] = {0};

static void InitBaseLUT(int base);
static void StrToHistogram(int *hist, const char *str, int len);
static int CountDigit(int num, int base);

/*****************************  ItoaBase10  **********************************/

char *ItoaBase10 (int value, char *str)
{
	return Itoa(value, str, BASE_10);
}

/*****************************  AtoiBase10  **********************************/

int AtoiBase10(const char *str)
{
	return Atoi(str, BASE_10);
}

/*****************************  Itoa  **********************************/

char *Itoa(int value , char *str, int base)
{
	int i = 0;
	
	assert(NULL != str);
	
	if (0 >= value)
	{
		*str = '-';
		
		value *= -1;

		++i;
	}
	
	if (0 == value)
	{
		*str = '0';	
	}
	
	InitBaseLUT(base);
	
	i += CountDigit(value, base);
		
	str[i] = '\0';
	--i;
	
	while (value)
	{
		str[i] = base_LUT[(value % base)];
		
		value /= base;
		
		--i;
	}
	
	return str;
}

/*****************************  Atoi  **********************************/

int Atoi(const char *str, int base)
{
	int res = 0;
	int i = 0;
	int len = 0;
	int sign = 1;
	
	assert(NULL != str);
	
	while (' ' == *str)
	{
		++str;
	}

	len = strlen(str);

	if ('-' == *str)
	{
		sign = -1;
		
		++str;
		--len;
	}
	
	InitBaseLUT(base);
	
	for (i = 0; i < len && base_LUT[(int)str[i]] != NOT_DIGIT; ++i)
	{
		res *= base;
		res += (base_LUT[(int)str[i]]);
	}
	
	return (res * sign);
}

/*****************************  InitBaseLUT  **********************************/

static void InitBaseLUT(int base)
{
	int i = 0;
	
	for (i = 0; i < ASCII_SIZE; ++i)
	{
		base_LUT[i] = NOT_DIGIT;
	}
	
	for (i = 0; i < base && i < DIGIT_NUM; ++i)
	{
		base_LUT[i] = ('0' + i);
		base_LUT['0' + i] = i;
	}
	
	for (i = 0;  i < (base - DIGIT_NUM) && i < LETTERS_NUM ; ++i)
	{
		base_LUT[i + 'A'] =(DIGIT_NUM + i);
		base_LUT[i + 'a'] =(DIGIT_NUM + i);
		base_LUT[(i + DIGIT_NUM)] = ('A' + i);
	}
}

/*****************************  PrintLetters  *********************************/

void PrintLetters(const char *str1, const char *str2, const char *str3,
					int len1, int len2, int len3)
{
	int histogram1[ASCII_SIZE] = {0};
	int histogram2[ASCII_SIZE] = {0};
	int histogram3[ASCII_SIZE] = {0};
	int i = 0;
	
	assert(NULL != str1 && NULL != str2 && NULL != str3);
	
	StrToHistogram(histogram1, str1, len1);
	StrToHistogram(histogram2, str2, len2);
	StrToHistogram(histogram3, str3, len3);
	
	for (i = 'A'; i <= 'Z'; ++i)
	{
		if (histogram1[i] && histogram2[i] && !histogram3[i])
		{
			printf("%c", i);
		}
	}

	for (i = 'a'; i <= 'z'; ++i)
	{
		if (histogram1[i] && histogram2[i] && !histogram3[i])
		{
			printf("%c", i);
		}
	}
	
	printf("\n");
}

/****************************  StrToHistogram  *******************************/

static void StrToHistogram(int *hist, const char *str, int len)
{
	int i = 0;
	
	assert(NULL != hist && NULL != str);
	
	for (i = 0; i < len; ++i, ++str)
	{
		++hist[(int)*str];
	}
}
/****************************  IsLittelEndian  *******************************/

boolian_t IsLittelEndian(void)
{
	boolian_t res = TRUE;
	short test = 1;
	
	if (*(char *)&test == 0)
	{
		res = FALSE;
	}
	
	return res;
}

/****************************  CountDigit  *******************************/

static int CountDigit(int num, int base)
{
	int i = 0;
	
	while (num)
	{
		++i;
		num /= base;
	}
	
	return i;
}

