#ifndef ILRD_DLIST_H
#define ILRD_DLIST_H
#include <stddef.h> /* size_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* This file contains functions for linked_list data structure operations.*/
/* Can be used for linked_list based algorithms */
/*############################################################################*/

typedef struct dlist dlist_t;
typedef struct dlist_node dlist_node_t;
typedef dlist_node_t * dlist_iter_t;

/*############################################################################
  # Description:
	creates double linked list 
  # Parameters:
	None
  # Return Value:
	pointer to the created list
############################################################################*/
dlist_t *DlistCreate(void);

/*############################################################################
  # Description:
	destroys the double linked list
  # Parameters:
	dlist - pointer to the list to be destroyed
  # Return Value:
	none
############################################################################*/
void DlistDestroy(dlist_t *dlist);

/*############################################################################
  # Description:
	returns an iterator to the next element in the list
  # Parameters:
	current - iterator to the current element in the list
  # Return Value:
	iterator to the next element in the list
############################################################################*/
dlist_iter_t DlistIterNext(dlist_iter_t current);

/*############################################################################
  # Description:
	returns an iterator to the previous element in the list
  # Parameters:
	current - iterator to the current element in the list
  # Return Value:
	iterator to the previous element in the list
############################################################################*/
dlist_iter_t DlistIterPrev(dlist_iter_t current);

/*############################################################################
  # Description:
	returns an iterator to the first element in the list
  # Parameters:
	dlist - pointer to linked list
  # Return Value:
	pointer to the beginning linked list
############################################################################*/
dlist_iter_t DlistBegin(dlist_t *dlist);

/*############################################################################
  # Description:
	returns an iterator to the last element in the list
  # Parameters:
	dlist - pointer to linked list
  # Return Value:
	pointer to the end of linked list
############################################################################*/
dlist_iter_t DlistEnd(dlist_t *dlist);

/*############################################################################
  # Description:
	returns address of data in current element
  # Parameters:
	current - pointer to element in linked list
  # Return Value:
	address of data in the element
############################################################################*/
void *DlistGetData(dlist_iter_t element);

/*############################################################################
  # Description:
	check if iterators are on the same element
  # Parameters:
	iter1 - iterator on the linked list
	iter2 - iterator on the linked list
  # Return Value:
	1 if equal, 0 otherwise
############################################################################*/
int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2);

/*############################################################################
  # Description:
	adds new element before current element.
  # Parameters:
	dlist - pointer to the linked list
	current - iterator on the linked list
	data - data to be inserted to the list
  # Return Value:
	iterator to the added element
	returns END iterator if failure
############################################################################*/
dlist_iter_t DlistInsert(dlist_t *dlist, dlist_iter_t current, void *data);

/*############################################################################
  # Description:
	removes element from linked list.
  # Parameters:
	current - iterator to element to be removed
  # Return Value:
	iterator to the next element
############################################################################*/
dlist_iter_t DlistRemove(dlist_iter_t current);

/*############################################################################
  # Description:
	Push element to the start of the list 
  # Parameters:
	dlist - pointer to the linked list
	data - data to be added
  # Return Value:
	iterator to added element.
	returns END iterator if failure
############################################################################*/
dlist_iter_t DlistPushFront(dlist_t *dlist, void *data);

/*############################################################################
  # Description:
	Push element to the end of the list 
  # Parameters:
	dlist - pointer to the linked list
	data - data to be added
  # Return Value:
	iterator to added element.
	returns END iterator if failure
############################################################################*/
dlist_iter_t DlistPushBack(dlist_t *dlist, void *data);

/*############################################################################
  # Description:
	Pop element from start of the list 
  # Parameters:
	dlist - pointer to the linked list
  # Return Value:
	the adress of data in the poped element
############################################################################*/
void* DlistPopFront(dlist_t *dlist);

/*############################################################################
  # Description:
	Pop element from end of the list 
  # Parameters:
	dlist - pointer to the linked list
  # Return Value:
	the adress of data in the poped element
############################################################################*/
void* DlistPopBack(dlist_t *dlist);

/*############################################################################
  # Description:
	count number of elements in linked list
  # Parameters:
	dlist - pointer to linked list
  # Return Value:
	number of elements in linked list
############################################################################*/
size_t DlistSize(const dlist_t *dlist);

/*############################################################################
  # Description:
	check if list is empty
  # Parameters:
	dlist - pointer to linked list
  # Return Value:
	1 if list is empty 0 otherwise
############################################################################*/
int DlistIsEmpty(const dlist_t *dlist);

/*############################################################################
  # Description:
	find element in slice in the linked list that contains same data as input data
  # Parameters:
	dlist - pointer to linked list
	data - data to compare
	(*cmp_func) - compare function 
	to - iterator to end of slice
	from - iterator to beginning of slice
  # Return Value:
	if found returns iterator to found element, else End
############################################################################*/
typedef int (*match_func_t)(const void *element_data, const void *user_data);

dlist_iter_t DlistFind(const dlist_t *list,
					   dlist_iter_t from, dlist_iter_t to, 
					   match_func_t match_func, const void *data);

/*############################################################################
  # Description:
	preforms action function on each element in a slice inside a linked list
  # Parameters:
	dlist - pointer to linked list
	(*action_func) - action function to preform
	to - iterator to end of slice
	from - iterator to beginning of slice
  # Return Value:
	in case of success returns 0, otherwise passes the return value from the action function
############################################################################*/
typedef int (*action_func_t)(void *element_data, void *param);

int DlistForEach(dlist_iter_t from, dlist_iter_t to,
				 action_func_t action_func, void *param_for_action_func);
/*############################################################################
  # Description:
	takes one slice of one list and move it to specified iterator on other list
  # Parameters:
	dest - iterator to where to put the slice
	src_to - iterator to end of slice
	src_from - iterator to beginning of slice
  # Return Value:
	iterator to start of slice that moved
############################################################################*/
dlist_iter_t DlistSplice(dlist_iter_t dest, dlist_iter_t src_from, 
						 dlist_iter_t src_to);


#endif /* ILRD_DLIST_H */
