/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Thursday September 19 2019 03:16:21 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure avl.
                                  Look at avl.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h>
#include <assert.h>

#include "avl.h"

typedef struct avl_node
{
    void *data;
    size_t height;
    struct avl_node *child[2];
} avl_node_t;

struct avl
{
    void *param;
    int(*cmp_func)(const void *tree_data, const void *new_data, void *param);
    avl_node_t *root;
};

typedef enum child_direction
{
    RIGHT,
    LEFT
} child_direction_t;

static avl_node_t *RemoveNode(avl_node_t *node, const void *data_to_remove, 
                              avl_t *avl);

static avl_node_t *AVLCreateNode(void *data, avl_node_t *child_right,
                                 avl_node_t *child_left)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = data;
    new_node->height = 1;
    new_node->child[RIGHT] = child_right;
    new_node->child[LEFT] = child_left;
    
    return new_node;
}

avl_t *AVLCreate(void *param, int (*cmp_func)(const void *tree_data,
								 			  const void *new_data,
								 			  void *param))
{
    avl_t *new_avl = (avl_t *)malloc(sizeof(avl_t));
    if (NULL == new_avl)
    {
        return NULL;
    }
    
    new_avl->root = AVLCreateNode((void *)0xDEADBEAF, NULL, NULL);
    if (NULL == new_avl->root)
    {
        free(new_avl); new_avl = NULL;
        return NULL;
    }
    
    new_avl->param = param;
    new_avl->cmp_func = cmp_func;
    
    return new_avl;
}

static avl_node_t *AVLGetRoot(const avl_t *avl)
{
    assert(NULL != avl);
    assert(NULL != avl->root);
    
    return avl->root->child[RIGHT];
}

static avl_node_t *AVLMoveToChild(avl_node_t *node, 
                                  enum child_direction direction)
{
    assert(NULL != node);

    return node->child[direction];
}

static void DestroyNodes(avl_node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    DestroyNodes(AVLMoveToChild(node, LEFT));
    DestroyNodes(AVLMoveToChild(node, RIGHT));
    free(node); node = NULL;
}

void AVLDestroy(avl_t *avl)
{
    avl_node_t *root = NULL;

    assert(NULL != avl);

    root = AVLGetRoot(avl);

    DestroyNodes(root);

    free(avl->root); avl->root = NULL;
    free(avl); avl = NULL;
}

static void TraverseAndCount(avl_node_t *node, size_t *counter)
{
    if (NULL == node)
    {
        return;
    }

    ++(*counter);
    TraverseAndCount(AVLMoveToChild(node, LEFT), counter);
    TraverseAndCount(AVLMoveToChild(node, RIGHT), counter);
}

size_t AVLCount(const avl_t *avl)
{
    avl_node_t *root = NULL;
    size_t counter = 0;

    assert(NULL != avl);

    root = AVLGetRoot(avl);

    TraverseAndCount(root, &counter);

    return counter;
}

int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);
    
    return (AVLGetRoot(avl) == NULL);
}

size_t AVLHeight(const avl_t *avl)
{
    assert(NULL != avl);
    
    return AVLGetRoot(avl)->height;
}

/*
static child_direction_t GetProgressDirection(avl_t *avl, const void *tree_data,
                                                          const void *new_data)
{
    assert(NULL != avl);

    return (avl->cmp_func(tree_data, new_data, avl->param) < 0);
}
*/

static avl_node_t *RecFind(avl_node_t *node, const avl_t *avl, 
                    const void *data_to_find)
{
    int cmp_result = 0;
    enum child_direction direction = RIGHT;

    assert(NULL != avl);
    
    if (NULL == node)
    {
        return NULL;
    }

    cmp_result = avl->cmp_func(node->data, data_to_find, avl->param);
    direction = (cmp_result < 0);
    
    if (cmp_result == 0)
    {
        return node;
    }

    return RecFind(node->child[direction], avl, data_to_find);
}

void *AVLFind(const avl_t *avl, const void *data_to_find)
{
    avl_node_t *node = NULL;

    assert(NULL != avl);
    
    node = AVLGetRoot(avl);

    node = RecFind(node, avl, data_to_find);
    
    return (NULL == node)? NULL : node->data;
}

static void RecForEach(avl_node_t *node, int (*action_func)(void *tree_data,
					  void *param), void *param, int *action_func_status)
{
    if (NULL == node || *action_func_status != 0)
    {
        return;
    }
    
    RecForEach(node->child[LEFT], action_func, param, action_func_status);
    *action_func_status += action_func(node->data, param);
    RecForEach(node->child[RIGHT], action_func, param, action_func_status);
}

int AVLForEach(void *param, avl_t *avl, int (*action_func)(void *tree_data,
														   void *param))
{
    avl_node_t *node = NULL;
    int action_func_status = 0;
    
    assert(NULL != avl);
    assert(NULL != action_func);
    
    node = AVLGetRoot(avl);

    RecForEach(node, action_func, param, &action_func_status);
    
    return action_func_status;
}

static size_t MaxHeight(size_t height_left, size_t height_right)
{
    return (height_left > height_right)? height_left : height_right;
}

static size_t UpdateHeight(avl_node_t *node)
{
    size_t height_right = 0;
    size_t height_left = 0;

    assert(NULL != node);

    height_left = (NULL == node->child[LEFT])? 0 : node->child[LEFT]->height;
    height_right = (NULL == node->child[RIGHT])? 0 : node->child[RIGHT]->height;

    return (1 + MaxHeight(height_left, height_right));
}

static int RecInsert(avl_t *avl, avl_node_t *node, avl_node_t *node_to_insert)
{
    enum child_direction direction = RIGHT;
    
    assert(NULL != avl);
    assert(NULL != node);
    assert(NULL != node_to_insert);

    direction = avl->cmp_func(node->data, node_to_insert->data, avl->param) < 0;

    if (NULL == node->child[direction])
    {
        node->child[direction] = node_to_insert;
        return 0;
    }

    RecInsert(avl, node->child[direction], node_to_insert);

    node->height = UpdateHeight(node);
    /*Balance(node);*/

    return 0;
}

int AVLInsert(avl_t *avl, const void *data_to_insert)
{
    avl_node_t *new_node = NULL;
    avl_node_t *root = NULL;

    assert(NULL != avl);
    
    new_node = AVLCreateNode((void *)data_to_insert, NULL, NULL);
    if (NULL == new_node)
    {
        return -1;
    }
    
    if (AVLIsEmpty(avl))
    {
        avl->root->child[RIGHT] = new_node;
        return 0;
    }

    root = AVLGetRoot(avl);

    return RecInsert(avl, root, new_node);
}

static avl_node_t *MostLeft(avl_node_t *node)
{
	avl_node_t *most_left = NULL;
	
	assert(NULL != node);
	
	most_left = node;
	
	if (NULL == most_left->child[LEFT])
	{
		return most_left;
	}
	
	most_left = MostLeft(most_left->child[LEFT]);
	
	return most_left;
}

static int ZeroChild(avl_node_t *node)
{
	assert(NULL != node);

	return (NULL == node->child[RIGHT] && NULL == node->child[LEFT]);
}

static int OneChild(avl_node_t *node)
{
	assert(NULL != node);

	return (NULL == node->child[RIGHT] || NULL == node->child[LEFT]);
}

static avl_node_t *DestroyNode(avl_node_t *node, avl_t *avl)
{
	assert(NULL != node);

	if (ZeroChild(node))
	{
		free(node); node = NULL;
	}
	else if (OneChild(node))
	{
		avl_node_t *child = (node->child[RIGHT] == NULL)? 
							 node->child[LEFT] : node->child[RIGHT];
		
		*node = *child;
		free(child); child = NULL;
	}
	else
	{
		avl_node_t *invalidate_node = MostLeft(node->child[RIGHT]);
		node->data = invalidate_node->data;
		node->child[RIGHT] = RemoveNode(node->child[RIGHT], node->data, avl);
	}
	
	return node;
}

static avl_node_t *RemoveNode(avl_node_t *node, const void *data_to_remove, 
                              avl_t *avl)
{
    int cmp_func_result = 0;
    enum child_direction direction = RIGHT;
    
    assert(NULL != avl);
    assert(NULL != node);
    
    cmp_func_result = avl->cmp_func(node->data, data_to_remove, avl->param);
    
    if (0 == cmp_func_result)
    {
        return DestroyNode(node, avl);
    }
    
    direction = (cmp_func_result < 0);
    
    if (NULL == node->child[direction])
    {
        return node;
    }
    
    node->child[direction] = RemoveNode(node->child[direction], data_to_remove,
                                        avl);
                                        
    node->height = UpdateHeight(node);
    return node;
    /*Balance(node);*/
}

void AVLRemove(avl_t *avl, const void *data_to_remove)
{
    avl_node_t *root = NULL;

    assert(NULL != avl);
    
    root = AVLGetRoot(avl);
    
    RemoveNode(root, data_to_remove, avl);
}

