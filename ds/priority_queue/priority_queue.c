/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 22/08/2019
**** Reviewer: Hay
**** Description: This file contains the implementations of functions
				  for the data structure doubly linked list.
				  Look at doubly_linked_list.h for more information about the 
				  functions.
*******************************************************************************/

#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "sorted_list.h" /*sorted_list_iter_t sorted_list_t*/
#include "priority_queue.h" /*pq_t*/

typedef struct parameters_for_pq
{
	cmp_func_t cmp_func;
	void *param_for_cmp;
}parameters_for_pq_t;

struct pq
{
	sorted_list_t *sorted_list;
	parameters_for_pq_t *param_for_pq;
};


static int Cmp2IsBeforeFunc(const void *data1, const void *data2, 
							void *param_for_pq)
{
	parameters_for_pq_t *new_param_for_pq = (parameters_for_pq_t *)param_for_pq;
	
	assert(NULL != data1);
	assert(NULL != data2);
	assert(NULL != param_for_pq);
	
	return (new_param_for_pq->cmp_func(data1, data2, 
			new_param_for_pq->param_for_cmp) > 0);
}

pq_t *PriorityQCreate(cmp_func_t cmp_func, void *param_for_cmp)
{
	pq_t *new_pq = malloc(sizeof(pq_t));
	if (NULL == new_pq)
	{
		return NULL;
	}

	new_pq->param_for_pq = malloc(sizeof(parameters_for_pq_t));
	if (NULL == new_pq->param_for_pq)
	{
		free(new_pq); new_pq = NULL;
		return NULL;
	}

	new_pq->param_for_pq->cmp_func = cmp_func;
	new_pq->param_for_pq->param_for_cmp = param_for_cmp;

	new_pq->sorted_list = SortedListCreate(Cmp2IsBeforeFunc, 
										   new_pq->param_for_pq);
	if (NULL == new_pq->sorted_list)
	{
		free(new_pq);
		new_pq = NULL;
		return NULL;
	}

	return new_pq;
}

void PriorityQDestroy(pq_t *pq)
{
	assert(NULL != pq);

	SortedListDestroy(pq->sorted_list);
	pq->sorted_list = NULL;

	pq->param_for_pq->cmp_func = NULL;
	pq->param_for_pq->param_for_cmp = NULL;
	free(pq->param_for_pq);
	pq->param_for_pq = NULL;

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
	sorted_list_iter_t inserted_node = {NULL};
	sorted_list_t *pq_sorted = NULL;
	
	assert(NULL != pq);
	
	pq_sorted = pq->sorted_list;

	inserted_node = SortedListInsert(pq_sorted, data);

	return (SortedListIsSameIter(SortedListEnd(pq_sorted), inserted_node));
}

void PriorityQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	assert(!PriorityQIsEmpty(pq));

	SortedListPopFront(pq->sorted_list);
}

void* PriorityQPeek(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListGetData(SortedListBegin(pq->sorted_list));
}

void PriorityQClear(pq_t *pq)
{
	sorted_list_iter_t current_in_list = {NULL};
	sorted_list_iter_t list_end = {NULL};
	sorted_list_t *pq_sorted = NULL;

	assert(pq != NULL);

	pq_sorted = pq->sorted_list;
	current_in_list = SortedListBegin(pq_sorted);
	list_end = SortedListEnd(pq_sorted);

	while (!SortedListIsSameIter(current_in_list,list_end))
	{
		current_in_list = SortedListRemove(current_in_list);
	}
}

void *PriorityQErase(pq_t *pq, void *data_to_erase, is_match_t is_match)
{
	sorted_list_t *pq_sorted = NULL;
	sorted_list_iter_t current_in_list = {NULL};
	sorted_list_iter_t list_end = {NULL};
	
	assert(NULL != pq);
	
	pq_sorted = pq->sorted_list;
	list_end = SortedListEnd(pq_sorted);
	
	for(current_in_list = SortedListBegin(pq_sorted); 
		!SortedListIsSameIter(current_in_list,list_end);
		current_in_list = SortedListIterNext(current_in_list))
	{
		if (is_match(data_to_erase, SortedListGetData(current_in_list)))
		{
			void *data_to_return = SortedListGetData(current_in_list);
			SortedListRemove(current_in_list);
			return data_to_return;
		}
	}

	return NULL;
}

