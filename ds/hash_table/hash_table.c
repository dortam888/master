/*******************************************************************************
**** Author: Dor Tambour
**** Last Update:
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure hash_table.
                                  Look at hash_table.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "doubly_linked_list.h"
#include "hash_table.h"

typedef size_t(*hash_func_t)(void *data);
typedef int(*hash_cmp_func_t)(const void *key1, const void *key2);

struct hash_table
{
    void **hash_array;
    size_t num_of_buckets;
    hash_func_t hash_func;
    hash_cmp_func_t hash_cmp_func;
};

static void CreateHashArray(hash_table_t *new_hash, size_t num_of_buckets)
{
    size_t i = 0;
    int is_alloc_succeed = 1;
    
    new_hash->hash_array = (void *)malloc(num_of_buckets * sizeof(void *));
    if (NULL == new_hash->hash_array)
    {
        return;
    }
    
    for (i = 0; i < num_of_buckets; ++i)
    {
        new_hash->hash_array[i] = DlistCreate();
        if (NULL == new_hash->hash_array[i])
        {
            is_alloc_succeed = 0;
            break;
        }
    }
    
    if (!is_alloc_succeed)
    {
        for (i = i; i > 0; --i)
        {
            DlistDestroy(new_hash->hash_array[i - 1]);
        }
    
        free(new_hash->hash_array); new_hash->hash_array = NULL;
    }

}

static void InitHash(hash_table_t *new_hash, size_t num_of_buckets, 
                     hash_func_t hash_func, hash_cmp_func_t hash_cmp_func)
{
    CreateHashArray(new_hash, num_of_buckets);
    if (NULL == new_hash->hash_array)
    {
        return;
    }

    new_hash->num_of_buckets = num_of_buckets;
    new_hash->hash_func = hash_func;
    new_hash->hash_cmp_func = hash_cmp_func;
}

hash_table_t *HashTableCreate(size_t num_of_buckets, hash_func_t hash_func,
                              hash_cmp_func_t hash_cmp_func)
{
	hash_table_t *new_hash = (hash_table_t *)malloc(sizeof(*new_hash));
	if (NULL == new_hash)
	{
	    return NULL;
	}

    InitHash(new_hash, num_of_buckets, hash_func, hash_cmp_func);
    if (NULL == new_hash->hash_array)
    {
        free(new_hash); new_hash = NULL;
        return NULL;
    }
    
    return new_hash;
}

void HashTableDestroy(hash_table_t *hash_table)
{
    size_t i = 0;
    
    for (i = 0; i < hash_table->num_of_buckets; ++i)
    {
        DlistDestroy(hash_table->hash_array[i]);
    }
    
    free(hash_table->hash_array); hash_table->hash_array = NULL;
    
    hash_table->hash_cmp_func = NULL;
    hash_table->hash_func = NULL;
    
    free(hash_table); hash_table = NULL;
}

int HashTableInsert(hash_table_t *hash_table, void *data)
{
    dlist_t *datas_dlist = NULL;

    assert(NULL != hash_table);

    datas_dlist = hash_table->hash_array[hash_table->hash_func(data)];
    
    return (DlistPushFront(datas_dlist, data) != DlistEnd(datas_dlist));
}

void HashTableRemove(hash_table_t *hash_table, void *data)
{
    dlist_t *datas_dlist = NULL;
    
    assert(NULL != hash_table);
    
    datas_dlist = hash_table->hash_array[hash_table->hash_func(data)];
    DlistRemove(DlistFind(datas_dlist, DlistBegin(datas_dlist), 
                          DlistEnd(datas_dlist), hash_table->hash_cmp_func, 
                          data));
}

void *HashTableFind(const hash_table_t *hash_table, void *data)
{
    dlist_t *datas_dlist = NULL;
    
    assert(NULL != hash_table);
    
    datas_dlist = hash_table->hash_array[hash_table->hash_func(data)];

    return DlistGetData(DlistFind(datas_dlist, DlistBegin(datas_dlist), 
                        DlistEnd(datas_dlist), hash_table->hash_cmp_func, 
                        data));
}

int HashTableForEach(const hash_table_t *hash_table,
					 int (*act_func)(void *table_data, void *param),
					 void *param)
{
    size_t i = 0;
    size_t num_of_buckets = 0;
    int act_func_status = 0;
    
    assert(NULL != hash_table);
    assert(NULL != act_func);
    
    num_of_buckets = hash_table->num_of_buckets;

    for (i = 0; (i < num_of_buckets) && (0 == act_func_status); ++i)
    {
        dlist_t *cur_dlist = hash_table->hash_array[i];
        
        act_func_status = DlistForEach(DlistBegin(cur_dlist), 
                                       DlistEnd(cur_dlist), act_func, param);
    }
    
    return act_func_status;
}

size_t HashTableSize(const hash_table_t *hash_table)
{
    size_t i = 0;
    size_t num_of_buckets = 0;
    size_t hash_size = 0;
    
    assert(NULL != hash_table);
    
    num_of_buckets = hash_table->num_of_buckets;
    
    for (i = 0; i < num_of_buckets; ++i)
    {
        hash_size += DlistSize(hash_table->hash_array[i]);
    }
    
    return hash_size;
}

int HashTableIsEmpty(const hash_table_t *hash_table)
{
    size_t i = 0;
    size_t num_of_buckets = 0;
    
    assert(NULL != hash_table);
    
    num_of_buckets = hash_table->num_of_buckets;
    
    for (i = 0; i < num_of_buckets; ++i)
    {
        if (!DlistIsEmpty(hash_table->hash_array[i]))
        {
            return 0;
        }
    }
    
    return 1;
}
