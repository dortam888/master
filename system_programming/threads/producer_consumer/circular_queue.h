#ifndef ILRD_c_queue_H
#define ILRD_c_queue_H
#include <unistd.h> /* size_t ssize_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Sunday August 11 2019 11:42:05 AM */
/* This file contains functions for c_queue data structure operations.*/
/* Can be used for c_queue based algorithms */
/*############################################################################*/

typedef struct c_queue c_queue_t;

/*#############################################################################
  # Description: 
	get an intial capacity and creates a circular queueer
  # Parameters:
	capacity: memory in bytes
  # Return Value:
	in case of success returns a pointer to a circular queueer
	in case of failure returns NULL
##############################################################################*/
c_queue_t *CQueueCreate(size_t capacity, size_t element_size);

/*#############################################################################
  # Description: 
	get c_queue and destroy it
  # Parameters:
	c_queue - a pointer to c_queue 
  # Return Value:
	None
##############################################################################*/
void CQueueDestroy(c_queue_t *c_queue);

/*#############################################################################
  # Description: 
	get a circular queue and reads number of bytes from it
  # Parameters:
	c_queue - a pointer to c_queue
	number_of_bytes - number of bytes to read from the circular queueer
  # Return Value:
	number of bytes that we read
	returns -1 in case of failure
##############################################################################*/
int CQueueEnqueue(c_queue_t *c_queue, void *data);

/*#############################################################################
  # Description: 
	get a circular queue and writes number of bytes to it
  # Parameters:
	c_queue - a pointer to c_queue
	number_of_bytes - number of bytes to write to the circular queueer
  # Return Value:
	number of bytes that were written to the circular queueer
	returns -1 in case of failure
##############################################################################*/
void *CQueueDequeue(c_queue_t *c_queue, void *data);

/*#############################################################################
  # Description: 
	get a circular queue and check if it's empty
  # Parameters:
	c_queue - a pointer to c_queue
  # Return Value:
	1 if it's empty and 0 otherwise
##############################################################################*/
int CQueueIsEmpty(const c_queue_t *c_queue);

#endif /* ILRD_c_queue_H */
