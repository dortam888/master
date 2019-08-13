#include "stack.h"
#include <stdio.h>

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n", #function)


void TestFlowEmptyStack()
{
	stack_t *stack = StackCreate(5,4);
	size_t stack_size = 0;
	int flag = 0;

	stack_size = StackSize(stack);

	/* check size is 0 */
	if (stack_size != 0)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the stack is empty before any action */
	if (!StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++flag;
	}

	StackPop(stack);

	/* check that it is still empty if we pop*/
	if (!StackIsEmpty(stack))
	{
		FAIL(StackPop);
		++flag;
	}

	if (StackPeek(stack) != NULL)
	{
		FAIL(Peek);
		++flag;
	}

	StackDestroy(stack);

	if (0 == flag)
	{
		PASS(EmptyStackActions);
	}
}

void TestFlowStack()
{
	stack_t *stack = StackCreate(3,4);
	size_t stack_size = 0;
	int flag = 0;
	int a = 77;

	StackPush(stack, &a);

	stack_size = StackSize(stack);

	/* check size is 1 aftr we push */
	if (stack_size != 1)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the stack is not empty after push */
	if (StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++flag;
	}

	/* check that the element in the stack is the right one */
	if (*(int *)StackPeek(stack) != a)
	{
		FAIL(Peek);
		++flag;
	}

	StackPop(stack);

	/* check that it is empty after we pop*/
	if (!StackIsEmpty(stack))
	{
		FAIL(StackPop);
		++flag;
	}

	StackDestroy(stack);

	if (0 == flag)
	{
		PASS(StackActions);
	}
}

void TestOverflowStack()
{
	stack_t *stack = StackCreate(3,4);
	size_t stack_size = 0;
	int status = 0;
	int flag = 0;
	int a = 77;
	char b[4] = "abc";
	short c[2] = {1, 2};

	StackPush(stack, &a);
	StackPush(stack, &b);
	StackPush(stack, &c);

	stack_size = StackSize(stack);

	/* check size is 3 aftr we push 3 elements*/
	if (stack_size != 3)
	{
		printf("%lu", stack_size);
		FAIL(Size);
		++flag;
	}

	/* check that the stack is not empty after push */
	if (StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++flag;
	}

	/* check that the element in the stack is the right one */
	if (*(int *)StackPeek(stack) != *(int *)c)
	{
		FAIL(Peek);
		++flag;
	}

	/* Check that it doesn't let me to push when in overflow*/
	
	status = StackPush(stack, &a);

	/* check that status is not 0*/
	if (status == 0)
	{
		FAIL(Push);
		++flag;
	}

	if (StackSize(stack) != 3)
	{
		FAIL(Push);
		++flag;
	}

	StackDestroy(stack);

	if (0 == flag)
	{
		PASS(StackOverflowActions);
	}
}


int main()
{
	TestFlowEmptyStack();
	TestFlowStack();
	TestOverflowStack();
	return 0;
}
