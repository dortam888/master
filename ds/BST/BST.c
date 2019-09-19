/*******************************************************************************
 **** Author: Dor Tambour                                                      *
 **** Last Update: Tuesday September 10 2019 02:04:39                          *
 **** Reviewer: dandan                                                         *
 **** Description: This file contains the implementations of functions         *
 *                                for the data structure BST.                  *
 *                                Look at BST.h for more information about the *
 *                                functions.                                   *
*******************************************************************************/
#include <stdlib.h> /*malloc free*/
#include <assert.h> /* assert */

#include "bst.h"

typedef int(*cmp_func_t)(void *iter_data, void *new_data, 
                         void *param);

enum child_direction {LEFT, RIGHT};
enum {BEGIN, ROOT};

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

/************************InitBSTIter()***************************************/
static bst_iter_t InitBSTIter()
{
    return NULL;
}

/************************BSTInvalidPointer************************************/
/*iterator of no node for example child of leaves*/
static bst_iter_t InvalidIter() 
{
    return (bst_iter_t)(0xDEADC0DE);
}

/************************GetNodeFromIter**************************************/
static struct bst_node *GetNodeFromIter(bst_iter_t iter)
{
    return iter; /*currently iter is just pointer to node*/
}

/************************GetNodeFromIter**************************************/
static bst_iter_t FreeIter(bst_iter_t iter)
{
    struct bst_node *node_to_free = GetNodeFromIter(iter);

    free(node_to_free); node_to_free = NULL;

    return InvalidIter();
}

/************************BSTBegin & BSTEnd Functions***************************/
bst_iter_t BSTBegin(const bst_t *bst)
{
    assert(NULL != bst);

    return bst->dummy_node->child[BEGIN];
}

bst_iter_t BSTEnd(const bst_t *bst)
{
    assert(NULL != bst);

    return bst->dummy_node;
}

/************************BSTMoveToChild***************************************/
static bst_iter_t BSTMoveToChild(bst_iter_t iter, 
                                 enum child_direction direction)
{
    return GetNodeFromIter(iter)->child[direction];
}

/************************BSTGetParent*****************************************/
static bst_iter_t BSTGetParent(bst_iter_t iter)
{
    return GetNodeFromIter(iter)->parent;
}

/*************************IsDummy*********************************************/
static int IsDummy(bst_iter_t iter)
{
    /*dummy is the only node that is the parent of itself*/
    return BSTIsSameIter(iter, BSTGetParent(iter));
}

/************************BranchIsEmpty**********for insert and find functions*/
static int BranchIsEmpty(bst_iter_t iter, enum child_direction direction)
{
    return (BSTMoveToChild(iter, direction) == InvalidIter());
}

/************************BSTGetRoot Function***********************************/
static bst_iter_t BSTGetRoot(const bst_t *bst) /*for insert and find functions*/
{
    assert(NULL != bst);
    assert(NULL != bst->dummy_node);

    return bst->dummy_node->child[ROOT];
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
    bst_iter_t next_iter = InitBSTIter();
    bst_iter_t prev_iter = iter;

    if (!BranchIsEmpty(iter, direction))
    {
        next_iter = BSTMoveToChild(iter, direction);
        direction = SwitchDirection(direction);

        while (!BranchIsEmpty(next_iter, direction))
        {
            next_iter = BSTMoveToChild(next_iter, direction);
        }
    }
    else
    {
        next_iter = BSTGetParent(prev_iter);
        
        while(BSTIsSameIter(BSTMoveToChild(next_iter, direction), prev_iter) && 
              !IsDummy(next_iter))
        {
            prev_iter = next_iter;
            next_iter = BSTGetParent(prev_iter);
        }
    }

    return next_iter;
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

/**************************BSTIsSameIter Function******************************/
int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
    return (iter1 == iter2);
}

/************************BSTCreate Function************************************/
static struct bst_node *CreateNode(void *data, 
                                   struct bst_node *parent,
                                   struct bst_node *child_right, 
                                   struct bst_node *child_left) 
{
    struct bst_node *new_node = 
                            (struct bst_node *)malloc(sizeof(struct bst_node));
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

    new_bst->dummy_node->child[BEGIN] = new_bst->dummy_node;
    new_bst->dummy_node->child[ROOT] = GetNodeFromIter(InvalidIter());
    new_bst->dummy_node->parent = new_bst->dummy_node;

    return new_bst;
}

/************************BSTInsert Function************************************/
bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    struct bst_node *new_node = NULL;
    bst_iter_t iter = InitBSTIter();
    bst_iter_t invalid_iter = InvalidIter();
    struct bst_node *invalid_node = GetNodeFromIter(invalid_iter);
    struct bst_node *parent = NULL;
    enum child_direction direction = RIGHT;

    assert(NULL != bst);

    parent = bst->dummy_node;
    new_node = CreateNode(data, invalid_node, invalid_node, invalid_node);
    if (NULL == new_node)
    {
        return NULL;
    }

    iter = BSTGetRoot(bst);

    /* update begin if minimum changed (or if empty)*/
    if (BSTIsEmpty(bst))
    {
        bst->dummy_node->child[BEGIN] = new_node;
    }
    else if (bst->cmp_func(BSTGetData(BSTBegin(bst)), data, bst->params) > 0) 
    {
        iter = BSTBegin(bst); /*move iter to minimum before update so insertion
                                will be O(1) instead of O(Log(n))*/
        bst->dummy_node->child[BEGIN] = new_node;
    }

    for (; !BSTIsSameIter(iter, invalid_iter); 
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
    bst_iter_t iter = InitBSTIter();
    bst_iter_t return_iter = InitBSTIter();
    bst_iter_t invalid_iter = InvalidIter();
    enum child_direction direction = RIGHT;

    assert(NULL != bst);

    for (iter = BSTGetRoot(bst); !BSTIsSameIter(iter, invalid_iter); 
         iter = BSTMoveToChild(iter, direction))
    {
        int cmp_func_result = bst->cmp_func(BSTGetData(iter), data_to_find,
                                            bst->params);

        if (cmp_func_result < 0)
        {
            direction = RIGHT;
        }
        else if (cmp_func_result > 0)
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

/**************************BSTIsEmpty Function*********************************/
int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);

    return BSTIsSameIter(BSTBegin(bst), BSTEnd(bst));
}

/**************************BSTSize Functions***********************************/
static void TraverseAndCount(bst_iter_t iter, size_t *counter)
{
    if (BSTIsSameIter(iter,InvalidIter()))
    {
        return;
    }

    TraverseAndCount(BSTMoveToChild(iter, LEFT), counter);
    ++(*counter);
    TraverseAndCount(BSTMoveToChild(iter, RIGHT), counter);
}

size_t BSTSize(const bst_t *bst)
{
    bst_iter_t iter = InitBSTIter();
    size_t counter = 0;

    assert(NULL != bst);

    iter = BSTGetRoot(bst);

    TraverseAndCount(iter, &counter);

    return counter;
}

/**************************BSTDestroy Functions********************************/
static void DestroyNodes(bst_iter_t iter)
{
    if (BSTIsSameIter(iter,InvalidIter()))
    {
        return;
    }

    DestroyNodes(BSTMoveToChild(iter, LEFT));
    DestroyNodes(BSTMoveToChild(iter, RIGHT));
    iter = FreeIter(iter);
}

void BSTDestroy(bst_t *bst)
{
    bst_iter_t iter = InitBSTIter();

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

/**************************BSTRemove Functions**********************************/
void BSTRemove(bst_iter_t iter_to_remove)
{
    enum child_direction direction = LEFT;
    struct bst_node *node_to_remove = GetNodeFromIter(iter_to_remove);
    struct bst_node *next_to_remove = GetNodeFromIter(BSTNext(iter_to_remove));
    struct bst_node *prev_to_remove = GetNodeFromIter(BSTPrev(iter_to_remove));

    /* checks parent of iter_to_remove direction */
    if (node_to_remove->parent->child[RIGHT] == node_to_remove)
    {
        direction = RIGHT;
    }
    else
    {
        direction = LEFT;
    }

    /*update begin if prev is dummy*/
    if (IsDummy(prev_to_remove))
    {
        prev_to_remove->child[BEGIN] = next_to_remove;
    }

    /* connects between the parent of node_to_remove and its right child*/
    node_to_remove->parent->child[direction] = node_to_remove->child[RIGHT];

    if (!BSTIsSameIter(BSTMoveToChild(iter_to_remove, RIGHT), InvalidIter()))
    {
        iter_to_remove->child[RIGHT]->parent = iter_to_remove->parent;
    }

    /* connects between the next node of node_to_remove and its left child*/
    if (!BSTIsSameIter(BSTMoveToChild(iter_to_remove, LEFT), InvalidIter()))
    {
        next_to_remove->child[LEFT] = node_to_remove->child[LEFT];
        node_to_remove->child[LEFT]->parent = next_to_remove;
    }

    iter_to_remove = FreeIter(iter_to_remove);
}

