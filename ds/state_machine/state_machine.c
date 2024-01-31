/*************************************
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Karin    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: State Machine	         *
**************************************/

#include <assert.h>	/* assert */
#include <stddef.h>	/* NULL */

#include "state_machine.h"	/* state_machine */

state_t state = START;

typedef enum word_res
{
	ACCEPTED = 0,
	NOT_ACCEPTED
}word_res_t;

/*------------- Helper Funcs -------------*/
static void ValidStart(char digit);
static void WaitForDigit(char digit);
static void WaitForZero(char digit);

/*---------------------------- StateMachine --------------------------------*/

int StateMachine(const char *word)
{
	char *curr_digit = NULL;
	
	assert(NULL != word);
	
	state = START;
	curr_digit = (char *)word;
	
	while ('\0' != *curr_digit)
	{
		switch (state)
		{
			case START:
				ValidStart(*curr_digit);
				break;
				
			case WAIT_FOR_DIGIT:
				WaitForDigit(*curr_digit);
				break;
				
			case VALID:
				WaitForZero(*curr_digit);
				break;
				
			default:
				break;
		}
		++curr_digit;
	}
	
	return ((VALID == state) ? ACCEPTED : NOT_ACCEPTED);
}

/*---------------------------- ValidStart --------------------------------*/

static void ValidStart(char digit)
{
	if ('0' == digit)
	{
		state = WAIT_FOR_DIGIT;
	}
	else
	{
		state = INVALID;
	}
}

/*---------------------------- WaitForDigit --------------------------------*/

static void WaitForDigit(char digit)
{
	if ('0' == digit)
	{
		state = VALID;
	}
	else
	{
		state = WAIT_FOR_DIGIT;
	}
}

/*---------------------------- WaitForZero --------------------------------*/

static void WaitForZero(char digit)
{
	if ('0' == digit)
	{
		state = VALID;
	}
	else
	{
		state = WAIT_FOR_DIGIT;
	}
}

