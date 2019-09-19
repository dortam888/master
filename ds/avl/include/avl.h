/*############################################################################*/
/* Owner: OL712                                                               */
/* Reviewer: OL712                                                            */
/* Create: Tuesday September 10 2019 02:04:39                                 */
/* This file contains functions for AVL tree (binary search tree) data        */
/* structure                                                                  */
/* operations.                                                                */
/* Can be used for AVL based algorithms                                       */
/*############################################################################*/

#ifndef ILRD_AVL_H
#define ILRD_AVL_H

#include <stddef.h> /* size_t */

typedef struct avl avl_t; 

/*
 * Create new avl tree
 * Param @cmp_func - user comparison function. returns 0 if <iter_data> and 
 *					 <new_data> are equal, positive if <iter_data> is larger 
 *					 or negative if its smaller.
 * Param @param - user param for <cmp_func> function
 * Return: pointer to new binary search tree
 * Errors: if memory allocation failed, return NULL
 */
avl_t *AVLCreate(int (*cmp_func)(void *iter_data, void *new_data, void *param), 
				 void *param);

/*
 * Destroy binary search tree 
 * Param @avl - pointer to binary search tree  
 * Return : --
 * Errors : --
 */
void AVLDestroy(avl_t *avl);

/*
 * Insert new element to tree
 * Param @AVL - pointer to binary search tree  
 * Param @data - pointer to data to insert
 * Return : pointer to new leaf
 * Errors : if memory allocation failed, return NULL
 */
int AVLInsert(avl_t *avl, void *data); 

/*
 * Removes iter from tree
 * Param @iter_to_remove - iter to remove
 * Return : --
 */
int AVLRemove(void *data);

/*
 * Count how many nodes in given binary search tree
 * Param  @AVL - binary search tree  
 * Return : size of AVL
 * Errors : --
 */
size_t AVLSize(const avl_t *avl); 

/*
 * Check if tree is empty
 * Param  @AVL - binary search tree
 * Return : 1 if AVL is empty, 0 otherwise
 * Errors : --
 */
int AVLIsEmpty(const avl_t *avl); 

/*
 * Perform <action_func> for each element in <AVL>, stops if action returns
 *        non-zero.
 * Param @param - user param for <action_func>
 * Param @action_func - function to perform.
 * Note : <action_func> should not alter <iter_data>, doing so might alter
 *		  the structural logic of the AVL.
		  <action_func> return 0 on success, non-zero otherwise
 * Return : 0 for success, action's last return value otherwise
 * Errors : --
 */
int AVLForEach(void *param, int (*action_func)(void *iter_data, void *param));

/*
 * Find <data> in binary tree <AVL> 
 * Param  @AVL - binary search tree to search
 * Param  @data - data to find
 * Return : iter to found iter
 * Errors : if <data> was not found, return END
 */
int AVLFind(avl_t *avl, void *data_to_find);

/*
 * count deepest branch in avl
 * Param  @AVL - avl tree  
 * Return : avl height
 * Errors : --
 */
size_t AVLHeight(const avl_t *avl);

#endif /*ILRD_AVL_H*/

