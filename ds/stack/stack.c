#include "stack.h"
#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc free */
#include <assert.h>

enum {STACK_OVERFLOW = -1, OK = 0};

struct stack
{
	void *base;
	void *current;
	void *end;
	size_t size_of_element;
};

stack_t* StackCreate(size_t max_capacity, size_t size_of_element)
{
	size_t stack_length = max_capacity * size_of_element;

	stack_t *stack = malloc(sizeof(struct stack));

	if (stack == NULL)
	{
		free(stack);
		stack = NULL;
		return stack;
	}

	stack -> base = malloc(stack_length);

	if (stack -> base == NULL)
	{
		free(stack->base);
		stack->base = NULL;

		free(stack);
		stack = NULL;
		return stack;
	}

	stack -> current = stack -> base;
	stack -> end = (char *)stack -> base + stack_length;
	stack -> size_of_element = size_of_element;

	return stack;
}


void StackDestroy(stack_t *stack)
{
	free(stack -> base);
	stack -> base = NULL;
	stack -> current = NULL;
	stack -> end = NULL;

	free(stack);
}


int StackPush(stack_t *stack, void *data)
{
	assert(stack != NULL && data != NULL);

	if ((size_t)(stack-> end) - (size_t)(stack-> current) < 
													   stack -> size_of_element)
	{
		return STACK_OVERFLOW;
	}

	memcpy(stack -> current, data, stack -> size_of_element);
	stack -> current = (char *)stack -> current + stack -> size_of_element;

	return OK;
}


int StackPop(stack_t *stack)
{
	if (stack -> current != stack -> base)
	{
		stack -> current = (char *)stack -> current - stack -> size_of_element;
	}

	return OK;
}


void *StackPeek(const stack_t *stack)
{
	if (stack -> current == stack -> base)
	{
		return NULL;
	}
	return (void *)((char *)stack -> current - stack -> size_of_element);
}


size_t StackSize(const stack_t *stack)
{
	return ((char *)stack -> current - (char *)stack -> base) / 
													 (stack -> size_of_element);
}


int StackIsEmpty(const stack_t *stack)
{
	return (stack -> current == stack -> base);
}

