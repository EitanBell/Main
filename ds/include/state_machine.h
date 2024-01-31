/*************************************
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Karin    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: State Machine	         *
**************************************/

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

typedef enum state
{ 
	START = 0, 
	VALID, 
	WAIT_FOR_DIGIT, 
	INVALID
}state_t;

/*------------------------------------------------------------------------------
Finite State Machine (FSM):
	A state machine reads a set of inputs and changes to a different state
	based on those inputs. FSM can have one letter at a time, and changes his
	state 

MAIN FUNCTIONALITY: 
	StateMachine().
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
DESCRIPTION: 
	This specific StateMachine() function, has this conditions for ACCEPTED
	word:
		- A word should start and finish by 0's.
		- Has at least 2 digits.
		- A word can have any amount of digits between those two 0's.

PARAMETERS: 
	word - the string input.

RETURN VALUE:
	0 for accepted,
	1 otherwise.

NOTE!
	Entering other characters (not 0/1) may cause an undefined behavior.

Complexity:
	time: O(n) / space: O(1).
------------------------------------------------------------------------------*/
int StateMachine(const char *word);

#endif /* __STATE_MACHINE_H__ */
