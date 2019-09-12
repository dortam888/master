/*******************************************************************************
 **** Author: Dor Tambour                                                      *
 **** Last Update: Tuesday September 10 2019 02:04:39                          *
 **** Reviewer:                                                                *
 **** Description: This file contains the implementations of functions         *
 *                                for the data structure BST.                  *
 *                                Look at BST.h for more information about the *
 *                                functions.                                   *
*******************************************************************************/
#include <stdlib.h> /*malloc free*/
#include <assert.h> /* assert */
#include <limits.h> /* LONG_MAX LONG_MIN */

#include "bst.h"

typedef int(*cmp_func_t)(void *iter_data, void *new_data, 
                         void *param);

enum child_direction {LEFT, RIGHT};

struct bst_node
{
    void *data;
    struct bst_node *parent;
    struct bst_node *child[2];
};

struct bst
{
    cmp_func_t cmp_func;
    void *params;
    struct bst_node *dummy_node; 
    /*dummy node has the minimum on the left child, the 
    root at the right child, and himself as parent*/
};

/************************BSTCreate Function************************************/
static struct bst_node *CreateNode(void *data, 
                                   struct bst_node *parent,
                                   struct bst_node *child_right, 
                                   struct bst_node *child_left) 
{
    struct bst_node *new_node = (struct bst_node *)
                                                malloc(sizeof(struct bst_node));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = data;
    new_node->parent = parent;
    new_node->child[RIGHT] = child_right;
    new_node->child[LEFT] = child_left;

    return new_node;
}

bst_t *BSTCreate(cmp_func_t cmp_func, void *params)
{
    bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));
    if (NULL == new_bst)
    {
        return NULL;
    }

    new_bst->cmp_func = cmp_func;
    new_bst->params = params;

    new_bst->dummy_node = CreateNode(NULL, NULL, NULL, NULL);
    if (NULL == new_bst->dummy_node)
    {
        free(new_bst); new_bst = NULL;
        return NULL;
    }

    new_bst->dummy_node->child[LEFT] = new_bst->dummy_node;
    new_bst->dummy_node->parent = new_bst->dummy_node;

    return new_bst;
}

/************************BranchIsEmpty**********for insert and find functions*/
static int BranchIsEmpty(bst_iter_t iter, enum child_direction direction)
{
    return (iter->child[direction] == NULL);
}

/************************BSTMoveToChild**********for insert and find functions*/
static bst_iter_t BSTMoveToChild(bst_iter_t iter, 
                                 enum child_direction direction)
{
    return iter->child[direction];
}


/************************BSTGetRoot Function***********************************/
static bst_iter_t BSTGetRoot(const bst_t *bst) /*for insert and find functions*/
{
    assert(NULL != bst);

    return bst->dummy_node->child[RIGHT];
}

/************************BSTInsert Function************************************/
bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    struct bst_node *new_node = NULL;
    bst_iter_t iter = NULL;
    struct bst_node *parent = NULL;
    enum child_direction direction = RIGHT;

    assert(NULL != bst);

    parent = bst->dummy_node;
    new_node = CreateNode(data, NULL, NULL, NULL);

    iter = BSTGetRoot(bst);

    if (BSTIsEmpty(bst))
    {
        bst->dummy_node->child[LEFT] = new_node;
    }
    else if (bst->cmp_func(BSTGetData(bst->dummy_node->child[LEFT]), 
             data, bst->params) > 0) /* update begin */
    {
        iter = bst->dummy_node->child[LEFT];
        bst->dummy_node->child[LEFT] = new_node;
    }

    for (; !BSTIsSameIter(iter, NULL); 
         iter = BSTMoveToChild(iter, direction))
    {
        parent = iter;

        if (bst->cmp_func(BSTGetData(iter), data, bst->params) < 0)
        {
            direction = RIGHT;
        }
        else
        {
            direction = LEFT;
        }
    }

    new_node->parent = parent;
    parent->child[direction] = new_node;

    return new_node;
}

/************************BSTFind Function**************************************/
bst_iter_t BSTFind(bst_t *bst, void *data_to_find)
{
    bst_iter_t iter = NULL;
    bst_iter_t return_iter = NULL;
    enum child_direction direction = RIGHT;

    assert(NULL != bst);
    
    return_iter = NULL;

    for (iter = BSTGetRoot(bst); !BSTIsSameIter(iter, NULL); 
         iter = BSTMoveToChild(iter, direction))
    {
        if (bst->cmp_func(BSTGetData(iter), data_to_find, bst->params) < 0)
        {
            direction = RIGHT;
        }
        else if (bst->cmp_func(BSTGetData(iter), data_to_find, bst->params) > 0)
        {
            direction = LEFT;
        }
        else
        {
            return_iter = iter;
            break;
        }
    }

    return return_iter;
}

/************************BSTNext & BSTPrev Functions***************************/
static enum child_direction SwitchDirection(enum child_direction direction)
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

static bst_iter_t NextNodeIfRightPrevNodeIfLeft(bst_iter_t iter, 
                                                enum child_direction direction)
{
    bst_iter_t next_iter = NULL;
    bst_iter_t prev_iter = iter;

    if (!BranchIsEmpty(iter, direction))
    {
        next_iter = iter->child[direction];
        direction = SwitchDirection(direction);

        while (!BranchIsEmpty(next_iter, direction))
        {
            next_iter = next_iter->child[direction];
        }
    }
    else
    {
        next_iter = prev_iter->parent;
        
        while(BSTIsSameIter(next_iter->child[direction], prev_iter) && 
              !BSTIsSameIter(next_iter, next_iter->parent))
        {
            prev_iter = next_iter;
            next_iter = prev_iter->parent;
        }
    }

    return next_iter? next_iter : prev_iter;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    return NextNodeIfRightPrevNodeIfLeft(iter, RIGHT);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    return NextNodeIfRightPrevNodeIfLeft(iter, LEFT);
}

/************************BSTGetData Function***********************************/
void *BSTGetData(bst_iter_t iter)
{
    return iter->data;
}

/************************BSTBegin & BSTEnd Functions***************************/
bst_iter_t BSTBegin(const bst_t *bst)
{
    assert(NULL != bst);

    return bst->dummy_node->child[LEFT];
}

bst_iter_t BSTEnd(const bst_t *bst)
{
    assert(NULL != bst);

    return bst->dummy_node;
}

/**************************BSTIsSameIter Function******************************/
int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
    return (iter1 == iter2);
}

/**************************BSTIsEmpty Function*********************************/
int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);

    return BSTIsSameIter(BSTBegin(bst), BSTEnd(bst));
}

/**************************BSTSize Functions***********************************/
static void TraverseAndCount(bst_iter_t iter, size_t *param)
{
    if (iter == NULL)
    {
        return;
    }

    TraverseAndCount(iter->child[LEFT], param);
    ++(*param);
    TraverseAndCount(iter->child[RIGHT], param);
}

size_t BSTSize(const bst_t *bst)
{
    bst_iter_t iter = NULL;
    size_t counter = 0;

    assert(NULL != bst);

    iter = BSTGetRoot(bst);

    TraverseAndCount(iter, &counter);

    return counter;
}

/**************************BSTDestroy Functions********************************/
static void DestroyNodes(bst_iter_t iter)
{
    if (iter == NULL)
    {
        return;
    }

    DestroyNodes(iter->child[LEFT]);
    DestroyNodes(iter->child[RIGHT]);
    free(iter);
}

void BSTDestroy(bst_t *bst)
{
    bst_iter_t iter = NULL;

    assert(NULL != bst);

    iter = BSTGetRoot(bst);

    DestroyNodes(iter);

    free(bst->dummy_node); bst->dummy_node = NULL;
    free(bst); bst = NULL;
}

/**************************BSTForEach Function*********************************/
typedef int(*action_func_t)(void *iter_data, void *params);

int BSTForEach(void *params, bst_iter_t from, bst_iter_t to, 
               action_func_t action_func)
{
    bst_iter_t iter = from;
    int action_func_status = 0;

    assert(NULL != action_func);

    for (iter = from; 
         !BSTIsSameIter(iter, to) && (0 == action_func_status); 
         iter = BSTNext(iter))
    {
        action_func_status = action_func(BSTGetData(iter), params);
    }
    
    return action_func_status;
}

/**************************BSTRemove Function**********************************/
void BSTRemove(bst_iter_t iter_to_remove)
{
    enum child_direction direction = LEFT;
    bst_iter_t next_to_remove = BSTNext(iter_to_remove);
    bst_iter_t prev_to_remove = BSTPrev(iter_to_remove);

    /* checks parent of iter_to_remove direction */
    if (BSTIsSameIter(iter_to_remove->parent->child[RIGHT], iter_to_remove))
    {
        direction = RIGHT;
    }
    else
    {
        direction = LEFT;
    }

    /*update begin if your prev is dummy*/
    if (BSTIsSameIter(prev_to_remove->parent, prev_to_remove))
    {
        prev_to_remove->child[LEFT] = next_to_remove;
    }

    /* connects between the parent of iter_to_remove and its right child*/
    iter_to_remove->parent->child[direction] = iter_to_remove->child[RIGHT];

    if (NULL != iter_to_remove->child[RIGHT])
    {
        iter_to_remove->child[RIGHT]->parent = iter_to_remove->parent;
    }

    /* connects between the nextnode of iter_to_remove and its left child*/
    if (NULL != iter_to_remove->child[LEFT])
    {
        next_to_remove->child[LEFT] = iter_to_remove->child[LEFT];
        iter_to_remove->child[LEFT]->parent = next_to_remove;
    }

    free(iter_to_remove); iter_to_remove = NULL;
}

