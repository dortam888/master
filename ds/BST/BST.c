/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Tuesday September 10 2019 02:04:39 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure BST.
                                  Look at BST.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h> /*malloc free*/
#include <assert.h> /* assert */
#include <limits.h> /* LONG_MAX LONG_MIN */

#include "BST.h"

typedef int(*cmp_func_t)(const void *iter_data, const void *new_data, 
                         void *param);

enum child_direction {RIGHT, LEFT};

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
};

static struct bst_node *CreateNode(void *data, 
                                   struct bst_node *parent,
                                   struct bst_node *childright, 
                                   struct bst_node *childleft)
{
    struct bst_node *new_node = (struct bst_node *)malloc(sizeof(struct bst_node));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = data;
    new_node->parent = parent;
    new_node->child[RIGHT] = childright;
    new_node->child[LEFT] = childleft;

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

    return new_bst;
}

static int BranchIsEmpty(bst_iter_t iter, enum child_direction direction)
{
    return (iter->child[direction] == NULL);
}

static bst_iter_t BSTVisitNext(bst_iter_t iter, enum child_direction direction)
{
    return iter->child[direction];
}

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

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    struct bst_node *new_node = NULL;
    bst_iter_t iter = NULL;
    struct bst_node * parent = bst->dummy_node;
    enum child_direction direction = RIGHT;

    assert(NULL != bst);

    new_node = CreateNode(data, NULL, NULL, NULL);

    for (iter = BSTBegin(bst); !BSTIsSameIter(iter, NULL); 
         iter = BSTVisitNext(iter, direction))
    {
        parent = iter;

        if (bst->cmp_func(BSTGetData(iter), data, bst->params) > 0)
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

bst_iter_t MoveToNode(bst_iter_t iter, enum child_direction direction)
{
    bst_iter_t next_iter = NULL;

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
        bst_iter_t prev_iter = iter;
        next_iter = prev_iter->parent;
        
        while(!BSTIsSameIter(next_iter->child[direction], prev_iter))
        {
            prev_iter = next_iter;
            next_iter = prev_iter->parent;
        }
    }

    return next_iter;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    return MoveToNode(iter, RIGHT);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
    return MoveToNode(iter, LEFT);
}

void *BSTGetData(bst_iter_t iter)
{
    return iter->data;
}

bst_iter_t BSTBegin(const bst_t *bst)
{
    return bst->dummy_node->child[RIGHT];
}

bst_iter_t BSTEnd(const bst_t *bst)
{
    return bst->dummy_node;
}

int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
    return (iter1 == iter2);
}

int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);

    return BSTIsSameIter(BSTBegin(bst), NULL);
}

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

    iter = BSTBegin(bst);

    TraverseAndCount(iter, &counter);

    return counter;
}

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

    iter = BSTBegin(bst);

    DestroyNodes(iter);

    free(bst->dummy_node); bst->dummy_node = NULL;
    free(bst); bst = NULL;
}

typedef int(*action_func_t)(void *iter_data, void *params)

static int DoActionFunc(bst_iter_t iter, action_func_t action_func, 
                        void *params, int *function_status)
{
    if (iter == NULL)
    {
        return 0;
    }

    *function_status = action_func(BSTGetData(iter), params);

    if (0 != *function_status)
    {
        return *function_status;
    }

    DoActionFunc(iter->child[LEFT], action_func, params);
    DoActionFunc(iter->child[RIGHT], action_func, params);
}

int BSTForEach(void *param, bst_iter_t from, bst_iter_t to, 
               action_func_t action_func)


