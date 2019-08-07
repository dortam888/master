#ifndef ILRD_STACK_H
#define ILRD_STACK_H
#include <stddef.h> /* size_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Last Update: 6/8/19 */
/* This file contains functions for stack data structure operations.*/
/* Can be used for stack based algorithms */
/*############################################################################*/

typedef struct stack stack_t;

/*#############################################################################
  # Description: 
	gets a pointer of stack_t type, the number of elements that the stack should
	hold (max_capacity) and the size of each of the data elements in the stack
  # Parameters:
	max_capacity: the maximum elements that the stack should hold
	size_of_element: the size of single data element that the stack should hold
  # Return Value:
	in case of success returns a pointer to the stack itself,
	in case of failure returns NULL
	if max_capacity or size_of_element is 0 undefined behavior
##############################################################################*/
stack_t* StackCreate(size_t max_capacity, size_t size_of_element);


/*#############################################################################
  # Description: 
	gets a pointer to the top of a stack and frees the memory that the stack
	holds.
  # Parameters:
	stack - the pointer to the stack that needs to be terminated
  # Return Value:
	None
##############################################################################*/
void StackDestroy(stack_t *stack);


/*#############################################################################
  # Description: 
	gets a pointer to the top of a stack and a data element.
	stores data element in the top of the stack.
  # Parameters:
	stack - the pointer to the stack to whom we push
	data - the data that should be stored in the stack
  # Return Value:
	returns 0 in case of success, and other integer value in case of failure
##############################################################################*/
int StackPush(stack_t *stack, void *data);


/*#############################################################################
  # Description: 
	gets a pointer to the top of a stack and pops the data element from the
	top of the stack
  # Parameters:
	stack - the pointer to the stack where we pop
  # Return Value:
	returns 0 in case of success, and other integer value in case of failure
##############################################################################*/
int StackPop(stack_t *stack);


/*#############################################################################
  # Description: 
	gets a pointer to the top of the stack and shows us the element in the 
	last place
  # Parameters:
	stack - the pointer to the stack of the element we want
  # Return Value:
	The element in the last place
	If stack is empty returns NULL
##############################################################################*/
void* StackPeek(const stack_t *stack);


/*#############################################################################
  # Description: 
	returns us how many elements are currently inside the stack
  # Parameters:
	stack - the pointer to the stack for which we want the size
  # Return Value:
	how many data elements are in the stack
##############################################################################*/
size_t StackSize(const stack_t *stack);


/*#############################################################################
  # Description: 
	tells us if we have any elements in the stack
  # Parameters:
	stack - the pointer to the stack that we check for elements
  # Return Value:
	1 if there are no elements in the stack 0 otherwise
##############################################################################*/
int StackIsEmpty(const stack_t *stack);

#endif /* ILRD_STACK_H */
