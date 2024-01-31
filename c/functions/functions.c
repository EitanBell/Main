#include <stdio.h>
#include <stdlib.h>

#define ESC_VALUE 27
typedef void (*functions)(void);

void IfATEsc();
void SwitchATEsc();
void LutATEsc();
void DoNothing(void);
void PrintA(void);
void PrintT(void);
void EscFunc(void);

int main()
{
	/*IfATEsc();*/
	SwitchATEsc();
	/*LutATEsc();*/
	
	return 0;
}

void IfATEsc()
{
	char input = '\0';
	
	printf("Please enter A, T.\nOr Esc to exit\n");
	system("stty -icanon -echo");
	
	while (ESC_VALUE != (input = getchar()))
	{
		if ('A' == input)
		{
			printf("A-pressed\n\n");
		}	
		else if ('T' == input)
		{
			printf("T-pressed\n\n");
		}
		else if (ESC_VALUE == input)
		{
			exit(0);
		}
	}
	system("stty icanon echo");
}	

void SwitchATEsc()
{
	char input = '\0';
	
	system("stty -icanon -echo");
	printf("Please enter A, T.\nOr Esc to exit\n");
	
	while (ESC_VALUE != (input = getchar()))
	{
		switch(input)
		{
			case 'A':
				printf("A-pressed\n\n");
				break;
			case 'T':
				printf("T-pressed\n\n");
				break;
			case ESC_VALUE:
				exit(0);
				break;
		}
	}
	system("stty icanon echo");	
}

void LutATEsc()
{
	int i = 0;
	functions lut[256] = {NULL};
	
	printf("Please enter A, T.\nOr Esc to exit\n");
	system("stty -icanon -echo");
	
	for (i = 0; i < 256; ++i)		//do define to magic number;
	{
		lut[i] = &doNothing;
	}
	
	lut['a'] = &printA;
	lut['t'] = &printT;
	lut[ESC_VALUE] = &EscFunc;
	
	while (ESC_VALUE != (input = getchar()))
	{
		lut[getchar()]();
	}
}


void DoNothing(void)
{
}
void PrintA(void)
{
	printf("A-pressed\n\n");
}
void PrintT(void)
{
	printf("T-pressed\n\n");
}
void EscFunc(void)
{
	system("stty icanon echo");
	exit(0);			//Do without EXIT!! can cause exit before dinamic allovcation. Use flag;
}

