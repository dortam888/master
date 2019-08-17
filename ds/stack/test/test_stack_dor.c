#include <stdio.h> /*printf*/

#include "stack.h" /*stack_t*/

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n", #function)


void TestFlowEmptyStack()
{
	stack_t *stack = StackCreate(5,4);
	size_t stack_size = 0;
	int error_counter = 0;

	stack_size = StackSize(stack);

	/* check size is 0 */
	if (stack_size != 0)
	{
		FAIL(Size);
		++error_counter;
	}

	/* check that the stack is empty before any action */
	if (!StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++error_counter;
	}

	StackDestroy(stack);

	if (0 == error_counter)
	{
		PASS(EmptyStackActions);
	}
}

void TestFlowStack()
{
	stack_t *stack = StackCreate(3,4);
	size_t stack_size = 0;
	int error_counter = 0;
	int a = 77;

	StackPush(stack, &a);

	stack_size = StackSize(stack);

	/* check size is 1 aftr we push */
	if (stack_size != 1)
	{
		FAIL(Size);
		++error_counter;
	}

	/* check that the stack is not empty after push */
	if (StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++error_counter;
	}

	/* check that the element in the stack is the right one */
	if (*(int *)StackPeek(stack) != a)
	{
		FAIL(Peek);
		++error_counter;
	}

	StackPop(stack);

	/* check that it is empty after we pop*/
	if (!StackIsEmpty(stack))
	{
		FAIL(StackPop);
		++error_counter;
	}

	StackDestroy(stack);

	if (0 == error_counter)
	{
		PASS(StackActions);
	}
}

void TestOverflowStack()
{
	stack_t *stack = StackCreate(3,4);
	size_t stack_size = 0;
	int status = 0;
	int error_counter = 0;
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
		++error_counter;
	}

	/* check that the stack is not empty after push */
	if (StackIsEmpty(stack))
	{
		FAIL(IsEmpty);
		++error_counter;
	}

	/* check that the element in the stack is the right one */
	if (*(int *)StackPeek(stack) != *(int *)c)
	{
		FAIL(Peek);
		++error_counter;
	}

	StackDestroy(stack);

	if (0 == error_counter)
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
