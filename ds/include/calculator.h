/*************************************
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Dotan    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: Calculator	    	     *
**************************************/

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__


/******************************* Description ***********************************
A calculator that enables to evalutate expressions with addition, subtraction,
multiplication, division and parentheses.

Attributes
		State Machine.
Main Functions
		Calculator.
*******************************************************************************/

/*******************************************************************************
Description
	The status which returns from the function indicates if the calculation was
	successful or not. If not, the value indicates the type of the error
	that occured.
*******************************************************************************/
typedef enum status
{
    SYS_FAULT = -1,
    SUCCESS = 0,
    MATH_FAULT = 1,
    SYNTAX_FAULT = 2
}status_t;

/*******************************************************************************
Description
	A function that receives a mathematical expression as a string, calculates
	it and places the calculated value in a given memory space. 
Parameters
	'string' - The expression to be calculated.
	'result' - The memory place to hold the result of the expression.
Return Value
	Status indicating if the calculation succeeded or not, as described above.
Comments
	If error is returned, the value inside 'result' is unspecified.
Complexity
	Time: O(n) / Space: O(n)
*******************************************************************************/

status_t Calculate(const char *string, double *result);

#endif /* __CALCULATOR_H__ */


