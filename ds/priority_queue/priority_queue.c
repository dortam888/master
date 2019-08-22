/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 22/08/2019
**** Reviewer: May
**** Description: This file contains the implementations of functions
				  for the data structure doubly linked list.
				  Look at doubly_linked_list.h for more information about the 
				  functions.
*******************************************************************************/

#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "sorted_list.h" /*sorted_list_iter_t sorted_list_t*/
#include "priority_queue.h"

struct pq
{
	sorted_list_t *sorted_list;
	cmp_func_t cmp_func;
	void *param_for_cmp;
};

pq_t *PriorityQCreate(cmp_func_t cmp_func, void *param_for_cmp)
{
	pq_t *new_priority_queue = malloc(sizeof(pq_t));
	if (NULL == new_priority_queue)
	{
		return NULL;
	}

	new_priority_queue->cmp_func = cmp_func;
	new_priority_queue->param_for_cmp = param_for_cmp;
	new_priority_queue->sorted_list = SortedListCreate(cmp_func, param_for_cmp);
	if (NULL == new_priority_queue->sorted_list)
	{
		free(new_priority_queue);
		new_priority_queue = NULL;
		return NULL;
	}

	return new_priority_queue;
}

void PriorityQDestroy(pq_t *pq)
{
	assert(NULL != pq);

	SortedListDestroy(pq->sorted_list);
	pq->sorted_list = NULL;
	pq->cmp_func = NULL;
	pq->param_for_cmp = NULL;

	free(pq);
	pq = NULL;
}

size_t PriorityQSize(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListSize(pq->sorted_list);
}

int PriorityQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListIsEmpty(pq->sorted_list);
}

int PriorityQEnqueue(pq_t *pq, void *data)
{
	sorted_list_iter_t i = {NULL};

	i = SortedListInsert(pq->sorted_list, data);
	if (SortedListIsSameIter(SortedListEnd(pq->sorted_list),i))
	{
		return 1;
	}

	return 0;
}

void PriorityQDequeue(pq_t *pq)
{
	assert(NULL != pq);

	SortedListPopFront(pq->sorted_list);
}

void* PriorityQPeek(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListPopFront(pq->sorted_list);
}

static void DestroyDlistNodes(dlist_t *dlist)
{
	dlist_iter_t current_in_list = NULL;
	dlist_iter_t list_end = NULL;
	dlist_iter_t next = NULL;

	assert(dlist != NULL);

	current_in_list = DlistBegin(dlist);
	list_end = DlistEnd(dlist);

	while (!DlistIsSameIter(current_in_list,list_end))
	{
		next = DlistIterNext(current_in_list);
		free(current_in_list);
		current_in_list = next;
	}
}

void PriorityQClear(pq_t *pq)
{
	DestroyDlistNodes(pq->sorted_list->dlist);
}


