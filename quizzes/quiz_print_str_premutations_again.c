/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 02/03/23	                  *
*	Subject: PrintStrPermutations	  *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <string.h> /* strlen */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

void Test(void);
void PrintStrPermutations(char *str);
static void RecPermutations(char *str, int left_inx, int right_inx);
static void Swap(char *x, char *y);

int main (void)
{
	printf("\n******************** TESTS BEGIN ********************\n\n");
	
	printf("PrintStrPermutations Tests: \n\n");
	Test();
	
	printf("\n\n******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

void Test(void)
{
	char str1[] = "ABC"; 
	char str2[] = "Eitan"; 
	
	PrintStrPermutations(str1);
	printf("\n\n");
	PrintStrPermutations(str2);
}

void PrintStrPermutations(char *str)
{
	int left_inx = 0;
	int right_inx = strlen(str) - 1;
	
	assert(NULL != str);
	
	RecPermutations(str, left_inx, right_inx);
}

static void RecPermutations(char *str, int left_inx, int right_inx)
{
	int i = 0;
	
	if (left_inx == right_inx)
	{
		printf("%s, ", str);
	}
	else
	{
		for (i = left_inx; i <= right_inx; ++i)
		{
			Swap(str + left_inx, str + i);
			
			RecPermutations(str, left_inx + 1, right_inx);
			
			Swap(str + left_inx, str + i);
		}
	}
}

static void Swap(char *x, char *y)
{
	char temp = 0;
	
	assert(NULL != x);
	assert(NULL != y);
			
	temp = *x;
	*x = *y;
	*y = temp;
}


