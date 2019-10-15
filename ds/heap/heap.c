/*******************************************************************************
**** Author: Dor Tambour
**** Last Update:
**** Reviewer: Hila
**** Description: This file contains the implementations of functions
                                  for the data structure heap.
                                  Look at heap.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h>
#include <assert.h>

#include "d_vector.h"
#include "heap.h"

typedef int(*cmp_func_t)(const void *data1, const void *data2, void *param);

struct heap
{
    d_vector_t *vector;
    cmp_func_t cmp_priority;
    void *param;
};

heap_t *HeapCreate(cmp_func_t cmp_priority, void *param)
{
    heap_t *new_heap = (heap_t *)malloc(sizeof(*new_heap));
    if (NULL == new_heap)
    {
        return NULL;
    }
    
    new_heap->vector = DVectorCreate(sizeof(void *), 1);
    if (NULL == new_heap->vector)
    {
        free(new_heap); new_heap = NULL;
        return NULL;
    }
    
    new_heap->cmp_priority = cmp_priority;
    new_heap->param = param;
    
    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    
    DVectorDestroy(heap->vector);
    heap->param = NULL;
    heap->cmp_priority = NULL;
    
    free(heap); heap = NULL; 
}

static void Swap(void *data1, void *data2)
{
    size_t tmp = *(size_t *)data1;
    *(size_t *)data1 = *(size_t *)data2;
    *(size_t *)data2 = tmp;
}

static size_t GetLastIndex(const heap_t *heap)
{
    assert(NULL != heap);
    
    if (HeapIsEmpty(heap))
    {
        return 0;
    }

    return (HeapSize(heap) - 1);
}

static size_t GetParentIndex(size_t index)
{
    return (index > 0)? (index - 1) >> 1 : 0;
}

static void HeapUp(heap_t *heap, size_t index)
{
    void *current_data = NULL;
    void *parent_data = NULL;
    size_t parent_index = 0;

    assert(NULL != heap);

    if (GetParentIndex(index) == index)
    {
        return;
    }

    parent_index = GetParentIndex(index);
    current_data = DVectorGetItemAddress(heap->vector, index);
    parent_data = DVectorGetItemAddress(heap->vector, parent_index);

    if (heap->cmp_priority(parent_data, current_data, heap->param) > 0)
    {
        Swap(current_data, parent_data);
    }
    
    HeapUp(heap, parent_index);
}

static void HeapDown(heap_t *heap, size_t index)
{
    size_t left_child_index = -(~(index << 1));
    size_t right_child_index = -(~left_child_index);
    size_t heap_size = HeapSize(heap);
    size_t index_of_largest = 0;
    void *data_of_largest = NULL;
    void *parent_data = NULL;
    void *left_child_data = NULL;
    void *right_child_data = NULL;

    assert(NULL != heap);

    parent_data = DVectorGetItemAddress(heap->vector, index);
    data_of_largest = parent_data;

    if (left_child_index < heap_size)
    {
        left_child_data = DVectorGetItemAddress(heap->vector, left_child_index);
        if (heap->cmp_priority(left_child_data, parent_data, heap->param) < 0)
        {
            index_of_largest = left_child_index;
            data_of_largest = left_child_data;
        }
    }
    
    if (right_child_index < heap_size)
    {
        right_child_data = DVectorGetItemAddress(heap->vector, 
                                                 right_child_index);
        if (heap->cmp_priority(right_child_data, 
                               data_of_largest, heap->param) < 0)
        {
            index_of_largest = right_child_index;
            data_of_largest = right_child_data;
        }
    }

    if (data_of_largest != parent_data)
    {
        Swap(data_of_largest, parent_data);
        HeapDown(heap, index_of_largest);
    }
}

void Heapify(heap_t *heap)
{
    assert(NULL != heap);
    
    if (HeapSize(heap) > 2)
    {
        HeapDown(heap, 0);
    }
    HeapUp(heap, GetLastIndex(heap));
}

int HeapPush(heap_t *heap, const void *data)
{
    enum heap_status status = 0;

    assert(NULL != heap);

    status = DVectorPushBack(heap->vector, data);
    if (0 != status)
    {
        return HEAP_ALLOC_FAIL;
    }

    Heapify(heap);

    return HEAP_SUCCESS;
}

void HeapPop(heap_t *heap)
{
    void *top_data = NULL;
    void *last_data = NULL;

    assert(NULL != heap);

    top_data = HeapPeek(heap);
    last_data = DVectorGetItemAddress(heap->vector, GetLastIndex(heap));
    
    Swap(top_data, last_data);

    DVectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        Heapify(heap);
    }
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    
    return DVectorGetItemAddress(heap->vector, 0); /*0 is start index*/
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    
    return DVectorSize(heap->vector);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);
    
    return DVectorIsEmpty(heap->vector);
}

void *HeapRemove(heap_t *heap, int(*is_match)(const void *heap_data,
			   	 const void *user_data), void *data)
{
    size_t i = 0;
    void *removed_data = NULL;
    void *last_data = NULL;

    assert(NULL != heap);
    assert(NULL != is_match);

    last_data = DVectorGetItemAddress(heap->vector, GetLastIndex(heap));

    for (i = 0; i < HeapSize(heap); ++i)
    {
        void *heap_data = DVectorGetItemAddress(heap->vector, i);

        if (is_match(heap_data, data))
        {
            removed_data = last_data;
            Swap(heap_data, last_data);
            DVectorPopBack(heap->vector);
            Heapify(heap);
        }
    }
    
    return removed_data;
}
