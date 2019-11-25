/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Thursday August 22 2019 04:09:15 PM */
/* This file contains functions for priority_queue data structure operations.*/
/* Can be used for priority_queue based algorithms */
/*############################################################################*/

#ifndef ILRD_PRIORITY_QUEUE_H
#define ILRD_PRIORITY_QUEUE_H
#include <stddef.h> /* size_t */

typedef struct pq pq_t;

/*#############################################################################
  # Description: 
	creates the priority queue 
  # Parameters:
	cmp_func - function for prioritize insertion
	param_for_cmp - parameter for prioritize function
  # Return Value:
	in case of success returns a pointer to the queue itself,
	in case of failure returns NULL
##############################################################################*/
typedef int (*cmp_func_t)(const void *data1, const void *data2, void *param);

pq_t *PriorityQCreate(cmp_func_t cmp_func, void *param_for_cmp);

/*#############################################################################
  # Description: 
	gets a priority queue and destroys it
  # Parameters:
	pq - pointer to the priority queue to be destroyed 
  # Return Value:
	None
##############################################################################*/
void PriorityQDestroy(pq_t *pq);

/*#############################################################################
  # Description: 
	returns how many elements are currently inside the queue
  # Parameters:
	pq - the pointer to the priority queue 
  # Return Value:
	number of elements in the queue
##############################################################################*/
size_t PriorityQSize(const pq_t *pq);

/*#############################################################################
  # Description: 
	checks if the queue is empty
  # Parameters:
	pq - the pointer to the priority queue 
  # Return Value:
	1 if there are no elements in the queue 0 otherwise
##############################################################################*/
int PriorityQIsEmpty(const pq_t *pq);

/*#############################################################################
  # Description: 
	adds data element to the priority queue
  # Parameters:
	pq - a pointer to the priority queue which the data will be added to
	data - address of data to store in the queue
  # Return Value:
	returns 0 if the data been added successefuly, otherwise returns other
	integer number
##############################################################################*/
int PriorityQEnqueue(pq_t *pq, void *data);

/*#############################################################################
  # Description: 
	remove the first data element in the queue - if the queue is empty the 
	behavior will be undefined
  # Parameters:
	pq - the pointer to the priority queue which the data will be removed from
  # Return Value:
	None
##############################################################################*/
void PriorityQDequeue(pq_t *pq);

/*#############################################################################
  # Description: 
	gets a pointer to a queue and returns the data of the first element in the 
	queue (the one that been added first) - if the queue is empty the 
	behavior will be undefined
  # Parameters:
	pq - the pointer to the priority queue 
  # Return Value:
	the first element in the priority queue (the one that been added first)
##############################################################################*/
void* PriorityQPeek(const pq_t *pq);

/*#############################################################################
  # Description: 
	gets a priority queue and clears all valid elements in it.
  # Parameters:
	pq - pointer to the priority queue
  # Return Value:
	none
##############################################################################*/
void PriorityQClear(pq_t *pq);

/*#############################################################################
  # Description: 
	gets a priority queue and data to erase from it
  # Parameters:
	pq - pointer to the priority queue
	data - data to compare
  # Return Value:
	none
##############################################################################*/
typedef int (*is_match_t)(const void *element, const void* data_to_erase);

void *PriorityQErase(pq_t *pq, 
					void *data_to_erase,
					is_match_t is_match);

#endif /* ILRD_PRIORITY_QUEUE_H */
