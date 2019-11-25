/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Sunday August 18 2019 19:39:00 AM
**** Reviewer: Hay
**** Description: This file contains the implementations of functions
				  for the data structure queue.
				  Look at queue.h for more information about the 
				  functions.
*******************************************************************************/

#include <assert.h> /*assert*/
#include <stdlib.h> /* malloc free */

#include "queue.h" /* queue_t */
#include "linked_list.h" /* slist_node_t */

enum {SUCCESS, FAILED_TO_ALLOCATE_MEMORY};

struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
};

queue_t *QueueCreate()
{
	queue_t *queue = NULL;
	slist_node_t *dummy_node = NULL;

	queue = malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		return NULL;
	}

	dummy_node = SlistCreateNode(NULL, NULL);
	if (NULL == dummy_node)
	{
		free(queue);
		queue = NULL;

		return NULL;
	}

	queue->head = dummy_node;
	queue->tail = queue->head;

	return queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	SlistFreeAll(queue->head);
	queue->head = NULL;
	queue->tail = NULL;

	free(queue);
	queue = NULL;
}

size_t QueueSize(const queue_t *queue)
{
	assert(NULL != queue);
	assert(NULL != queue->head);

	return SlistCount((queue->head)->next_node); /*minus dummy node*/
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);

	return (queue->head == queue->tail);
}

int QueueEnqueue(queue_t *queue, void *data)
{
	slist_node_t *new_node = SlistCreateNode(data, NULL);
	if (NULL == new_node)
	{
		return FAILED_TO_ALLOCATE_MEMORY;
	}

	assert(NULL != queue);

	new_node = SlistInsertAfterNode(queue->tail, new_node);
	queue->tail = new_node;

	return SUCCESS;
}

void QueueDequeue(queue_t *queue)
{
	slist_node_t *removed_element = NULL;

	assert(NULL != queue);
	assert(!QueueIsEmpty);

	removed_element = SlistRemoveAfter(queue->head);

	if (0 == QueueSize(queue))
	{
		queue->tail = queue->head;
	}

	SlistFreeAll(removed_element);
}

void* QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	assert(NULL != queue->head);
	assert(NULL != (queue->head)->next_node);

	return ((queue->head)->next_node)->data;
}

queue_t *QueueAppend(queue_t *dest_queue, queue_t *src_queue)
{
	assert(NULL != dest_queue);
	assert(NULL != src_queue);
	assert(NULL != dest_queue->tail);
	assert(NULL != src_queue->head);

	(dest_queue->tail)->next_node = (src_queue->head)->next_node;
	dest_queue->tail = src_queue->tail;
	src_queue->tail = src_queue->head;
	(src_queue->head)->next_node = NULL;

	return dest_queue;
}

