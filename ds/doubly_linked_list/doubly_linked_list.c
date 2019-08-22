/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 21/08/2019
**** Reviewer: Hay
**** Description: This file contains the implementations of functions
				  for the data structure doubly linked list.
				  Look at doubly_linked_list.h for more information about the 
				  functions.
*******************************************************************************/

#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "doubly_linked_list.h" /*dlist_t dlist_node_t dlist_iter_t*/

struct dlist_node
{
	void *data;
	struct dlist_node *next_node;
	struct dlist_node *prev_node;
};

struct dlist
{
	dlist_node_t *dummy_head;
	dlist_node_t *dummy_tail;
};

static dlist_node_t *DlistNodeCreate(void *data, dlist_node_t *next_node, 
									dlist_node_t *prev_node)
{
	dlist_node_t *new_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if(NULL == new_node)
	{
		return NULL;
	}

	new_node->data = data;
	new_node->next_node = next_node;
	new_node->prev_node = prev_node;

	return new_node;
}


dlist_t *DlistCreate(void)
{
	dlist_t *new_dlist = (dlist_t *)malloc(sizeof(dlist_t));
	if (NULL == new_dlist)
	{
		return NULL;
	}

	new_dlist->dummy_head = DlistNodeCreate(NULL, NULL, NULL);
	if (NULL == new_dlist->dummy_head)
	{
		free(new_dlist); 
		new_dlist = NULL;
		return NULL;
	}

	new_dlist->dummy_tail = DlistNodeCreate(NULL, NULL, new_dlist->dummy_head);
	if (NULL == new_dlist->dummy_head)
	{
		free(new_dlist);
		free(new_dlist->dummy_head);
		new_dlist = NULL;
		new_dlist->dummy_head =  NULL;
		return NULL;
	}

	new_dlist->dummy_head->next_node = DlistEnd(new_dlist);

	return new_dlist;
}

static void DestroyDlistStruct(dlist_t *dlist)
{
	free(dlist->dummy_head);
	dlist->dummy_head = NULL;

	free(dlist->dummy_tail);
	dlist->dummy_tail = NULL;

	free(dlist);
	dlist = NULL;
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

void DlistDestroy(dlist_t *dlist)
{
	assert(dlist != NULL);

	DestroyDlistNodes(dlist);
	DestroyDlistStruct(dlist);
}

dlist_iter_t DlistIterNext(dlist_iter_t current)
{
	assert(NULL != current);

	return (current->next_node);
}

dlist_iter_t DlistIterPrev(dlist_iter_t current)
{
	assert(NULL != current);

	return (current->prev_node);
}

dlist_iter_t DlistBegin(dlist_t *dlist)
{
	assert(NULL != dlist);
	assert(NULL != dlist->dummy_head);

	return DlistIterNext(dlist->dummy_head);
}

dlist_iter_t DlistEnd(dlist_t *dlist)
{
	assert(NULL != dlist);

	return (dlist->dummy_tail);
}

void *DlistGetData(dlist_iter_t element)
{
	assert(NULL != element);

	return (element->data);
}

int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);

	return (iter1 == iter2);
}

dlist_iter_t DlistInsert(dlist_t *dlist, dlist_iter_t current, void *data)
{
	dlist_iter_t insert_node = NULL;

	assert(NULL != dlist);
	assert(NULL != current);
	assert(NULL != current->prev_node);

	insert_node = DlistNodeCreate(data, current, DlistIterPrev(current));
	if (NULL == insert_node)
	{
		return DlistEnd(dlist);
	}

	current->prev_node->next_node = insert_node;
	current->prev_node = insert_node;

	return insert_node;
}

dlist_iter_t DlistRemove(dlist_iter_t current)
{
	dlist_iter_t removed_node_next = NULL;

	assert(NULL != current);
	assert(NULL != current->prev_node);
	assert(NULL != current->next_node);

	removed_node_next = DlistIterNext(current);
	current->prev_node->next_node = DlistIterNext(current);
	current->next_node->prev_node = DlistIterPrev(current);

	current->next_node = NULL;
	current->prev_node = NULL;
	current->data = NULL;

	free(current);
	current = NULL;

	return removed_node_next;
}

dlist_iter_t DlistPushFront(dlist_t *dlist, void *data)
{
	assert(NULL != dlist);

	return DlistInsert(dlist, DlistBegin(dlist), data);;
}

dlist_iter_t DlistPushBack(dlist_t *dlist, void *data)
{
	assert(NULL != dlist);

	return DlistInsert(dlist, DlistEnd(dlist), data);
}

void* DlistPopFront(dlist_t *dlist)
{
	dlist_iter_t popped_node = NULL;
	void *popped_node_data = NULL;

	assert(!DlistIsEmpty(dlist));
	assert(NULL != dlist);
	assert(NULL != dlist->dummy_head);

	popped_node = DlistBegin(dlist);
	popped_node_data = DlistGetData(popped_node);
	DlistRemove(popped_node);

	return popped_node_data;
}

void* DlistPopBack(dlist_t *dlist)
{
	dlist_iter_t popped_node = NULL;
	void *popped_node_data = NULL;

	assert(!DlistIsEmpty(dlist));
	assert(NULL != dlist);

	popped_node = DlistIterPrev(DlistEnd(dlist));
	popped_node_data = DlistGetData(popped_node);
	DlistRemove(popped_node);

	return popped_node_data;
}

int DlistForEach(dlist_iter_t from, dlist_iter_t to,
				 action_func_t action_func, void *param_for_action_func)
{
	int action_func_returned_status = 0;
	dlist_iter_t current_iterator = from;

	assert(NULL != action_func);
	assert(NULL != to);
	assert(NULL != from);

	for (current_iterator = from; 
		!DlistIsSameIter(to, current_iterator) && 
		(0 == action_func_returned_status); 
		current_iterator = DlistIterNext(current_iterator))
	{
		action_func_returned_status = action_func(DlistGetData(current_iterator), 
												  param_for_action_func);
	}

	return action_func_returned_status;
}

int DlistIsEmpty(const dlist_t *dlist)
{
	return (DlistIsSameIter(DlistBegin((dlist_t *)dlist),
							DlistEnd((dlist_t *)dlist)));
}

static int AddOneToParameter(void *element_data, void *parameter)
{
	assert(NULL != parameter);
	assert(NULL != element_data);

	++(*(size_t *)parameter);
	(void)element_data;

	return 0;
}

size_t DlistSize(const dlist_t *dlist)
{
	size_t count_list_elements = 0;

	assert(NULL != dlist);

	DlistForEach(DlistBegin((dlist_t *)dlist), DlistEnd((dlist_t *)dlist),
				 AddOneToParameter, &count_list_elements);

	return count_list_elements;
}

dlist_iter_t DlistFind(const dlist_t *list,
					   dlist_iter_t from, dlist_iter_t to, 
					   match_func_t match_func, const void *data)
{
	dlist_iter_t current_iterator = from;

	assert(NULL != match_func);
	assert(NULL != list);
	assert(NULL != to);
	assert(NULL != from);

	for (current_iterator = from; 
		 !DlistIsSameIter(to, current_iterator); 
		 current_iterator = DlistIterNext(current_iterator))
	{
		if (0 == match_func(data, DlistGetData(current_iterator)))
		{
			return current_iterator;
		}
	}

	return DlistEnd((dlist_t *)list);
}

dlist_iter_t DlistSplice(dlist_iter_t dest, dlist_iter_t src_from, 
						 dlist_iter_t src_to)
{
	assert(NULL != dest);
	assert(NULL != src_from);
	assert(NULL != src_to);
	assert(NULL != dest->prev_node);
	assert(NULL != src_to->prev_node);
	assert(NULL != src_from->prev_node);

	dest->prev_node->next_node = src_from;
	dest->prev_node = src_to;

	src_to->prev_node->next_node = dest;
	src_to->prev_node = DlistIterPrev(src_from);

	src_from->prev_node->next_node = src_to;
	src_from->prev_node = DlistIterPrev(dest);


	return src_from;
}
