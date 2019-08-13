#ifndef ILRD_LINKED_LIST_H
#define ILRD_LINKED_LIST_H
#include <stddef.h> /* size_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Tuesday August 13 2019 01:31:06 PM */
/* This file contains functions for linked_list data structure operations.*/
/* Can be used for linked_list based algorithms */
/*############################################################################*/

typedef struct node node_t;

/*#############################################################################
  # Description: 
	get an intial capacity and creates a circular buffer
  # Parameters:
	capacity: memory in bytes
  # Return Value:
	in case of success returns a pointer to a circular buffer
	in case of failure returns NULL
##############################################################################*/
node_t *LinkedListCreateNode(void *data);

/*#############################################################################
  # Description: 
	get c_buff and destroy it
  # Parameters:
	c_buff - a pointer to c_buff 
  # Return Value:
	None
##############################################################################*/
void LinkedListFreeAll(node_t *head);

/*#############################################################################
  # Description: 
	get a circular buffer and returns how much bytes it can hold
  # Parameters:
	c_buff - a pointer to c_buff 
  # Return Value:
	number of bytes the circular buffer can hold
##############################################################################*/
int LinkedListInsert(node_t *node);

/*#############################################################################
  # Description: 
	get a circular buffer and returns how much bytes can be written without lose of information
  # Parameters:
	c_buff - a pointer to c_buff 
  # Return Value:
	number of bytes that can be written
##############################################################################*/
int LinkedListInsertAfter(node_t *node);

/*#############################################################################
  # Description: 
	get a circular buffer and reads number of bytes from it
  # Parameters:
	c_buff - a pointer to c_buff
	number_of_bytes - number of bytes to read from the circular buffer
  # Return Value:
	number of bytes that we read
	returns -1 in case of failure
##############################################################################*/
ssize_t CBuffRead(c_buff_t *c_buff, void *buffer, size_t number_of_bytes);

/*#############################################################################
  # Description: 
	get a circular buffer and writes number of bytes to it
  # Parameters:
	c_buff - a pointer to c_buff
	number_of_bytes - number of bytes to write to the circular buffer
  # Return Value:
	number of bytes that were written to the circular buffer
	returns -1 in case of failure
##############################################################################*/
ssize_t CBuffWrite(c_buff_t *c_buff, void *data ,size_t number_of_bytes);

/*#############################################################################
  # Description: 
	get a circular buffer and check if it's empty
  # Parameters:
	c_buff - a pointer to c_buff
  # Return Value:
	1 if it's empty and 0 otherwise
##############################################################################*/
int CBuffIsEmpty(const c_buff_t *c_buff);
#endif /* ILRD_LINKED_LIST_H */
