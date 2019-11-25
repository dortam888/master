#ifndef ILRD_QUEUE_H
#define ILRD_QUEUE_H


typedef struct queue queue_t;

/*#############################################################################
  # Description: 
	creates the queue 
  # Parameters:
	None
  # Return Value:
	in case of success returns a pointer to the queue itself,
	in case of failure returns NULL
##############################################################################*/
queue_t *QueueCreate();

/*#############################################################################
  # Description: 
	gets a queue and destroys it
  # Parameters:
	queue - the pointer to the queue to be destroyed 
  # Return Value:
	None
##############################################################################*/
void QueueDestroy(queue_t *queue);

/*#############################################################################
  # Description: 
	returns how many elements are currently inside the queue
  # Parameters:
	queue - the pointer to the queue 
  # Return Value:
	number of elements in the queue
##############################################################################*/
size_t QueueSize(const queue_t *queue);

/*#############################################################################
  # Description: 
	checks if the queue is empty
  # Parameters:
	queue - the pointer to the queue 
  # Return Value:
	1 if there are no elements in the queue 0 otherwise
##############################################################################*/
int QueueIsEmpty(const queue_t *queue);

/*#############################################################################
  # Description: 
	adds data element to the end of the queue
  # Parameters:
	queue - a pointer to the queue which the data will be added to
	data - address of data to store in the queue
  # Return Value:
	returns 0 if the data been added successefuly, otherwise returns other
	integer number
##############################################################################*/
int QueueEnqueue(queue_t *queue, void *data);

/*#############################################################################
  # Description: 
	remove the first data element in the queue - if the queue is empty the 
	behavior will be undefined
  # Parameters:
	queue - the pointer to the queue which the data will be removed from
  # Return Value:
	None
##############################################################################*/
void QueueDequeue(queue_t *queue);

/*#############################################################################
  # Description: 
	gets a pointer to a queue and returns the data of the first element in the 
	queue (the one that been added first) - if the queue is empty the 
	behavior will be undefined
  # Parameters:
	queue - the pointer to the queue 
  # Return Value:
	the first element in the queue (the one that been added first)
##############################################################################*/
void* QueuePeek(const queue_t *queue);

/*#############################################################################
  # Description: 
	gets two pointers to two different queues and appends the second queue to
	the first one
  # Parameters:
	dest_queue - pointer to the first queue (destination)
	src_queue - pointer to the second queue (source)
  # Return Value:
	returns a pointer to the destination queue
##############################################################################*/
queue_t *QueueAppend(queue_t *dest_queue, queue_t *src_queue);

#endif /* ILRD_QUEUE_H */










