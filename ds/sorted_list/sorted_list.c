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

#include "sorted_list.h" /*sorted_list_iter_t sorted_list_t*/

struct sorted_list
{
	dlist_t *dlist;
	is_before_t is_before;
	void *param_is_before;
};

sorted_list_t *SortedListCreate(is_before_t is_before, void *param)
{
	sorted_list_t *new_sorted_list = 
	(sorted_list_t *)malloc(sizeof(sorted_list_t));

	new_sorted_list->dlist = DlistCreate();
	if(NULL == new_sorted_list->dlist)
	{
		free(new_sorted_list);
		new_sorted_list = NULL;
		return NULL;
	}

	new_sorted_list->is_before = is_before;
	new_sorted_list->param_is_before = param;

	return new_sorted_list;
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
	DlistDestroy(sorted_list->dlist);
	sorted_list->dlist = NULL;
	sorted_list->is_before = NULL;

	free(sorted_list); sorted_list = NULL;
}

sorted_list_iter_t SortedListBegin(sorted_list_t *sorted_list)
{
	sorted_list_iter_t begin = {NULL};

	assert(NULL != sorted_list);

	begin.dlist_iter = DlistBegin(sorted_list->dlist);

	return begin;
}

sorted_list_iter_t SortedListEnd(sorted_list_t *sorted_list)
{
	sorted_list_iter_t end = {NULL};

	assert(NULL != sorted_list);

	end.dlist_iter = DlistEnd(sorted_list->dlist);

	return end;
}

sorted_list_iter_t SortedListIterNext(sorted_list_iter_t current)
{
	sorted_list_iter_t next = {NULL};

	next.dlist_iter = DlistIterNext(current.dlist_iter);

	return next;
}

sorted_list_iter_t SortedListIterPrev(sorted_list_iter_t current)
{
	sorted_list_iter_t prev = {NULL};

	prev.dlist_iter = DlistIterPrev(current.dlist_iter);

	return prev;
}

void *SortedListGetData(sorted_list_iter_t element)
{
	return DlistGetData(element.dlist_iter);
}

int SortedListIsSameIter(sorted_list_iter_t iter1, sorted_list_iter_t iter2)
{
	return DlistIsSameIter(iter1.dlist_iter, iter2.dlist_iter);
}

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t i = {NULL};
	sorted_list_iter_t inserted_node = {NULL};

	assert(NULL != sorted_list);
 
	for (i = SortedListBegin(sorted_list);
		 !SortedListIsSameIter(i, SortedListEnd(sorted_list)) &&
		 !sorted_list->is_before(SortedListGetData(i), 
								 data, sorted_list->param_is_before);
		 i = SortedListIterNext(i))
	{}

	inserted_node.dlist_iter = DlistInsert(sorted_list->dlist, 
										   i.dlist_iter, data);

	return inserted_node;
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t current)
{
	sorted_list_iter_t removed_node = {NULL};

	removed_node.dlist_iter = DlistRemove(current.dlist_iter);

	return removed_node;
}

void* SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistPopFront(sorted_list->dlist);
}

void* SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistPopBack(sorted_list->dlist);
}

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistSize(sorted_list->dlist);
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistIsEmpty(sorted_list->dlist);
}

/***************************SortedListFind*************************************/
sorted_list_iter_t SortedListFind(const sorted_list_t *list,
								  sorted_list_iter_t from,
								  sorted_list_iter_t to,
								  const void *data)
{
	sorted_list_iter_t same_data_node = from;

	assert(NULL != list);

	for (same_data_node = from; 
		 !SortedListIsSameIter(to, same_data_node); 
		 same_data_node = SortedListIterNext(same_data_node))
	{
		if (!(list->is_before(data, SortedListGetData(same_data_node), 
			list->param_is_before) ^ 
			(list->is_before(SortedListGetData(same_data_node), data, 
			list->param_is_before))))
		{
			return (same_data_node);
		}
	}

	return SortedListEnd((sorted_list_t *)list);
}

sorted_list_iter_t SortedListFindIf(const sorted_list_t *list,
									sorted_list_iter_t from,
									sorted_list_iter_t to,
									match_func_t match_func,
									const void *data)
{
	sorted_list_iter_t same_data_node = {NULL};

	assert(NULL != list);

	same_data_node.dlist_iter = DlistFind(list->dlist, from.dlist_iter, to.dlist_iter, 
										  match_func, data);

	return same_data_node;
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
					  action_func_t action_func, void *param_for_action_func)
{
	return DlistForEach(from.dlist_iter, to.dlist_iter,
						action_func, param_for_action_func);
}

void SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list)
{
	sorted_list_iter_t i = {NULL};
	sorted_list_iter_t next_src = {NULL};
	sorted_list_iter_t next_dest = {NULL};

	assert(NULL != dest_list);
	assert(NULL != src_list);

	next_dest = SortedListBegin(dest_list);

	for (i = SortedListBegin(src_list); 
		 !SortedListIsSameIter(i, SortedListEnd(src_list));
		 i = next_src)
	{
		for (; 
		     !SortedListIsSameIter(next_dest, SortedListEnd(dest_list)) &&
			 !dest_list->is_before(SortedListGetData(next_dest), 
								   SortedListGetData(i), 
								   dest_list->param_is_before);
		     next_dest = SortedListIterNext(next_dest))
		{}

		next_src = SortedListIterNext(i);
		DlistSplice(next_dest.dlist_iter, 
					i.dlist_iter, DlistIterNext(i.dlist_iter));
	}

}
