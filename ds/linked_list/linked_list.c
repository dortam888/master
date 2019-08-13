#include "linked_list.h"
#include <assert.h> /*assert*/

slist_node_t *SlistCreateNode(void *data, slist_node_t *next_node)
{
	slist_node_t *new_node = malloc(sizeof(slist_node_t));

	if(NULL != new_node)
	{
		new_node->data = data;
		new_node->next_node = next_node;
	}

	return new_node
}

static void SwapPointers(void **ptr1, void **ptr2)
{
	void *tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

static void SwapNodes(slist_node_t *node1, slist_node_t *node2)
{
	SwapPointers(&(node1->next_node), &(node2->next_node));
	SwapPointers(&(node1->data), &(node2->data));
}


slist_node_t *SlistInsertNode(slist_node_t *current_node,
														 slist_node_t *new_node)
{
	assert(NULL != current_node && NULL != new_node);

	new_node->next_node = current_node;
	SwapPointers(current_node->next_node, new_node->next_node);

	return new_node;
}

slist_node_t *SlistInsertAfterNode(slist_node_t *current_node,
														 slist_node_t *new_node)
{
	assert(NULL != current_node && NULL != new_node);

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
	assert(NULL != current_node);

}

slist_node_t *SlistRemoveAfter(slist_node_t *current_node)
{

}

size_t SlistCount(const slist_node_t *head)
{
	static size_t node_counter;

	if (NULL != head)
	{
		slist_node_t *tmp = head->next_node;
		++node_counter;
		SlistCount(tmp);
	}
}

slist_node_t *SlistFind(const slist_node_t *head, const void *data, 
												  match_func_t match_func)
{
	
}

int SlistForEach(slist_node_t *head, action_func_t action_func, 
									 void *param_for_action_func)
{

}

slist_node_t *SlistFlip(const slist_node_t *head)
{

}

int SlistHasLoops(const slist_node_t *head)
{

}

slist_node_t *SlistFindIntersection(const slist_node_t *head1,
									const slist_node_t *head2)
{

}
