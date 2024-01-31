/*************************************
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  itai    	         *
*	Date: 22/01/23	                 *
*	Valgrind & GDB passed            *
*	Subject: Calculator	    	     *
**************************************/

#include <string.h>	    /* strtod, strlen */
#include <stdlib.h> 	/* malloc, free*/
#include <assert.h> 	/* assert */
#include <math.h>		/* pow */

#include "calculator.h" /* calculator API */
#include "stack.h"		/* stack API*/

#define ASCII_SIZE (256)
#define INIT_NUM (1)
#define INIT_OPER (1)

typedef enum state 
{
	WAIT_FOR_NUM, 
	WAIT_FOR_OPER,
	RESULT,
	ERROR,
	STATES_NUM
} state_t;

typedef enum input
{
	DIGIT,
	ADD,
	SUB,
	MULTI,
	DIVIDE,
	POW,
	OP_PAR,
	CL_PAR,
	SPACE,
	END,
	OTHER,
	INPUT_NUM, 
	NO_TYPE
} input_t;

typedef struct calc
{
   char *expression;
   double *result;
   stack_t *nums_stack;
   stack_t *opers_stack;
   state_t state;
   status_t status;
	
} calculator_t;

typedef double (*math_oper_t)(double num1, double num2);
typedef state_t (*handler_t)(calculator_t *calculator, double *result);

/*------------------------------------------------------------------------*/
/*                              STATIC FUNCTIONS                          */
/*------------------------------------------------------------------------*/

/*--------------- GENERAL funcs ---------------*/
static input_t GetInputType(char *expression);
static void DestroyStacks(stack_t *stack1, stack_t *stack2);
static double CalcExpression(calculator_t *calc);

/*--------------- CHECK funcs ---------------*/
static void IsValidSyntax(calculator_t *calc);
static int CheckParenthesis(const char *str);
static state_t CheckMathFault(calculator_t *calc, double num);
static int CheckPriority(char *new_op, void *peek_op);
static int IsOpenParInStack(calculator_t *calc);

/*--------------- INIT funcs ---------------*/
static void InitOperLut(math_oper_t *oper_lut);
static void InitInputLut(input_t *input_lut);
static void InitCalculator(calculator_t *calc, const char *str, 
															double *result);

/*--------------- MATH funcs ---------------*/
static double Add(double num1, double num2);
static double Substract(double num1, double num2);
static double Multiply(double num1, double num2);
static double Divide(double num1, double num2);
static double Power(double num1, double num2);
static double DoNothing(double num1, double num2);

/*--------------- HANDLER funcs ---------------*/
static state_t NumHandler(calculator_t *calc, double *result);
static state_t OperHandler(calculator_t *calc, double *result);
static state_t SpaceHandler(calculator_t *calc, double *result);
static state_t ParenHandler(calculator_t *calc, double *result);
static state_t ErrorHandler(calculator_t *calc, double *result);
static state_t ResultHandler(calculator_t *calc, double *result);

/*--------------- LUT funcs ---------------*/
static input_t input_lut[ASCII_SIZE] = {0};
static math_oper_t oper_lut[ASCII_SIZE] = {0}; 
static handler_t transition_lut[INPUT_NUM][STATES_NUM] = 
{
	/*WAIT_FOR_NUM*//*WAIT_FOR_OPER*/ /*RESULT*/  /*ERROR*/

	{&NumHandler, &ErrorHandler, &ResultHandler, &ErrorHandler},  	/* NUM */
	{&NumHandler, &OperHandler, &ResultHandler, &ErrorHandler},     /* ADD */ 
	{&NumHandler, &OperHandler, &ResultHandler, &ErrorHandler},	    /* SUB */
	{&ErrorHandler, &OperHandler, &ResultHandler, &ErrorHandler},	/* MULTI */
	{&ErrorHandler, &OperHandler, &ResultHandler, &ErrorHandler}, 	/* DIVIDE */
	{&ErrorHandler, &OperHandler, &ResultHandler, &ErrorHandler},   /* POW */
	{&ParenHandler, &ParenHandler, &ResultHandler, &ErrorHandler},	/* OP_PAR */
	{&ParenHandler, &ParenHandler, &ResultHandler, &ErrorHandler},	/* CL_PAR */
	{&SpaceHandler, &SpaceHandler, &ResultHandler, &ErrorHandler},	/* SPACE */
	{&ErrorHandler, &ResultHandler, &ResultHandler, &ErrorHandler},	/* END */
	{&ErrorHandler, &ErrorHandler, &ResultHandler, &ErrorHandler} 	/* OTHER */
};

/*------------------------------------------------------------------------*/
/*                              MAIN FUNCTIONS                            */
/*------------------------------------------------------------------------*/

/*------------------------------- Calculate ------------------------------*/

status_t Calculate(const char *string, double *result)
{
	calculator_t calc = {0};
	input_t input_type = NO_TYPE;
	
	assert(NULL != string);
	assert(NULL != result);
	
	InitOperLut(oper_lut);		
	InitInputLut(input_lut);	
	InitCalculator(&calc, string, result); 
	if (ERROR == calc.state)
	{
		DestroyStacks(calc.nums_stack, calc.opers_stack);
		return SYNTAX_FAULT;
	}

	IsValidSyntax(&calc);
	if (ERROR == calc.state)
	{
		DestroyStacks(calc.nums_stack, calc.opers_stack);
		return SYNTAX_FAULT;
	} 
	
	while (ERROR != calc.state && RESULT != calc.state)
	{
		input_type = GetInputType(calc.expression); 
		calc.state = transition_lut[input_type][calc.state](&calc, result);
	}
	
	DestroyStacks(calc.nums_stack, calc.opers_stack);
	
	return calc.status;
}

/*------------------------------------------------------------------------*/
/*                               HELPER FUNCTIONS                         */
/*------------------------------------------------------------------------*/

/*-------------------------------- InitOperLut ---------------------------*/

static void InitOperLut(math_oper_t *oper_lut)
{
	size_t i = 0;
	
	assert(NULL != oper_lut);
	
	for (i = 0; ASCII_SIZE > i; ++i)
	{
	    oper_lut[i] = &DoNothing;
	}
	
	oper_lut['+'] = &Add;
	oper_lut['-'] = &Substract;
	oper_lut['*'] = &Multiply;
	oper_lut['/'] = &Divide;
	oper_lut['^'] = &Power;
}

/*----------------------------- InitInputLut -----------------------------*/

static void InitInputLut(input_t *input_lut)
{
	int i = 0;
	
	assert(NULL != input_lut);
	
	for (i = 0; ASCII_SIZE > i; ++i)
	{
		input_lut[i] = OTHER;
	}
	
	for (i = '0'; '9' >= i; ++i)
	{
		input_lut[i] = DIGIT;
	}
	
	input_lut['+'] = ADD;
	input_lut['-'] = SUB;
	input_lut['*'] = MULTI;
	input_lut['/'] = DIVIDE;
	input_lut['^'] = POW;
	input_lut['('] = OP_PAR;
	input_lut[')'] = CL_PAR;
	input_lut[' '] = SPACE;
	input_lut['\0'] = END;
}

/*---------------------------- InitCalculator ----------------------------*/

static void InitCalculator(calculator_t *calc, const char *str, 
															 double *result)
{
	void *init_num = NULL;
	char *init_oper = "+";
	double num = 0;

	calc->opers_stack = StackCreate(strlen(str) + INIT_OPER);
	if (NULL == calc->opers_stack)
	{
		calc->status = SYS_FAULT;
		calc->state = ERROR;
	}
	
	calc->nums_stack = StackCreate(strlen(str) + INIT_NUM);
	if (NULL == calc->nums_stack)
	{
		StackDestroy(calc->opers_stack);
		calc->status = SYS_FAULT;
		calc->state = ERROR;
	}
	
	calc->expression = (char *)str;
	calc->result = result;
	calc->state = WAIT_FOR_NUM;
	calc->status = SUCCESS;
	
	memcpy((double **)&init_num, &num, sizeof(double));
	StackPush(calc->nums_stack, init_num);
	StackPush(calc->opers_stack, init_oper);
}

/*---------------------------- IsValidSyntax -----------------------------*/

static void IsValidSyntax(calculator_t *calc)
{
	assert(NULL != calc);
	
	if (WAIT_FOR_NUM == calc->state)
	{
		calc->status = CheckParenthesis(calc->expression);
		
		if (SUCCESS != calc->status)
		{
			calc->state = ERROR;
		}
	}
}

/*-------------------------- CheckParenthesis ----------------------------*/

static int CheckParenthesis(const char *str)
{
	stack_t *parent_stack = NULL;
	int status = SYNTAX_FAULT;
	char *runner = NULL;
	
	assert(NULL != str);
	runner = (char *)str;
	
	parent_stack = StackCreate(strlen(str));
	
	while ('\0' != *(runner))
	{
	    switch (*runner)
	    {
	        case '(':
	        {
	        	StackPush(parent_stack, runner);
	        	++runner;
	            break;
	        }
	        case ')':
	        {
	          if (!StackIsEmpty(parent_stack) && 
				  '(' == *(char *)StackPeek(parent_stack))
	          {
	              StackPop(parent_stack);
	          }
	          else
	          {
	          	StackPush(parent_stack, runner);
	          }
	              ++runner;
	              break;
	        }
	        default:
	        {
	        	++runner;
	        	break;
	        }	
	    }
	}
   
	status = (StackIsEmpty(parent_stack) ? SUCCESS : SYNTAX_FAULT);
	StackDestroy(parent_stack);
	
	return status;
}

/*---------------------------- GetInputType ------------------------------*/

static input_t GetInputType(char *expression)
{
	assert(NULL != expression);
	
	return input_lut[(int)*expression];
}

/*---------------------------- DestroyStacks -----------------------------*/

static void DestroyStacks(stack_t *stack1, stack_t *stack2)
{
	assert(NULL != stack1);
	assert(NULL != stack2);
	
	StackDestroy(stack1);
	StackDestroy(stack2);
}

/*--------------------------- CheckMathFault -----------------------------*/

static state_t CheckMathFault(calculator_t *calc, double num)
{
	assert(NULL != calc);
	
	if (!StackIsEmpty(calc->opers_stack) && 
		0.0 == num &&
		'/' == *(char *)StackPeek(calc->opers_stack))
	{
		calc->status = MATH_FAULT;
		calc->state = ERROR;
	}
	
	return calc->state;
}

/*---------------------------- CheckPriority -----------------------------*/

static int CheckPriority(char *new_op, void *peek_op)
{
	input_t new_input = 0;
	input_t peek_input = 0;
	
	assert(NULL != new_op);
	assert(NULL != peek_op);
	
	new_input = GetInputType(new_op); 
	new_input += (ADD == new_input) + (MULTI == new_input);
	                
	peek_input = GetInputType((char *)peek_op); 
	peek_input += (ADD == peek_input) + (MULTI == peek_input);
	
	return (new_input - peek_input);
}

/*----------------------------- CalcAndPush ------------------------------*/

static double CalcAndPush(calculator_t *calc)
{
	double res = 0;
	void *num = NULL;
	
	assert(NULL != calc);
	
	res = CalcExpression(calc);
	memcpy(&num, &res, sizeof(double));
	StackPush(calc->nums_stack, num);
	
	return res;
}

/*---------------------------- CalcExpression ----------------------------*/

static double CalcExpression(calculator_t *calc)
{
	double res = 0;
	double num1 = 0;
	double num2 = 0;
	void *num1_ptr = NULL;
	void *num2_ptr = NULL;
	char oper = '\0';
	
	assert(NULL != calc);
		
	oper = *(char *)StackPop(calc->opers_stack);
		
	num2_ptr = StackPop(calc->nums_stack);
	num1_ptr = StackPop(calc->nums_stack);
	
	memcpy(&num1, &num1_ptr, sizeof(double));
	memcpy(&num2, &num2_ptr, sizeof(double));
	
	res = oper_lut[(int)oper](num1, num2); 
	
	return res;
}

/*-------------------------- IsOpenParInStack ----------------------------*/

static int IsOpenParInStack(calculator_t *calc)
{
	assert(NULL != calc);
	
	return ('(' == *(char *)StackPeek(calc->opers_stack));
}

/*------------------------------------------------------------------------*/
/*                             HANDLER FUNCTIONS                          */
/*------------------------------------------------------------------------*/

/*------------------------------ NumHandler ------------------------------*/

static state_t NumHandler(calculator_t *calc, double *result)
{
	double res = 0;
	void *res_ptr = NULL;
		
	assert(NULL != calc);
	assert(NULL != result);
	(void)result;
	
	res = strtod(calc->expression, &calc->expression);

	memcpy(&res_ptr, &res, sizeof(double));
	
	calc->state = WAIT_FOR_OPER;
	calc->state = CheckMathFault(calc, res);
	
	StackPush(calc->nums_stack, res_ptr);

	return calc->state;
}

/*----------------------------- OperHandler ------------------------------*/

static state_t OperHandler(calculator_t *calc, double *result)
{
	char *oper = NULL;
	
	assert(NULL != calc);
	assert(NULL != result);
	(void)result;
	
	oper = calc->expression;

	if (!StackIsEmpty(calc->opers_stack)
		&& !IsOpenParInStack(calc) 
		&& 0 >= CheckPriority(oper, StackPeek(calc->opers_stack)))
	{
		CalcAndPush(calc);
	}
	
	StackPush(calc->opers_stack, oper);
	calc->state = WAIT_FOR_NUM;
	++(calc->expression);
	
	return calc->state;
}

/*----------------------------- SpaceHandler -----------------------------*/

static state_t SpaceHandler(calculator_t *calc, double *result)
{
	assert(NULL != calc);
	(void)result;
	
	++(calc->expression);
	
	return calc->state;
}

/*---------------------------- ResultHandler -----------------------------*/

static state_t ResultHandler(calculator_t *calc, double *result)
{
	double res = 0;
	
	assert(NULL != calc);
	assert(NULL != result);
	
	if (IsOpenParInStack(calc))
	{
		calc->state = ERROR;
		calc->status = SYNTAX_FAULT;
	}
	else
	{
		while (!StackIsEmpty(calc->opers_stack))
		{
			res = CalcAndPush(calc);
		} 
		
		*result = res;
		calc->status = SUCCESS;
	}
	
	return RESULT;
}

/*----------------------------- ParenHandler -----------------------------*/

static state_t ParenHandler(calculator_t *calc, double *result)
{	
	assert(NULL != calc);
	assert(NULL != result);
	(void)result;

	if ('(' == *(calc->expression))
	{
		StackPush(calc->opers_stack, calc->expression);
		calc->state = WAIT_FOR_NUM;
	}
	else
	{
		while (!StackIsEmpty(calc->opers_stack) && 
			   '(' != *(char *)StackPeek(calc->opers_stack))
		{
			CalcAndPush(calc);
		}
		
		if (!StackIsEmpty(calc->opers_stack))
		{
			StackPop(calc->opers_stack);	
		}
		else
		{
			calc->state = ERROR;
			calc->status = SYNTAX_FAULT;
		}
	}
	
	++(calc->expression);
	
	return calc->state;
}

/*----------------------------- ErrorHandler -----------------------------*/

static state_t ErrorHandler(calculator_t *calc, double *result)
{
	assert(NULL != calc);
	(void)result;
	
	if (MATH_FAULT != calc->status)
	{
		calc->status = SYNTAX_FAULT;
	}
	
	return ERROR;
}

/*------------------------------------------------------------------------*/
/*                             MATH FUNCTIONS                             */
/*------------------------------------------------------------------------*/

static double Add(double num1, double num2)
{
	return (num1 + num2);
}

static double Substract(double num1, double num2)
{
	return (num1 - num2);
}

static double Multiply(double num1, double num2)
{
	return (num1 * num2);
}

static double Divide(double num1, double num2)
{
	return (num1 / num2);
}

static double Power(double num1, double num2)
{
	return (pow(num1, num2));
}

static double DoNothing(double num1, double num2)
{
	(void)num1;
	(void)num2;
	
	return ERROR;
}

