/*************************************
*	Developer: Eitan Bcellaiche       *	 
*	Reviewer:  itai    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: Calculator	    	     *
**************************************/
					   			        
#include <stdio.h>	/* printf() */

#include "calculator.h"	/* calculator */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOLEAN_TO_STATUS(res) (res ^ 1)


int main(void)
{
	char *plus_basic_input[5] = {"7.1 + 5", "3.5+4", "1 +2 + 3.9 +4", "1++5", "007"};
	double plus_basic_res[5] = {12.1, 7.5, 10.9, 6, 7};
	
	char *minus_basic_input[5] = {"7.2 - 5", "3-4", "1 -2.9 + 3 -4", "5-6+7-8+9", "-8"};
	double minus_basic_res[5] = {2.2, -1, -2.9, 7, -8};
	
	char *multiple_basic_input[4] = {"7 * 5", "3*4", "1 *2.2 + 3 *4", "0*8"};
	double multiple_basic_res[4] = {35, 12, 14.2, 0};
	
	char *divide_basic_input[5] = {"7 / 5", "3/4", "100/25", "8/2"};
	double divide_basic_res[5] = {1.4, 0.75, 4.0, 4.0};
	
	char *pare_basic_input[5] = {"(7+5)", "(((((1)))))", "((8/2))"};
	double pare_basic_res[5] = {12, 1, 4.0};
	
	char *power_basic_input[5] = {"2^3", "1^1000", "0^1"};
	double power_basic_res[5] = {8, 1, 0};
	
	char *syntax_error_input[7] = {"/5", "1**5", "^1^", "EITAN_BELL", ")(1+1)", "()"};
	char *math_error_input[1] = {"1/0"};
	
	char *expression_input[5] = {"(1)+((5*2)/-1+0)", "2-(1+0.5-1)+(1)", "(1+1)+(2*2)/(3^0)-(-1)", "3*(4-1)/4/3+1/1"};
	double expression_res[5] = {-9, 2.5, 7, 1.75};
	
	double res_arr[5] = {0, 0, 0, 0, 0};
	double res_arr2[7] = {0, 0, 0, 0, 0, 0, 0};
	int test_res = 0;
	size_t i = 0;
	
	printf("\n\t------------------ Tests for PLUS BASIC INPUT: -----------------\n\n");
	for (i = 0; i < 5; ++i)
	{
		test_res = (SUCCESS == Calculate(plus_basic_input[i], res_arr + i)) && (res_arr[i] == plus_basic_res[i]);
		printf("\t\tCalculate PLUS BASIC INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t----------------- Tests for MINUS BASIC INPUT: -----------------\n\n");
	for (i = 0; i < 5; ++i)
	{
		test_res = (SUCCESS == Calculate(minus_basic_input[i], res_arr + i)) && (res_arr[i] == minus_basic_res[i]);
		printf("\t\tCalculate MINUS BASIC INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t---------------- Tests for MULTIPLE BASIC INPUT: ----------------\n\n");
	for (i = 0; i < 4; ++i)
	{
		test_res = (SUCCESS == Calculate(multiple_basic_input[i], res_arr + i)) && (res_arr[i] == multiple_basic_res[i]);
		printf("\t\tCalculate MULTIPLE BASIC INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t----------------- Tests for DEVIDE BASIC INPUT: -----------------\n\n");
	for (i = 0; i < 4; ++i)
	{
		test_res = (SUCCESS == Calculate(divide_basic_input[i], res_arr + i)) && (res_arr[i] == divide_basic_res[i]);
		printf("\t\tCalculate DEVIDE BASIC INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t--------------- Tests for PARENTHESIS BASIC INPUT: ---------------\n\n");
	for (i = 0; i < 3; ++i)
	{
		test_res = (SUCCESS == Calculate(pare_basic_input[i], res_arr + i)) && (res_arr[i] == pare_basic_res[i]);
		printf("\t\tCalculate PARENTHESIS BASIC INPUT Test-%ld:   %s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t------------------ Tests for POWER BASIC INPUT: ------------------\n\n");
	for (i = 0; i < 3; ++i)
	{
		test_res = (SUCCESS == Calculate(power_basic_input[i], res_arr + i)) && (res_arr[i] == power_basic_res[i]);
		printf("\t\tCalculate POWER BASIC INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t------------------ Tests for SYNTAX FAULT INPUT: ------------------\n\n");
	for (i = 0; i < 6; ++i)
	{
		test_res = (SYNTAX_FAULT == Calculate(syntax_error_input[i], res_arr2 + i ));
		printf("\t\tCalculate SYNTAX FAULT INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t------------------ Tests for MATH FAULT INPUT: ------------------\n\n");
	for (i = 0; i < 1; ++i)
	{	
		status_t calc_res = Calculate(math_error_input[i], res_arr + i);
		test_res = (MATH_FAULT == calc_res);
		printf("\t\tCalculate MATH FAULT INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	printf("\n\t------------------ Tests for EXPRESSION INPUT: ------------------\n\n");
	for (i = 0; i < 4; ++i)
	{
		test_res = (SUCCESS == Calculate(expression_input[i], res_arr + i)) && (res_arr[i] == expression_res[i]);
		printf("\t\tCalculate EXPRESSION INPUT Test-%ld: \t%s\n", i + 1, TEST_RES(BOOLEAN_TO_STATUS(test_res)));
	}
	
	return 0;
}
