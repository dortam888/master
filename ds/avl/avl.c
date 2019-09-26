/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Thursday September 27 
**** Reviewer: 
**** Description: This file contains the implementations of functions
                                  for the data structure avl tree.
                                  Look at avl.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h> /*malloc free*/
#include <assert.h> /* assert */

#include "avl.h" /* avl_t */

/*************************STRUCT AND ENUMS DEFINITIONS*************************/
typedef struct avl_node
{
    struct avl_node *child[2];
    void *data;
    size_t height;
} avl_node_t;

typedef int(*cmp_func_t)(const void *tree_data, const void *new_data, 
                         void *param);

struct avl
{
    cmp_func_t cmp_func;
    void *param;
    avl_node_t root;
};

typedef enum child_direction
{
    RIGHT,
    LEFT
} child_direction_t;

enum {ROOT};

typedef struct node_child_height
{
    size_t right;
    size_t left;
} node_child_height_t;
/******************************************************************************/

/****************************FUNCTION DECLERATION******************************/
static avl_node_t *RemoveNode(avl_node_t *node, const void *data_to_remove, 
                              cmp_func_t cmp, void *param);
/******************************************************************************/

/******************************AVL UTILITIES***********************************/
static child_direction_t SwitchDirection(child_direction_t direction)
{
    switch (direction)
    {
        case (RIGHT):
                     direction = LEFT;
                     break;
        case (LEFT):
                     direction = RIGHT;
                     break;
    }

    return direction;
}

static child_direction_t GetAdvanceDirection(int cmp_result)
{
    return cmp_result < 0;
}

static avl_node_t *AVLGetRoot(const avl_t *avl)
{
    assert(NULL != avl);
    
    return avl->root.child[ROOT];
}

static avl_node_t *AVLMoveToChild(const avl_node_t *node, 
                                  child_direction_t direction)
{
    assert(NULL != node);

    return node->child[direction];
}

static size_t AVLGetHeight(const avl_node_t *node)
{
    assert(NULL != node);

    return node->height;
}

static void *AVLGetData(const avl_node_t *node)
{
    assert(NULL != node);
    
    return node->data;
}
/******************************AVL UTILITIES END*******************************/

/*****************************AVLCreate FUNCTION*******************************/
avl_t *AVLCreate(void *param, int (*cmp_func)(const void *tree_data,
								 			  const void *new_data,
								 			  void *param))
{
    avl_t *new_avl = (avl_t *)malloc(sizeof(avl_t));
    if (NULL == new_avl)
    {
        return NULL;
    }
    
    new_avl->root.child[ROOT] = NULL;
    new_avl->param = param;
    new_avl->cmp_func = cmp_func;
    
    return new_avl;
}
/*****************************AVLCreate FUNCTION END***************************/

/*****************************AVLDestroy FUNCTION******************************/
static void DestroyNodes(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }

    DestroyNodes(AVLMoveToChild(node, LEFT));
    DestroyNodes(AVLMoveToChild(node, RIGHT));
    free(node); node = NULL;
}

void AVLDestroy(avl_t *avl)
{
    assert(NULL != avl);

    DestroyNodes(AVLGetRoot(avl));

    free(avl); avl = NULL;
}
/*****************************AVLDestroy FUNCTION END**************************/

/*****************************AVLCOUNT FUNCTION********************************/
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
    size_t counter = 0;

    assert(NULL != avl);

    TraverseAndCount(AVLGetRoot(avl), &counter);

    return counter;
}
/*****************************AVLCOUNT FUNCTION END****************************/

/*****************************AVLIsEmpty FUNCTION******************************/
int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);
    
    return (AVLGetRoot(avl) == NULL);
}
/*****************************AVLIsEmpty FUNCTION END**************************/

/*****************************AVLHeight FUNCTION*******************************/
size_t AVLHeight(const avl_t *avl)
{
    assert(NULL != avl);
    
    return AVLGetHeight(AVLGetRoot(avl));
}
/*****************************AVLHeight FUNCTION END***************************/

/*****************************AVLFIND FUNCTION*********************************/
static avl_node_t *RecFind(avl_node_t *node, cmp_func_t cmp, void *param, 
                           const void *data_to_find)
{
    int cmp_result = 0;
    child_direction_t direction = RIGHT;

    if (NULL == node)
    {
        return NULL;
    }

    cmp_result = cmp(AVLGetData(node), data_to_find, param);
    direction = GetAdvanceDirection(cmp_result);
    
    if (0 == cmp_result)
    {
        return node;
    }

    return RecFind(AVLMoveToChild(node, direction), cmp, param, data_to_find);
}

void *AVLFind(const avl_t *avl, const void *data_to_find)
{
    avl_node_t *node_with_data_to_find = NULL;

    assert(NULL != avl);

    node_with_data_to_find = RecFind(AVLGetRoot(avl), 
                                     avl->cmp_func, avl->param, data_to_find);
    
    return (NULL == node_with_data_to_find)? NULL : 
                                             AVLGetData(node_with_data_to_find);
}
/*****************************AVLFIND FUNCTION END*****************************/

/*****************************AVLFOREACH FUNCTION******************************/
static void RecForEach(avl_node_t *node, int (*action_func)(void *tree_data,
					  void *param), void *param, int *action_func_status)
{
    if (NULL == node || 0 != *action_func_status)
    {
        return;
    }

    RecForEach(AVLMoveToChild(node, LEFT), action_func, param,
               action_func_status);
    *action_func_status += action_func(AVLGetData(node), param);
    RecForEach(AVLMoveToChild(node, RIGHT), action_func, param,
               action_func_status);
}

int AVLForEach(void *param, avl_t *avl, int (*action_func)(void *tree_data,
														   void *param))
{
    int action_func_status = 0;

    assert(NULL != avl);
    assert(NULL != action_func);

    RecForEach(AVLGetRoot(avl), action_func, param, &action_func_status);

    return action_func_status;
}
/*****************************AVLFOREACH FUNCTION******************************/

/************************TREE BALANCE UTILITIES********************************/
static size_t MaxHeight(size_t height_left, size_t height_right)
{
    return (height_left > height_right)? height_left : height_right;
}

static node_child_height_t GetChildHeight(const avl_node_t *node)
{
    node_child_height_t child_height = {0,0};
    avl_node_t *child_left = NULL;
    avl_node_t *child_right = NULL;

    assert(NULL != node);

    child_left = AVLMoveToChild(node, LEFT);
    child_right = AVLMoveToChild(node, RIGHT);

    child_height.left = (NULL == child_left)? 0 : AVLGetHeight(child_left);
    child_height.right = (NULL == child_right)? 0 : AVLGetHeight(child_right);

    return child_height;
}

static void UpdateHeight(avl_node_t *node)
{
    node_child_height_t child_height = {0,0};

    assert(NULL != node);

    child_height = GetChildHeight(node);

    node->height = (1 + MaxHeight(child_height.left, child_height.right));
}

static int GetBalance(const avl_node_t *node)
{
    node_child_height_t node_child_height = {0,0};

    assert(NULL != node);

    node_child_height = GetChildHeight(node);
    
    return node_child_height.left - node_child_height.right;
}
/************************TREE BALANCE UTILITIES END****************************/

/************************ROTATES FUNCTIONS*************************************/
typedef avl_node_t *(*rotate_t)(avl_node_t *node, child_direction_t direction);

static avl_node_t *Rotate(avl_node_t *node, child_direction_t direction)
{
    avl_node_t *pivot_node = NULL;
    avl_node_t *other_node = NULL;
    child_direction_t opposite_direction = SwitchDirection(direction);

    assert(NULL != node);

    pivot_node = AVLMoveToChild(node, opposite_direction);
    other_node = AVLMoveToChild(pivot_node, direction);

    pivot_node->child[direction] = node;
    node->child[opposite_direction] = other_node;

    UpdateHeight(node);
    UpdateHeight(pivot_node);

    return pivot_node;
}

static avl_node_t *DoubleRotate(avl_node_t *node, child_direction_t direction)
{
    child_direction_t opposite_direction = SwitchDirection(direction);

    node->child[opposite_direction] = Rotate(
                                      AVLMoveToChild(node,opposite_direction),
                                      opposite_direction);
    return Rotate(node, direction);
}
/************************ROTATES FUNCTIONS END*********************************/

/************************BALANCE FUNCTION**************************************/
static avl_node_t *Balance(avl_node_t *node)
{
    int balance_factor = 0;
    rotate_t imbalance_lut[2] = {Rotate, DoubleRotate};

    assert(NULL != node);

    balance_factor = GetBalance(node);

    if (balance_factor == 2) /*Left Imbalance*/
    {
        int balance_factor_child = GetBalance(AVLMoveToChild(node, LEFT));

        return imbalance_lut[balance_factor_child == -1](node, RIGHT);
    }
    else if (balance_factor == -2) /*Right Imbalance*/
    {
        int balance_factor_child = GetBalance(AVLMoveToChild(node, RIGHT));

        return imbalance_lut[balance_factor_child == 1](node, LEFT);
    }

    UpdateHeight(node);
    return node;
}
/************************BALANCE FUNCTION END**********************************/

/************************AVLInsert FUNCTION************************************/
static avl_node_t *AVLCreateNode(void *data)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = data;
    new_node->height = 1; /*node height in creation is implement defined as 1*/
    new_node->child[RIGHT] = NULL;
    new_node->child[LEFT] = NULL;
    
    return new_node;
}

static avl_node_t *RecInsert(cmp_func_t cmp, void *param, avl_node_t *node, 
                             avl_node_t *node_to_insert)
{
    child_direction_t direction = RIGHT;
    int cmp_result = 0;
    
    assert(NULL != node_to_insert);

    if (NULL == node)
    {
        return node_to_insert;
    }
    
    cmp_result = cmp(AVLGetData(node), AVLGetData(node_to_insert), param);
    direction =  GetAdvanceDirection(cmp_result);

    node->child[direction] = RecInsert(cmp, param, 
                             AVLMoveToChild(node, direction), node_to_insert);

    return Balance(node);
}

int AVLInsert(avl_t *avl, const void *data_to_insert)
{
    avl_node_t *new_node = NULL;

    assert(NULL != avl);
    
    new_node = AVLCreateNode((void *)data_to_insert);
    if (NULL == new_node)
    {
        return -1;
    }

    avl->root.child[ROOT] = RecInsert(avl->cmp_func, avl->param, 
                                      AVLGetRoot(avl), new_node);

    return 0;
}
/************************AVLInsert FUNCTION END********************************/

/************************AVLRemove FUNCTION************************************/
static avl_node_t *MostLeftNodeInBranch(avl_node_t *node)
{
	avl_node_t *most_left_node = node;
	
	assert(NULL != node);
	
	if (NULL == AVLMoveToChild(most_left_node, LEFT))
	{
		return most_left_node;
	}
	
	most_left_node = MostLeftNodeInBranch(AVLMoveToChild(most_left_node, LEFT));
	
	return most_left_node;
}

static int IsLeaf(const avl_node_t *node)
{
	assert(NULL != node);

	return (NULL == AVLMoveToChild(node, RIGHT) && 
	        NULL == AVLMoveToChild(node, LEFT));
}

static int IsNodeHasOneChild(const avl_node_t *node)
{
	assert(NULL != node);

	return (NULL == AVLMoveToChild(node, LEFT) || 
	        NULL == AVLMoveToChild(node, RIGHT));
}

static avl_node_t *DestroyNode(avl_node_t *node, cmp_func_t cmp, void *param)
{
	assert(NULL != node);

	if (IsLeaf(node))
	{
		free(node); node = NULL;
	}
	else if (IsNodeHasOneChild(node))
	{
		avl_node_t *child = (AVLMoveToChild(node, RIGHT) == NULL)? 
							 AVLMoveToChild(node, LEFT) : 
							 AVLMoveToChild(node, RIGHT);
		
		*node = *child;
		free(child); child = NULL;
	}
	else
	{
		avl_node_t *invalidate_node = MostLeftNodeInBranch(AVLMoveToChild(node,
		                                                                RIGHT));
		node->data = AVLGetData(invalidate_node);
		node->child[RIGHT] = RemoveNode(AVLMoveToChild(node, RIGHT), 
		                                AVLGetData(node), cmp, param);
	}
	
	return node;
}

static avl_node_t *RemoveNode(avl_node_t *node, const void *data_to_remove, 
                              cmp_func_t cmp, void *param)
{
    int cmp_result = 0;
    child_direction_t direction = RIGHT;
    
    assert(NULL != cmp);
    assert(NULL != node);
    
    cmp_result = cmp(AVLGetData(node), data_to_remove, param);
    
    if (0 == cmp_result)
    {
        return DestroyNode(node, cmp, param);
    }
    
    direction = GetAdvanceDirection(cmp_result);
    
    if (NULL == AVLMoveToChild(node, direction))
    {
        return node;
    }
    
    node->child[direction] = RemoveNode(AVLMoveToChild(node, direction),
                                        data_to_remove, cmp, param);

    return Balance(node);
}

void AVLRemove(avl_t *avl, const void *data_to_remove)
{
    assert(NULL != avl);
    
    RemoveNode(AVLGetRoot(avl), data_to_remove, avl->cmp_func, avl->param);
}
/************************AVLRemove FUNCTION END********************************/

