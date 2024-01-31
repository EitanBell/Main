/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad       *
*	Date: 22/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: atoi & itoa         *
*				     *
**************************************/
#include <stdio.h>	/* printf */	

#include "atit.h"	


void AtoiTest(void);
void ItoaTest(void);
void IsLittleEndianTest(void);
void TestItoaAnyBase(void);
void TestPrintLetters(void);
void TestAtoi(void);
	
int main(void)
{
	TestAtoi();
	TestItoaAnyBase();
	ItoaTest();
	IsLittleEndianTest();
	TestPrintLetters();
	
	return 0;
}


void ItoaTest(void)
{
	int num = -54321;
	char str[7] = {0};
	int num1 = -375;
	int num2 = 44;
	int num3 = 8;
	char str1[4] = {0};
	char str2[5] = {0};
	char str3[7] = {0};
	
	printf("\n------------ItoaTest------------\n");
	Itoa(num, str, 10);
	printf("%s\n", str);
	
	Itoa(num1, str1, 10);
	printf("%s\n", str1);
	
	Itoa(num2, str2, 16);
	printf("%x\n", num2);
	printf("%s\n", str2);
	
	Itoa(num3, str3, 2);
	printf("%s\n", str3);
	
}


void IsLittleEndianTest(void)
{
	printf("\n------------IsLittleEndianTest------------\n");
	if (1 == IsLittelEndian())
	{
		printf("This computer works with Little Endian system.\n");
	}
}



void TestItoaAnyBase(void)
{
	char str1[10] = {'\0'};
	char str2[10] = {'\0'};
	char str3[10] = {'\0'};
	char str4[10] = {'\0'};
	char str5[10] = {'\0'};
	char str6[10] = {'\0'};
	char str7[10] = {'\0'};
	
	printf("\n------------TestItoaAnyBase------------\n");
	printf("127 base 2: %s\n", Itoa(127, str1, 2));
	printf("64 base 8: %s\n", Itoa(64, str2, 8));
	printf("12345 base 10: %s\n", Itoa(12345, str3, 10));
	printf("-4 base 10: %s\n", Itoa(-4, str4, 10));
	printf("64 base 16: %s\n", Itoa(64, str5, 16));
	printf("100 base 32: %s\n", Itoa(100, str6, 32));
	printf("9876 base 36: %s\n", Itoa(9876, str7, 36));
}

void TestAtoi(void)
{
	char str2[] = "654";
	char str8[] = "80";
	char str10[] = "12345";
	char str16[] = "64";
	char str32[] = "900";
	
	printf("\n------------TestAtoi------------\n");
	printf("654 Base 2: %d\n",Atoi(str2, 2));
	printf("80 Base 8: %d\n",Atoi(str8, 8));
	printf("12345 Base 10: %d\n",Atoi(str10, 10));
	printf("64 Base 16: %d\n",Atoi(str16, 16));
	printf("900 Base 32: %d\n",Atoi(str32, 32));
}


void TestPrintLetters(void)
{
	printf("\n------------TestPrintLetters------------\n");
	printf("Should print 'a', 'f':\n");
	PrintLetters("aabbddeeffgg","abgggf","ebgggggzzz", 12, 6, 10);
}

