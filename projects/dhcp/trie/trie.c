/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure trie.
                                  Look at trie.h for more information about the 
                                  functions.
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "trie.h" /* single_bit_trie API */

typedef struct trie_node
{
    struct trie_node *parent;
    struct trie_node *child[2];
} trie_node_t;

struct trie
{
    trie_node_t dummy;
    size_t bits_in_key;
};

typedef enum child_direction
{
    LEFT, 
    RIGHT
} child_direction_t;

/***************************Util Functions****************************/
static int HasRightChild(const trie_node_t *node)
{
    assert(NULL != node);

    return (NULL != node->child[RIGHT]);
}

static int HasLeftChild(const trie_node_t *node)
{
    assert(NULL != node);

    return (NULL != node->child[LEFT]);
}

static int IsLeaf(const trie_node_t *node)
{
    assert(NULL != node);

    return !HasLeftChild(node) && !HasRightChild(node);
}

static child_direction_t FindChildDirection(const trie_node_t *node)
{
    assert(NULL != node);
    assert(NULL != node->parent); /*not dummy*/

    return (node->parent->child[RIGHT] == node);
}

/*****************************TrieCreate**********************************/
static void InitNode(trie_node_t *node, trie_node_t *parent)
{
    assert(NULL != node);

    node->parent = parent;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;
}

trie_t *TrieCreate(size_t bits_in_key)
{
    trie_t *new_trie = (trie_t *)malloc(sizeof(*new_trie));
    if (NULL == new_trie)
    {
        return NULL;
    }

    new_trie->bits_in_key = bits_in_key;
    InitNode(&new_trie->dummy, NULL);

    return new_trie;
}

/*************************TrieDestroy*************************************/
static void NodeDestroy(trie_node_t *node)
{
    assert(NULL != node);

    node->parent = NULL;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;
    free(node); node = NULL;
}

static void DestroyTrieNodes(trie_node_t *root)
{
    if (NULL == root)
    {
        return;
    }

    DestroyTrieNodes(root->child[LEFT]);
    DestroyTrieNodes(root->child[RIGHT]);

    if (NULL != root->parent)
    {
        NodeDestroy(root);
        root = NULL;
    }
}

void TrieDestroy(trie_t *trie)
{
    assert(NULL != trie);

    DestroyTrieNodes(&trie->dummy);

    free(trie); trie = NULL;
}

static trie_node_t *NodeCreate(trie_node_t *parent_of_new)
{
    trie_node_t *new_node = (trie_node_t *)malloc(sizeof(*new_node));
    if (NULL == new_node)
    {
        return NULL;
    }

    InitNode(new_node, parent_of_new);

    return new_node;
}

static trie_node_t *FindKeyEnd(trie_t *trie, unsigned int key)
{
    trie_node_t *current_node = NULL;
    int i = 0;

    assert(NULL != trie);

    current_node = &trie->dummy;

    for (i = trie->bits_in_key - 1; 0 <= i; --i)
    {
        unsigned int current_bit = key & (0x1u << i);
        current_bit >>= i;

        if (NULL == current_node->child[current_bit])
        {
            return NULL;
        }

        current_node = current_node->child[current_bit];
    }

    return current_node;
}

int TrieIsKeyFound(trie_t *trie, unsigned int key)
{
	assert(NULL != trie);
	
	return (NULL != FindKeyEnd(trie, key));
}

static int IsDummy(trie_node_t *node)
{
    assert(NULL != node);
    
    return (NULL == node->parent);
}

static void DestroyPath(trie_node_t *current_node)
{
    assert(NULL != current_node);

    while (!IsDummy(current_node) && IsLeaf(current_node))
    {
        trie_node_t *parent = current_node->parent;
        child_direction_t current_child_index = 
                                        FindChildDirection(current_node);

        NodeDestroy(current_node);
        parent->child[current_child_index] = NULL;

        current_node = parent;
    }
}

insertion_status_t TrieInsert(trie_t *trie, unsigned int key)
{
    trie_node_t *current_node = NULL;
    insertion_status_t status = ALREADY_INSERTED;
    int i = 0;

    assert(NULL != trie);

    current_node = &trie->dummy;

    for (i = trie->bits_in_key - 1; 0 <= i; --i)
    {
        unsigned int current_bit = key & (0x1u << i);
        current_bit >>= i;

        if (NULL == current_node->child[current_bit])
        {
            current_node->child[current_bit] = NodeCreate(current_node);
            if (NULL == current_node->child[current_bit])
            {
                DestroyPath(current_node);
                current_node = NULL;

                return FAILED_ALLOCATION;
            }
            
            status = INSERT_SUCCESS;
        }

        current_node = current_node->child[current_bit];
    }

    return status;
}

void TrieRemove(trie_t *trie, unsigned int key)
{
    trie_node_t *node_key_end = NULL;

    assert(NULL != trie);

    node_key_end = FindKeyEnd(trie, key);
    if (NULL == node_key_end)
    {
        return;
    }

    DestroyPath(node_key_end);
    node_key_end = NULL;
}

int TrieIsEmpty(const trie_t *trie)
{
    assert(NULL != trie);

    return IsLeaf(&trie->dummy);
}

static size_t RecCountUsedPaths(trie_node_t *current_node)
{
    if (NULL == current_node)
    {
        return 0;
    }

    if (IsLeaf(current_node))
    {
        return 1;
    }

    return RecCountUsedPaths(current_node->child[LEFT]) +
           RecCountUsedPaths(current_node->child[RIGHT]); 
}

size_t TrieCountUsedPaths(trie_t *trie)
{
    assert(NULL != trie);

    if (TrieIsEmpty(trie))
    {
        return 0;
    }

    return RecCountUsedPaths(&trie->dummy);
}
