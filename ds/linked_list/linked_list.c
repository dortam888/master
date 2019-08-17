/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 17/08/2019
**** Reviewer: Daniel
**** Description: This file contains the implementations of functions
				  for the data structure singly linked list.
				  Look at linked list.h for more information about the 
				  functions.
*******************************************************************************/

#include <assert.h> /*assert*/
#include <stdlib.h> /* malloc free */

#include "linked_list.h" /* slist_node_t */

enum {FOUND, NOT_FOUND};
enum loop_indicator {LOOP_NOT_FOUND, LOOP_FOUND};

slist_node_t *SlistCreateNode(void *data, slist_node_t *next_node)
{
	slist_node_t *new_node = malloc(sizeof(slist_node_t));
	if(NULL == new_node)
	{
		return NULL;
	}

	new_node->data = data;
	new_node->next_node = next_node;

	return new_node;
}

static void SwapPointers(void **ptr1, void **ptr2)
{
	void *tmp_ptr1 = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp_ptr1;
}

slist_node_t *SlistInsertNode(slist_node_t *current_node,
														 slist_node_t *new_node)
{
	assert(NULL != current_node);
	assert(NULL != new_node);

	/*pointer invalidation. new_node next will be current next but their data will be swapped*/
	new_node->next_node = current_node->next_node;
	current_node->next_node = new_node;
	SwapPointers(&current_node->data, &new_node->data);

	return current_node; /*current_node data became new_node data*/
}

slist_node_t *SlistInsertAfterNode(slist_node_t *current_node,
														 slist_node_t *new_node)
{
	assert(NULL != current_node);
	assert(NULL != new_node);

	new_node->next_node = current_node->next_node;
	current_node->next_node = new_node;

	return new_node;
}

void SlistFreeAll(slist_node_t *head)
{
	if (NULL != head)
	{
		slist_node_t *tmp = head->next_node;
		free(head);
		head = NULL;
		SlistFreeAll(tmp);
	}
}

slist_node_t *SlistRemove(slist_node_t *current_node)
{
	slist_node_t *removed_node = NULL;

	assert(NULL != current_node);

	/*pointer invalidation. the removed node will be the next node but the data will be swapped*/
	removed_node = current_node->next_node;
	SwapPointers(&current_node->data, &removed_node->data); /* swap datas */
	current_node->next_node = removed_node->next_node;
	removed_node->next_node = NULL;

	return removed_node;
}

slist_node_t *SlistRemoveAfter(slist_node_t *current_node)
{
	slist_node_t *removed_node = NULL;

	assert(NULL != current_node);

	removed_node = current_node->next_node;
	current_node->next_node = current_node->next_node->next_node;
	removed_node->next_node = NULL;

	return removed_node;
}

size_t SlistCount(const slist_node_t *head)
{
	size_t node_counter = 0;

	while (NULL != head)
	{
		head = head->next_node;
		++node_counter;
	}

	return node_counter;
}

slist_node_t *SlistFind(const slist_node_t *head, const void *data, 
												  match_func_t match_func)
{
	int is_data_in_linked_list = NOT_FOUND;
	const slist_node_t *current_node = head;

	assert(NULL != match_func);

	while (NULL != current_node)
	{
		is_data_in_linked_list = match_func(data, current_node->data);

		if (FOUND == is_data_in_linked_list)
		{
			return (slist_node_t *)current_node;
		}

		current_node = current_node->next_node;
	}

	return NULL;
}

int SlistForEach(slist_node_t *head, action_func_t action_func, 
									 void *param_for_action_func)
{
	int action_func_returned_status = 0;

	assert(NULL != action_func);

	while (NULL != head && 0 == action_func_returned_status)
	{
		action_func_returned_status = action_func(head->data, 
												  param_for_action_func);
		head = head->next_node;
	}

	return action_func_returned_status;
}

slist_node_t *SlistFlip(slist_node_t *head)
{
	slist_node_t *current_node = head;
	slist_node_t *prev_node = NULL; /* NULL for the first flip */

	while (current_node != NULL)
	{
		slist_node_t *next_node = current_node->next_node; /*save next node*/

		current_node->next_node = prev_node; /*update current next to prev*/
		/* move pointers one node ahead */
		prev_node = current_node; 
		current_node = next_node;
	}

	return prev_node;
}

int SlistHasLoops(const slist_node_t *head)
{
	const slist_node_t *move_one_node_next = head;
	const slist_node_t *move_two_nodes_next = head;

	assert(NULL != head);

	while ((NULL != move_two_nodes_next) && 
		   (NULL != move_two_nodes_next->next_node))
	{
		move_one_node_next = move_one_node_next->next_node;
		move_two_nodes_next = move_two_nodes_next->next_node->next_node;

		if (move_one_node_next == move_two_nodes_next)
		{
			return LOOP_FOUND;
		}
	}

	return LOOP_NOT_FOUND;
}

slist_node_t *SlistFindIntersection(const slist_node_t *head1,
									const slist_node_t *head2)
{
	size_t number_of_nodes_in_list1 = 0;
	size_t number_of_nodes_in_list2 = 0;
	const slist_node_t *current_node_list1 = head1;
	const slist_node_t *current_node_list2 = head2;

	assert(NULL != head1);
	assert(NULL != head2);

	number_of_nodes_in_list1 = SlistCount(head1);
	number_of_nodes_in_list2 = SlistCount(head2);

	while (number_of_nodes_in_list1 > number_of_nodes_in_list2)
	{
		current_node_list1 = current_node_list1->next_node;
		--number_of_nodes_in_list1;
	}

	while (number_of_nodes_in_list2 > number_of_nodes_in_list1)
	{
		current_node_list2 = current_node_list2->next_node;
		--number_of_nodes_in_list2;
	}

	while (current_node_list2 != current_node_list1)
	{
		current_node_list1 = current_node_list1->next_node;
		current_node_list2 = current_node_list2->next_node;
	}

	return (slist_node_t *)current_node_list1;
}
