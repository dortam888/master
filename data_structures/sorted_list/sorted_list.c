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

struct sorted_list
{
	dlist_t *dlist;
	is_before_t is_before;
	void *param_is_before;
};

/***************************SortedListCreate***********************************/
sorted_list_t *SortedListCreate(is_before_t is_before, void *param)
{
	sorted_list_t *new_sorted_list = 
	(sorted_list_t *)malloc(sizeof(sorted_list_t));
	if (NULL == new_sorted_list)
	{
		return NULL;
	}

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

/***************************SortedListDestroy**********************************/
void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	
	DlistDestroy(sorted_list->dlist);
	sorted_list->dlist = NULL;
	sorted_list->is_before = NULL;

	free(sorted_list); sorted_list = NULL;
}

/***************************SortedListIterBegin********************************/
sorted_list_iter_t SortedListBegin(sorted_list_t *sorted_list)
{
	sorted_list_iter_t begin = {NULL};

	assert(NULL != sorted_list);

	begin.dlist_iter = DlistBegin(sorted_list->dlist);

	return begin;
}

/***************************SortedListIterEnd**********************************/
sorted_list_iter_t SortedListEnd(sorted_list_t *sorted_list)
{
	sorted_list_iter_t end = {NULL};

	assert(NULL != sorted_list);

	end.dlist_iter = DlistEnd(sorted_list->dlist);

	return end;
}

/***************************SortedListIterNext*********************************/
sorted_list_iter_t SortedListIterNext(sorted_list_iter_t current)
{
	sorted_list_iter_t next = {NULL};

	next.dlist_iter = DlistIterNext(current.dlist_iter);

	return next;
}

/***************************SortedListIterPrev*********************************/
sorted_list_iter_t SortedListIterPrev(sorted_list_iter_t current)
{
	sorted_list_iter_t prev = {NULL};

	prev.dlist_iter = DlistIterPrev(current.dlist_iter);

	return prev;
}

/***************************SortedListIsGetData********************************/
void *SortedListGetData(sorted_list_iter_t element)
{
	return DlistGetData(element.dlist_iter);
}

/***************************SortedListIsSameIter*******************************/
int SortedListIsSameIter(sorted_list_iter_t iter1, sorted_list_iter_t iter2)
{
	return DlistIsSameIter(iter1.dlist_iter, iter2.dlist_iter);
}

/***************************SortedListInsert***********************************/
sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t i = {NULL};
	sorted_list_iter_t inserted_node = {NULL};
	sorted_list_iter_t end_list = {NULL};

	assert(NULL != sorted_list);

	end_list = SortedListEnd(sorted_list);

	for (i = SortedListBegin(sorted_list);
		 !SortedListIsSameIter(i, end_list) &&
		 !sorted_list->is_before(SortedListGetData(i), 
								 data, sorted_list->param_is_before);
		 i = SortedListIterNext(i))
	{}

	inserted_node.dlist_iter = DlistInsert(sorted_list->dlist, 
										   i.dlist_iter, data);

	return inserted_node;
}

/***************************SortedListRemove***********************************/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t current)
{
	sorted_list_iter_t next_to_removed_node = {NULL};

	next_to_removed_node.dlist_iter = DlistRemove(current.dlist_iter);

	return next_to_removed_node;
}

/***************************SortedListPopFront*********************************/
void* SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistPopFront(sorted_list->dlist);
}

/***************************SortedListPopBack**********************************/
void* SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistPopBack(sorted_list->dlist);
}

/***************************SortedListSize*************************************/
size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistSize(sorted_list->dlist);
}

/***************************SortedListIsEmpty**********************************/
int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DlistIsEmpty(sorted_list->dlist);
}

static int IsDataEqual(const sorted_list_t *list, const void *data, 
					   sorted_list_iter_t node)
{
	is_before_t list_is_before = NULL;
	void *list_param = NULL;
	void *user_data = NULL;
	
	assert(NULL != list);
	assert(NULL != data);
	
	list_is_before = list->is_before;
	list_param = list->param_is_before;
	user_data = SortedListGetData(node);

	return (list_is_before(data, user_data, list_param) == 
			(list_is_before(user_data, data, list_param)));
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
		if (IsDataEqual(list, data, same_data_node))
		{
			return same_data_node;
		}
	}

	return SortedListEnd((sorted_list_t *)list);
}

/***************************SortedListFindIf***********************************/
sorted_list_iter_t SortedListFindIf(const sorted_list_t *list,
									sorted_list_iter_t from,
									sorted_list_iter_t to,
									match_func_t match_func,
									const void *data)
{
	sorted_list_iter_t same_data_node = {NULL};

	assert(NULL != list);

	same_data_node.dlist_iter = DlistFind(list->dlist, from.dlist_iter, 
	                                      to.dlist_iter, match_func, data);

	return same_data_node;
}

/***************************SortedListForEach**********************************/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
					  action_func_t action_func, void *param_for_action_func)
{
	return DlistForEach(from.dlist_iter, to.dlist_iter,
						action_func, param_for_action_func);
}

/***************************SortedListMerge************************************/
void SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list)
{
	sorted_list_iter_t src = {NULL};
	sorted_list_iter_t next_src = {NULL};
	sorted_list_iter_t next_dest = {NULL};
	sorted_list_iter_t src_end = {NULL};
	sorted_list_iter_t dest_end = {NULL};
	void *dest_param = NULL;
	is_before_t dest_is_before = NULL;

	assert(NULL != dest_list);
	assert(NULL != src_list);

	next_dest = SortedListBegin(dest_list);
	src_end = SortedListEnd(src_list);
	dest_end = SortedListEnd(dest_list);
	dest_param = dest_list->param_is_before;
	dest_is_before = dest_list->is_before;

	for (src = SortedListBegin(src_list); 
		 !SortedListIsSameIter(src, src_end);
		 src = next_src)
	{
		for (; 
		     !SortedListIsSameIter(next_dest, dest_end) &&
			 !dest_is_before(SortedListGetData(next_dest), 
								   SortedListGetData(src), 
								   dest_param);
		     next_dest = SortedListIterNext(next_dest))
		{}

		next_src = SortedListIterNext(src);
		DlistSplice(next_dest.dlist_iter, 
					src.dlist_iter, DlistIterNext(src.dlist_iter));
	}

}
