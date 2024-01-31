/*   includes & macros   */

#include <stdio.h>	/* printf */

#include "test_help.h" /* Test Help Header File */

/*   Print Functions   */

void PrintTestTitle(char *tests_name)
{
	printf("\n\n\033[1;4m%s Tests:\033[m\n", tests_name);
}

void PrintTestStat(int stat, char *tests_name)
{
	if (stat)
	{
		printf("\033[0;31m");
		printf("\n\n\033[1mFailed in %d tests.\n\n", stat);
	}
	else
	{
		printf("\033[0;32m");
		printf("\n\n\033[1mPassed all %s tests!\n\n", tests_name);
	}	
	printf("\033[0m\033[m");
}

void PrintUnitTitle(char *func_name)
{
	printf("\n\n\033[1m%s tests:\033[m\n\n", func_name);
}

void PrintUnitStat(int stat, char *func_name)
{
	if (stat)
	{
		printf("\033[0;31m");
		printf("Failed in %d tests.\n", stat);
	}
	else
	{
		printf("\033[0;32m");
		printf("Passed all %s tests.\n\n", func_name);
	}	
	printf("\033[0m");
}

void PrintCheckStat(int stat)
{
	if (stat)
	{
		printf(FAIL_MARK);
	}
	else
	{
		printf(CHECK_MARK);
	}
}

void PrintCheckTitle(char *tests_name, size_t test_num)
{
	printf("%s test no. %lu:", tests_name, test_num);
}
