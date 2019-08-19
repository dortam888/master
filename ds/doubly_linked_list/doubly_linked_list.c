#include <stdlib.h>
#include <assert.h>

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
	dlist_node_t *new_node = malloc(sizeof(dlist_node_t));
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
	dlist_t *new_dlist = malloc(sizeof(dlist_t));
	if (NULL == new_dlist)
	{
		return NULL;
	}

	new_dlist->dummy_head = DlistNodeCreate(NULL, NULL, NULL);
	if (NULL != new_dlist->dummy_head)
	{
		return NULL;
	}

	new_dlist->dummy_tail = DlistNodeCreate(NULL, NULL, new_dlist->dummy_head);
	if (NULL != new_dlist->dummy_head)
	{
		return NULL;
	}

	new_dlist->dummy_head->next_node = new_dlist->dummy_tail;

	return new_dlist;
}

void DlistDestroy(dlist_t *dlist)
{
	dlist_iter_t current_in_list = NULL;
	dlist_iter_t list_end = NULL;
	dlist_iter_t next = NULL;

	assert(dlist != NULL);

	current_in_list = DlistBegin(dlist);
	list_end = DlistEnd(dlist);

	while (next != list_end)
	{
		next = DlistIterNext(current_in_list);
		free(current_in_list);
		current_in_list = NULL;
	}

	free(dlist->dummy_head);
	dlist->dummy_head = NULL;

	free(dlist->dummy_tail);
	dlist->dummy_tail = NULL;

	free(dlist);
	dlist = NULL;
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

dlist_iter_t DlistBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	assert(NULL != dlist->dummy_head);

	return DlistIterNext(dlist->dummy_head);
}

dlist_iter_t DlistEnd(const dlist_t *dlist)
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
	dlist_iter_t removed_node = NULL;

	assert(NULL != current);

	removed_node = current;
	current->prev_node->next_node = DlistIterNext(current);
	current->next_node->prev_node = DlistIterPrev(current);
	current->next_node = NULL;
	current->prev_node = NULL;

	return removed_node;
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

	popped_node = DlistRemove(DlistBegin(dlist));
	popped_node_data = DlistGetData(popped_node);

	free(popped_node);

	return popped_node_data;
}

void* DlistPopBack(dlist_t *dlist)
{
	dlist_iter_t popped_node = NULL;
	void *popped_node_data = NULL;

	assert(!DlistIsEmpty(dlist));
	assert(NULL != dlist);
	assert(NULL != dlist->dummy_tail);

	popped_node = DlistRemove(DlistIterPrev(DlistEnd(dlist)));
	popped_node_data = DlistGetData(popped_node);

	free(popped_node);

	return popped_node_data;
}

int DlistForEach(action_func_t action_func, dlist_iter_t to,
				 dlist_iter_t from, void *param_for_action_func)
{
	int action_func_returned_status = 0;
	dlist_iter_t current = from;

	assert(NULL != action_func);

	while (current != to && 0 == action_func_returned_status)
	{
		action_func_returned_status = action_func(DlistGetData(current), 
												  param_for_action_func);
		current = DlistIterNext(current);
	}

	return action_func_returned_status;
}

int DlistIsEmpty(const dlist_t *dlist)
{
	return (DlistBegin(dlist) == DlistEnd(dlist));
}

static int Count(void *element_data, void *counter)
{
	++(*(size_t *)counter);
	(void)element_data;

	return 0;
}

size_t DlistSize(const dlist_t *dlist)
{
	size_t count_list_elements = 0;

	DlistForEach(Count, DlistBegin(dlist), DlistEnd(dlist), 
				 &count_list_elements);

	return count_list_elements;
}

dlist_iter_t DlistFind(const void *data, dlist_iter_t to, 
					   dlist_iter_t from, match_func_t match_func)
{
	dlist_iter_t current_iterator = from;

	assert(NULL != match_func);

	while (to != current_iterator)
	{
		if (0 == match_func(data, DlistGetData(current_iterator)))
		{
			return current_iterator;
		}

		current_iterator = DlistIterNext(current_iterator);
	}

	return NULL;
}
