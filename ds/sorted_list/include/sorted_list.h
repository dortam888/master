/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* This file contains functions for sorted list data structure operations.*/
/* Can be used for sorted list based algorithms */
/*############################################################################*/

#ifndef ILRD_SORTED_LIST_H
#define ILRD_SORTED_LIST_H
#include <stddef.h> /* size_t */

#include "doubly_linked_list.h" /* double linked list declarations*/

typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iter
{
	dlist_iter_t dlist_iter;
} sorted_list_iter_t;

typedef int (*is_before_t)(const void *element_data, const void *user_data, 
						   void *param);
/*############################################################################
  # Description:
	creates sorted linked list which will be sorted with a compare function
	received from the user
  # Parameters:
	is_before - compare function for sorting the data
  # Return Value:
	pointer to the created sorted list
############################################################################*/
sorted_list_t *SortedListCreate(is_before_t is_before, void *param);

/*############################################################################
  # Description:
	destroys the sorted linked list
  # Parameters:
	sorted_list - pointer to the list to be destroyed
  # Return Value:
	none
############################################################################*/
void SortedListDestroy(sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	returns an iterator to the first element in the list
  # Parameters:
	sorted_list - pointer to sorted linked list
  # Return Value:
	pointer to the beginning of the sorted linked list
############################################################################*/
sorted_list_iter_t SortedListBegin(sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	returns an iterator to the last element in the list
  # Parameters:
	sorted_list - pointer to sorted linked list
  # Return Value:
	pointer to the end of sorted linked list
############################################################################*/
sorted_list_iter_t SortedListEnd(sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	returns an iterator to the next element in the list
  # Parameters:
	current - iterator to the current element in the list
  # Return Value:
	iterator to the next element in the list
############################################################################*/
sorted_list_iter_t SortedListIterNext(sorted_list_iter_t current);

/*############################################################################
  # Description:
	returns an iterator to the previous element in the list
  # Parameters:
	current - iterator to the current element in the list
  # Return Value:
	iterator to the previous element in the list
############################################################################*/
sorted_list_iter_t SortedListIterPrev(sorted_list_iter_t current);


/*############################################################################
  # Description:
	returns address of data in current element
  # Parameters:
	current - pointer to element in sorted linked list
  # Return Value:
	address of data in the element
############################################################################*/
void *SortedListGetData(sorted_list_iter_t element);

/*############################################################################
  # Description:
	check if iterators are on the same element
  # Parameters:
	iter1 - iterator on the sorted linked list
	iter2 - iterator on the sorted linked list
  # Return Value:
	1 if equal, 0 otherwise
############################################################################*/
int SortedListIsSameIter(sorted_list_iter_t iter1, sorted_list_iter_t iter2);

/*############################################################################
  # Description:
	adds new element in sorted way. complexity O(n)
  # Parameters:
	sorted_list - pointer to the linked list
	data - data to be inserted to the list
  # Return Value:
	iterator to the added element
	returns END iterator if failure
############################################################################*/
sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data);

/*############################################################################
  # Description:
	removes element from linked list.
  # Parameters:
	current - iterator to element to be removed
  # Return Value:
	iterator to the element after the removed element
############################################################################*/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t current);

/*############################################################################
  # Description:
	Pop element from start of the list
  # Parameters:
	sorted_list - pointer to the sorted linked list
  # Return Value:
	the adress of data in the poped element
############################################################################*/
void* SortedListPopFront(sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	Pop element from end of the sorted linked list
  # Parameters:
	sorted_list - pointer to the sorted linked list
  # Return Value:
	the adress of data in the poped element
############################################################################*/
void* SortedListPopBack(sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	count number of elements in linked list
  # Parameters:
	sorted_list - pointer to linked list
  # Return Value:
	number of elements in linked list
############################################################################*/
size_t SortedListSize(const sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	check if list is empty
  # Parameters:
	SorteSortedList - pointer to linked list
  # Return Value:
	1 if list is empty 0 otherwise
############################################################################*/
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/*############################################################################
  # Description:
	find element in slice in the linked list using the sort compare function
  # Parameters:
	sorted_list - pointer to linked list
	data - data to compare
	(*cmp_func) - compare function
	to - iterator to end of slice
	from - iterator to beginning of slice
  # Return Value:
	if found returns iterator to found element, else End
############################################################################*/
sorted_list_iter_t SortedListFind(const sorted_list_t *list,
								  sorted_list_iter_t from,
								  sorted_list_iter_t to,
								  const void *data);

/*############################################################################
  # Description:
	find element in slice in the linked list using compare function from the user
  # Parameters:
	sorted_list - pointer to linked list
	data - data to compare
	(*cmp_func) - compare function
	to - iterator to end of slice
	from - iterator to beginning of slice
  # Return Value:
	if found returns iterator to found element, else End
############################################################################*/
sorted_list_iter_t SortedListFindIf(const sorted_list_t *list,
									sorted_list_iter_t from,
					   				sorted_list_iter_t to,
									match_func_t match_func,
									const void *data);

/*############################################################################
  # Description:
	preforms action function on each element in a slice inside a linked list
  # Parameters:
	sorted_list - pointer to linked list
	(*action_func) - action function to preform
	to - iterator to end of slice
	from - iterator to beginning of slice
  # Return Value:
	in case of success returns 0, otherwise passes the return value from the
	action function
############################################################################*/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
					  action_func_t action_func, void *param_for_action_func);

/*#############################################################################
# Description:
gets two pointers to two different sorted lists and merges them into one sorted
list in dest. src sorted list remains empty.
# Parameters:
dest_queue - pointer to the first queue (destination)
src_queue - pointer to the second queue (source)
# Return Value:
none
##############################################################################*/
void SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list);


#endif /* ILRD_SORTED_LIST_H */
