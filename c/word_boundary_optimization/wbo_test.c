/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:                    *
*	Date: 23/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: word boudery opti   *
*				     *
**************************************/

#include "wbo.h"

void TestSlist(void);

int main(void)
{
	TestSlist();
	
	return 0;
}

void TestSlist(void)
{
	char string[] = {'e', 'i', 't', 'a', 'n', 'b','8', '7', '6', '5', '\0'};
	char string1[] = {'8', 'm', '6', 'm', '4', 'm', '2', 'm','3', 'm', '\0'};
	char string2[] =  {'e', 'i', 't', 'a', 'n', 'b', '\0'};
	char string3[] = {'8', '7', '6', '5', '4', '3', '2', '1', '\0'};
	char string4[] = {'8', '7', '6', '5', '4', '3', '2', '1', '\0'};
	char string5[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
	size_t n1 = 3;
	size_t n2 = 7;
	size_t n3 = 9;
	size_t n4 = 256;
	size_t *num = NULL;
	int n5 = 1024;
	int *num1 = NULL;
	char c = '$';
	
	printf("\n----------- TEST WORD BOUNDERY OPTIMIZATION ------------- \n\n");
	
	Memset((void *)string2, c, n1);
	printf("%s\n", string2);
	
	Memset((void *)string3, c, n2);
	printf("%s\n", string3);
	
	Memcpy(string1, string, n3);
	printf("%s\n", string1);
	
	Memmove(string4, string5, 4);
	printf("%s\n", string4);
	
	num = &n4;
	num1 = &n5;
	
	Memmove(num1, (const void *)num , 10);
	printf("%d\n", *num1);

	printf("\n");
	
	if (Memset((void *)string2, c, n1) == memset((void *)string2, c, n1)
	    && Memset((void *)string3, c, n2) == memset((void *)string3, c, n2))
	{
		printf("Test: Memset - SUCCEED\n");
	}
	else
	{
		printf("Test: Memset - FALIED\n");
	}
	if (Memcpy(string1, string, n3) == memcpy(string1, string, n3))
	{
		printf("Test: Memcpy - SUCCEED\n");
	}
	else
	{
		printf("Test: Memcpy - FALIED\n");
	}
	if (Memmove(string4, string5, 4) == memmove(string4, string5, 4)
	    && Memmove(num1, (const void *)num , 10) ==
		  memmove(num1, (const void *)num , 10))
	{
		printf("Test: Memmove - SUCCEED\n");
	}
	else
	{
		printf("Test: Memmove - FALIED\n");
	}


	printf("\n---------------------- END TESTS ---------------------- \n\n");
}


