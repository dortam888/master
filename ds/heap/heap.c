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

/**********************Static Help Functions***********************************/
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

static void *GetDataFromVectorPosition(const void *vector_position)
{
    return (*(void **)vector_position);
}

static void SiftUp(heap_t *heap, size_t index)
{
    void *current_data = NULL;
    void *parent_data = NULL;
    size_t parent_index = 0;

    assert(NULL != heap);

    if (0 == index)
    {
        return;
    }

    parent_index = GetParentIndex(index);
    current_data = DVectorGetItemAddress(heap->vector, index);
    parent_data = DVectorGetItemAddress(heap->vector, parent_index);

    if (heap->cmp_priority(*(void **)parent_data, *(void **)current_data, 
        heap->param) > 0)
    {
        Swap(current_data, parent_data);
    }
    
    SiftUp(heap, parent_index);
}

static void SiftDown(heap_t *heap, size_t index)
{
    size_t left_child_index = -(~(index << 1));
    size_t right_child_index = -(~left_child_index);
    size_t heap_size = 0;
    size_t index_of_largest = 0;
    void *position_of_largest_in_vector = NULL;
    void *parent_vector_pos = NULL;

    assert(NULL != heap);

    heap_size = HeapSize(heap);
    parent_vector_pos = DVectorGetItemAddress(heap->vector, index);
    position_of_largest_in_vector = parent_vector_pos;

    if (left_child_index < heap_size)
    {
        void *left_child_vector_pos = DVectorGetItemAddress(heap->vector, 
                                      left_child_index);
        if (heap->cmp_priority(GetDataFromVectorPosition(left_child_vector_pos), 
                               GetDataFromVectorPosition(parent_vector_pos),
                               heap->param) < 0)
        {
            index_of_largest = left_child_index;
            position_of_largest_in_vector = left_child_vector_pos;
        }
    }
    
    if (right_child_index < heap_size)
    {
        void *right_child_vector_pos = DVectorGetItemAddress(heap->vector, 
                                       right_child_index);
        if (heap->cmp_priority(GetDataFromVectorPosition
                               (right_child_vector_pos), 
                               GetDataFromVectorPosition
                               (position_of_largest_in_vector), 
                               heap->param) < 0)
        {
            index_of_largest = right_child_index;
            position_of_largest_in_vector = right_child_vector_pos;
        }
    }

    if (position_of_largest_in_vector != parent_vector_pos)
    {
        Swap(position_of_largest_in_vector, parent_vector_pos);
        SiftDown(heap, index_of_largest);
    }
}

static void Sift(heap_t *heap)
{
    assert(NULL != heap);
    
    if (HeapSize(heap) > 2)
    {
        SiftDown(heap, 0);
    }
    
    SiftUp(heap, GetLastIndex(heap));
}

/************************API Functions*****************************************/
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

int HeapPush(heap_t *heap, const void *data)
{
    int status = 0;

    assert(NULL != heap);

    status = DVectorPushBack(heap->vector, &data);
    if (0 != status)
    {
        return HEAP_ALLOC_FAIL;
    }

    Sift(heap);

    return HEAP_SUCCESS;
}

void HeapPop(heap_t *heap)
{
    void *top_data = NULL;
    void *last_data = NULL;

    assert(NULL != heap);

    top_data = DVectorGetItemAddress(heap->vector, 0);
    last_data = DVectorGetItemAddress(heap->vector, GetLastIndex(heap));
    
    Swap(top_data, last_data);

    DVectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        Sift(heap);
    }
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    
    return GetDataFromVectorPosition(DVectorGetItemAddress(heap->vector, 0)); 
    /*0 start index*/
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
    void *last_item_in_vector = NULL;

    assert(NULL != heap);
    assert(NULL != is_match);

    last_item_in_vector = 
    DVectorGetItemAddress(heap->vector, GetLastIndex(heap));

    for (i = 0; i < HeapSize(heap); ++i)
    {
        void *heap_pos_in_vector = DVectorGetItemAddress(heap->vector, i);
        void *heap_data = GetDataFromVectorPosition(heap_pos_in_vector);

        if (is_match(heap_data, data))
        {
            removed_data = heap_data;
            Swap(heap_pos_in_vector, last_item_in_vector);
            DVectorPopBack(heap->vector);
            Sift(heap);
        }
    }

    return removed_data;
}
