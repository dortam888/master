#include "stack.h" /* stack_t */
#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

enum {STACK_IS_EMPTY = -2, STACK_OVERFLOW = -1, OK = 0};

struct stack
{
	void *base;
	void *current;
	void *end;
	size_t size_of_element;
};

stack_t* StackCreate(size_t max_capacity, size_t size_of_element) 
{
	size_t stack_length_bytes = max_capacity * size_of_element;

	stack_t *stack = malloc(sizeof(stack_t));

	if (stack != NULL)
	{
		stack->base = malloc(stack_length_bytes);
		
		if (stack->base != NULL) 
		{
			stack->current = stack->base;
			stack->end = (char *)stack->base + stack_length_bytes;
			stack->size_of_element = size_of_element;
		}
		else
		{
			free(stack);
			stack = NULL;
		}
	}

	return stack;
}


void StackDestroy(stack_t *stack)
{
	free(stack->base);
	stack->base = NULL;
	stack->current = NULL;
	stack->end = NULL;

	free(stack);
}


int StackPush(stack_t *stack, void *data) 
{
	assert(stack != NULL && data != NULL);

	if ((size_t)(stack->end) - (size_t)(stack->current) < 
													   stack->size_of_element)
	{
		return STACK_OVERFLOW;
	}

	memcpy(stack->current, data, stack->size_of_element);
	stack->current = (char *)stack->current + stack->size_of_element;

	return OK;
}


int StackPop(stack_t *stack) 
{

	int function_status = STACK_IS_EMPTY;

	assert(NULL != stack);

	if (!StackIsEmpty(stack))
	{
		stack->current = (char *)stack->current - stack->size_of_element;
		function_status = OK;
	}

	return function_status; 
}


void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);

	if (StackIsEmpty(stack))
	{
		return NULL;
	}

	return (void *)((char *)stack->current - stack->size_of_element);
}


size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);

	return ((char *)stack->current - (char *)stack->base) / 
													 (stack->size_of_element);
}


int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);

	return (stack->current == stack->base);
}

