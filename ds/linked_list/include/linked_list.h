#ifndef ILRD_LINKED_LIST_H
#define ILRD_LINKED_LIST_H

#include <stddef.h> /* size_t */


/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: שלישי אוגוסט 13 2019 01:38:08 pm */
/* This file contains functions for linked_list data structure operations.*/
/* Can be used for linked_list based algorithms */
/*############################################################################*/

typedef struct node
					{
						void* data;
						struct node *next_node;
					} slist_node_t;

/*############################################################################
  # Description:
	creates new node 
  # Parameters:
	data - pointer to data requested to be stored
  # Return Value:
	pointer to new node
############################################################################*/
slist_node_t *SlistCreateNode(void *data, slist_node_t *next_node);

/*############################################################################
  # Description:
	adds new node before current node. CAUSES POINTER INVALIDATION	
  # Parameters:
	current node - node to add before
	new node - node to add 
	head 0 
  # Return Value:
	pointer to address of inserted node
############################################################################*/
slist_node_t *SlistInsertNode(slist_node_t *current_node,
													slist_node_t *new_node);

/*############################################################################
  # Description:
	adds new node after current node 
  # Parameters:
	current node - node to add after
	new node - node to add
  # Return Value:
	pointer to address of inserted node
############################################################################*/
slist_node_t *SlistInsertAfterNode(slist_node_t *current_node,
												 slist_node_t *new_node);

/*############################################################################
  # Description:
	free all linked list
  # Parameters:
	head - pointer to first node of linked list
  # Return Value:
	none
############################################################################*/
void SlistFreeAll(slist_node_t *head);

/*############################################################################
  # Description:
	removes node from linked list. CAUSES POINTER INVALIDATION
  # Parameters:
	current_node - node to remove
  # Return Value:
	poitnter to address of new current node
############################################################################*/
slist_node_t *SlistRemove(slist_node_t *current_node);

/*############################################################################
  # Description:
	remove node after current node
  # Parameters:
	current node
  # Return Value:
	poitnter to address of new current node
############################################################################*/
slist_node_t *SlistRemoveAfter(slist_node_t *current_node);

/*############################################################################
  # Description:
	count number of nodes in linked list
  # Parameters:
	head - pointer to first node of linked list
  # Return Value:
	number of nodes in linked list
############################################################################*/
size_t SlistCount(const slist_node_t *head);

/*############################################################################
  # Description:
	find node that contains same data as input data
  # Parameters:
	head - pointer to first node of linked list
	data - data to compare
	(*cmp_func) - compare function 
  # Return Value:
	if found returns address of found node, else NULL
############################################################################*/
typedef int (*match_func_t)(const void *node_data, const void *user_data);

slist_node_t *SlistFind(const slist_node_t *head, const void *data, 
												match_func_t match_func);

/*############################################################################
  # Description:
	preforms action function on each node in linked list
  # Parameters:
	head - pointer to first node of linked list
	(*action_func) - action function to preform
  # Return Value:
	in case of success returns 0, otherwise passes the return value from the action function
############################################################################*/
typedef int (*action_func_t)(void *data, void *param);

int SlistForEach(slist_node_t *head, action_func_t action_func, 
											 void *param_for_action_func);

/*############################################################################
  # Description:
	flip order of linked list
  # Parameters:
	head - pointer to first node of linked list
  # Return Value:
	poitnter to address of new head
############################################################################*/
slist_node_t *SlistFlip(slist_node_t *head);

/*############################################################################
  # Description:
	check if linked list contains loop
  # Parameters:
	head - pointer to first node of linked list
  # Return Value:
	1 if loop found, 0 otherwise
############################################################################*/
int SlistHasLoops(const slist_node_t *head);

/*############################################################################
  # Description:
	check if Intersection is found
  # Parameters:
	head1 - pointer to first node of first linked list
	head2 - pointer to first node of second linked list
  # Return Value:
	address of found intersected node. otherwise NULL
############################################################################*/
slist_node_t *SlistFindIntersection(const slist_node_t *head1,
									const slist_node_t *head2);


#endif /* ILRD_LINKED_LIST_H */
