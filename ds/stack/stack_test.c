/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Tal       *
*	Date: 29/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Stack API	         *
*				     *
**************************************/
#include <stdio.h> /* prinft */	

#include "stack.h" /* my stack */

void TestStack(void);
void TestEmpty(stack_t *stack);

int main(void)
{
	TestStack();
	
	return 0;
}


void TestStack(void)
{
	int member1 = 1;
	char member2 = 'a';
	char *member3 = "ahi";
	char *member4 = "ah yakar";
	float member5 = 2.71;
	int member6 = -123;
	size_t capacity = 10;
	
	stack_t *stack = StackCreate(capacity);
	
	printf("capacity of stack: %ld \n", StackCapacity(stack));
	TestEmpty(stack);
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, &member1);
	printf("push\n");
	printf("top of the stack: %d \n", *(int *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, &member2);
	printf("push\n");
	printf("top of the stack: %c \n", *(char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, member3);
	printf("push\n");
	printf("top of the stack: %s \n", (char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, member4);
	printf("push\n");
	printf("top of the stack: %s \n", (char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, &member5);
	printf("push\n");
	printf("top of the stack: %.2f \n", *(float *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	StackPush(stack, &member6);
	printf("push\n");
	printf("top of the stack: %d \n", *(int *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	TestEmpty(stack);
	
	printf("pop: %d \n", *(int *)StackPop(stack));
	printf("top of the stack: %.2f \n", *(float *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	printf("pop: %.2f \n", *(float *)StackPop(stack));
	printf("top of the stack: %s \n", (char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	printf("pop: %s \n", (char *)StackPop(stack));
	printf("top of the stack: %s \n", (char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	printf("pop: %s \n", (char *)StackPop(stack));
	printf("top of the stack: %c \n", *(char *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	printf("pop: %c \n", *(char *)StackPop(stack));
	printf("top of the stack: %d \n", *(int *)StackPeek(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	printf("pop: %d \n", *(int *)StackPop(stack));
	printf("size of stack: %ld \n", StackSize(stack));
	
	TestEmpty(stack);
	
	StackDestroy(stack);
	
	stack = StackCreate(capacity);
	StackPush(stack, member3);
	StackPush(stack, member3);
	printf("size of stack: %ld \n", StackSize(stack));
	StackPop(stack);
	printf("size of stack: %ld \n", StackSize(stack));
	TestEmpty(stack);
	StackDestroy(stack);
}

void TestEmpty(stack_t *stack)
{
	printf("\n");
	if (StackIsEmpty(stack))
	{
		printf("stack is empty \n");
	}
	else
	{
		printf("stack is not empty \n");
	}
	printf("\n");
}

