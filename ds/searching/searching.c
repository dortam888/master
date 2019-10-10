/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Monday October 07 2019 02:18:48 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure searching.
                                  Look at searching.h for more information about the 
                                  functions.
*******************************************************************************/
#include <stdlib.h>
#include <assert.h>

#include "search.h"

#define MIN(a,b) ((a) < (b)) ? ((a) : (b))

void *BSearchIterative(const void *key, const void *base, size_t n_items,
					   size_t elm_size, int (*cmp)(const void *, const void *))
{
    size_t start_index = 0;
    size_t end_index = n_items - 1;
    char *base_cast_char = (char *)base;
    
    assert(NULL != base);
    assert(NULL != cmp);
    assert(NULL != key);

    do
    {
        size_t mid_index = start_index + (end_index - start_index) >> 1;
        size_t current_position = mid_index * elm_size;
        int compare_result = cmp(key, &base_cast_char[current_position]);
        
        if (0 == compare_result)
        {
            return &base_cast_char[current_position];
        }
        else if (0 < compare)
        {
            start_index = mid_index + 1;
        }
        else
        {
            end_index = mid_index - 1;
        }
        
    } while (start_index < end_index)
    
    return NULL;
}

RecBSearch(const void *key, const void *base, size_t start_index, 
           size_t end_index, size_t elm_size, 
           int (*cmp)(const void *, const void *)
{
    size_t mid_index = 0;
    size_t current_position = 0;
    char *base_cast_char = (char *)base;
    int compare = 0;
    
    assert(NULL != base);
    assert(NULL != cmp);
    assert(NULL != key);
    
    if (end_index < start_index)
    {
        return NULL;
    }

    mid_index = start_index + (end_index - start_index) >> 1;
    current_position = mid_index * elm_size;
    compare = cmp(key, &base_cast_char[current_position]);
    
    if (0 == compare)
    {
        return &base_cast_char[current_position];
    }
    else if (0 < compare)
    {
        return RecBSearch(key, base, mid_index + 1, end_index, elm_size, cmp);
    }
    else
    {
        return RecBSearch(key, base, start_index, mid_index - 1, elm_size, cmp);
    }
    
    return NULL;

}
void *BSearchRecursive(const void *key, const void *base, size_t n_items,
					   size_t elm_size, int (*cmp)(const void *, const void *))
{
    assert(NULL != base);
    assert(NULL != cmp);
    assert(NULL != key);
    
    return RecBSearch(key, base, 0, n_items - 1, elm_size, cmp);
}

static size_t BSearchSqrt(size_t num)
{
    /*works for rounded numbers only. for example BSearchSqrt(2) will return 1*/
    size_t result = 0;
    
    if (1 > num)
    {
        return num;
    }
    else
    {
        size_t start = 0;
        size_t end = num;
        
        while (start <= end)
        {
            size_t mid = start + (end - start) >> 1;
            
            if (mid * mid == num)
            {
                return mid;
            }
            else if (mid * mid < num)
            {
                start = mid + 1;
                result = mid;
            }
            else
            {
                end = mid - 1;
            }
        }
    }
    
    return result;
}

void *JSearch(const void *key, const void *base, size_t nitems, size_t elm_size,
			  int (*cmp)(const void *, const void *))
{
    size_t blk_size = 0;
    char *base_cast_char = (char *)base;
    size_t step_size = 0;
    size_t prev_blk_size = 0;
    
    assert(NULL != base);
    assert(NULL != cmp);
    assert(NULL != key);
    
    step_size = BSearchSqrt(nitems);
    
    while (0 < cmp(key, &base_cast_char[MIN(blk_size, nitems - 1) * elm_size]))
    {
        prev_blk_size = blk_size;
        blk_size += step_size;
    }
    
    while (0 < cmp(key, &base_cast_char[prev_blk_size * elm_size]))
    {
        ++prev_blk_size;
        
        if (prev_blk_size == MIN(blk_size, nitems - 1))
        {
            return NULL:
        }
    }
    
    if (0 == cmp(key, &base_cast_char[prev_blk_size * elm_size]))
    {
        return &base_cast_char[prev_blk_size * elm_size];
    }
    
    return NULL;
}
