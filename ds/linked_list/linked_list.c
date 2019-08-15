/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 15/08/2019
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
enum loop_indicator {LOOP_FOUND, LOOP_NOT_FOUND};

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

static void SwapNodes(slist_node_t **node1, slist_node_t **node2)
{
	slist_node_t *tmp_node1 = *node1;

	assert(NULL != node1);
	assert(NULL != node2);

	tmp_node1 = *node1;
	*node1 = *node2;
	*node2 = tmp_node1;
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

	return current_node; /*which is data became new node*/
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
	SwapPointers(&current_node->data, &current_node->next_node->data);
	SwapNodes(&removed_node, &current_node->next_node->next_node);
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
	int compare_status = NOT_FOUND;
	const slist_node_t *run_on_list = head;

	assert(NULL != head);
	assert(NULL != data);

	while (NULL != run_on_list)
	{
		compare_status = match_func(data, run_on_list->data);

		if (FOUND == compare_status)
		{
			return (slist_node_t *)run_on_list;
		}

		run_on_list = run_on_list->next_node;
	}

	return NULL;
}

int SlistForEach(slist_node_t *head, action_func_t action_func, 
									 void *param_for_action_func)
{
	int action_func_returned_status = 0;

	while (NULL != head && 0 != action_func_returned_status)
	{
		action_func_returned_status = action_func(head->data, 
												  param_for_action_func);
		head = head->next_node;
	}

	return action_func_returned_status;
}

slist_node_t *SlistFlip(slist_node_t *head)
{
	slist_node_t *tmp_head = NULL;

	assert(NULL != head);

	tmp_head = head;

	while (head->next_node != NULL)
	{
		slist_node_t *tmp_head_next = tmp_head->next_node;
		tmp_head->next_node->next_node = tmp_head;
		tmp_head = tmp_head_next;
	}

	head->next_node = NULL;

	return tmp_head;
}

int SlistHasLoops(const slist_node_t *head)
{
	const slist_node_t *move_one_node = NULL;
	const slist_node_t *move_two_nodes = NULL;

	assert(NULL != head);

	move_one_node = head;
	move_two_nodes = head;

	while ((NULL != move_two_nodes) && (NULL != move_two_nodes->next_node))
	{
		move_one_node = move_one_node->next_node;
		move_two_nodes = move_two_nodes->next_node->next_node;

		if (move_one_node == move_two_nodes)
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
	const slist_node_t *run_on_list1 = head1;
	const slist_node_t *run_on_list2 = head2;

	assert(NULL != head1);
	assert(NULL != head2);

	number_of_nodes_in_list1 = SlistCount(head1);
	number_of_nodes_in_list2 = SlistCount(head2);

	while (number_of_nodes_in_list1 > number_of_nodes_in_list2)
	{
		run_on_list1 = run_on_list1->next_node;
		--number_of_nodes_in_list1;
	}


	while (number_of_nodes_in_list2 > number_of_nodes_in_list1)
	{
		run_on_list2 = run_on_list2->next_node;
		--number_of_nodes_in_list2;
	}

	while (run_on_list2 != run_on_list1)
	{
		run_on_list1 = run_on_list1->next_node;
		run_on_list2 = run_on_list2->next_node;
	}

	return (slist_node_t *)run_on_list1;
}
