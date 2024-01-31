/*************************************
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Karin    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: State Machine	         *
**************************************/
					   			        
#include <stdio.h>	/* printf() */

#include "state_machine.h"	/* state_machine */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

int main(void)
{
	char *success_str[5] = {"01000010", "00", "010", "01111110", "010101010"};
	char *fail_str[5] = {"11000010", "0", "0001001", "11111111", "110101010"};
	size_t i = 0;
	
	printf("\n\t------------- Tests for ACCEPTED strings: -------------\n\n");
	for (i = 0; i < 5; ++i)
	{
		printf("\t\tStateMachine Test-%ld: \t\t%s\n", i + 1, TEST_RES(StateMachine(success_str[i])));
	}
	
	printf("\n\t------------- Tests for NOT_ACCEPTED strings: -------------\n\n");
	for (i = 0; i < 5; ++i)
	{
		printf("\t\tStateMachine Test-%ld: \t\t%s\n", i + 1, TEST_RES(!StateMachine(fail_str[i])));
	}
	printf("\n");
	


	return 0;
}


