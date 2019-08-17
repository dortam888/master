/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 17/08/2019
**** Reviewer: Alex
**** Description: This file contains the implementations of functions
				  for the data structure stack.
				  Look at stack.h for more information about the 
				  functions.
*******************************************************************************/

#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "stack.h" /* stack_t */

enum function_status {SUCCESS = 0};

struct stack
{
	char *base;
	char *current;
	char *end;
	size_t size_of_element;
};

stack_t* StackCreate(size_t max_capacity, size_t size_of_element) 
{
	size_t stack_length_bytes = max_capacity * size_of_element;

	stack_t *stack = malloc(sizeof(stack_t));
	if (stack == NULL)
	{
		return NULL;
	}
	
	stack->base = malloc(stack_length_bytes);
	if (stack->base == NULL) 
	{
		free(stack);
		stack = NULL;
	}

	stack->current = stack->base;
	stack->end = stack->base + stack_length_bytes;
	stack->size_of_element = size_of_element;

	return stack;
}


void StackDestroy(stack_t *stack)
{
	free(stack->base);
	stack->base = NULL;
	stack->current = NULL;
	stack->end = NULL;

	free(stack);
	stack = NULL;
}

static int IsStackFull(const stack_t *stack)
{
	return ((size_t)(stack->end) - (size_t)(stack->current) >= 
													   stack->size_of_element);
}

int StackPush(stack_t *stack, void *data) 
{
	assert(stack != NULL && data != NULL);
	assert(!IsStackFull(stack));

	memcpy(stack->current, data, stack->size_of_element);
	stack->current += stack->size_of_element;

	return SUCCESS;
}


int StackPop(stack_t *stack) 
{
	assert(NULL != stack);
	assert(!StackIsEmpty(stack));

	stack->current -= stack->size_of_element;
	
	return SUCCESS; 
}


void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	assert(!StackIsEmpty(stack));

	return (void *)(stack->current - stack->size_of_element);
}


size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);

	return ((stack->current - stack->base) / (stack->size_of_element));
}


int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);

	return (stack->current == stack->base);
}

