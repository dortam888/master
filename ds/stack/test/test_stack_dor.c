#include "stack.h"
#include <stdio.h>

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n", #function)


void Test_StackCreateAndDestroy()
{
	stack_t *stack = StackCreate(50, 4);

	StackDestroy(stack);

	if (stack == NULL)
	{
		PASS;
	}
	else
	{
		FAIL;
	}
}

void Test_StackPush()
{
	stack_t *stack1 = StackCreate(5,4);
	stack_t *stack2 = StackCreate(1,4);
}


void Test_StackPop()
{

}


void Test_StackPeek()
{

}


void Test_StackSize()
{

}


void Test_StackIsEmpty()
{

}


int main()
{
	return 0;
}
