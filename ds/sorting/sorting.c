/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Sunday September 08 2019 10:26:51 
**** Reviewer: Adi
**** Description: This file contains the implementations of sorting functions
                                  for the data structure array.
                                  Look at sorting.h for more information about the 
                                  functions.
*******************************************************************************/
#include <assert.h> /* assert */
#include <unistd.h> /*ssize_t*/
#include <limits.h> /*CHAR_BIT*/
#include <stdlib.h> /*malloc calloc free*/
#include <string.h> /*memcpy*/

#include "sorting.h"

#define INT_BIT CHAR_BIT * sizeof(int)

static void Swap(int *num1, int *num2)
{
    int tmp = 0;

    assert(NULL != num1);
    assert(NULL != num2);

    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

void Insertion(int *arr, size_t size)
{
    size_t i = 0;

    assert(NULL != arr);

    for (i = 1; i < size; ++i)
    {
        int current_array_value = arr[i];
        ssize_t j = i - 1;

        while (j >= 0 && arr[j] > current_array_value)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        
        arr[j + 1] = current_array_value;
    }
}

static size_t FindIndexOfMin(int *arr, size_t size, size_t start_index)
{
    size_t index_of_min = start_index;
    size_t i = index_of_min;
    int min_value_of_arr = 0;

    assert(NULL != arr);

    min_value_of_arr = arr[start_index];

    for (i = index_of_min; i < size; ++i)
    {
        if (min_value_of_arr > arr[i])
        {
            min_value_of_arr = arr[i];
            index_of_min = i;
        }
    }

    return index_of_min;
}

void Selection(int *arr, size_t size)
{
    size_t i = 0;
    size_t j = 0;

    assert(NULL != arr);

    for (i = 0; i < size; ++i)
    {
        j = FindIndexOfMin(arr, size, i);
        Swap(&arr[i], &arr[j]);
    }
}

void Bubble(int *arr, size_t size)
{
    size_t i = 0;

    assert(NULL != arr);

    do
    {
        size_t last_index_swapped = 0;

        for (i = 0; i < size - 1; ++i)
        {
            if (arr[i + 1] < arr[i])
            {
                Swap(&arr[i], &arr[i + 1]);
                last_index_swapped = i + 1;
            }
        }

        size = last_index_swapped;
    } while (size > 1);
}

static void FillHistogramWithCounts(size_t *histogram, int min, int *arr, 
                                    size_t size)
{
    int *start_of_arr = NULL;

    assert(NULL != arr);
    assert(NULL != histogram);

    start_of_arr = arr;

    for (; (size_t)(arr - start_of_arr) < size; ++arr)
    {
        ++histogram[*arr - min + 1];
    }

    arr = start_of_arr;
}

static void ShiftHistogramPosition(size_t *histogram, int histogram_range)
{
    size_t *start_of_histogram = NULL;
    size_t *end_of_histogram = NULL;

    assert(NULL != histogram);

    start_of_histogram = histogram;
    end_of_histogram = histogram + histogram_range - 1;

    for (++histogram; histogram < end_of_histogram; ++histogram)
    {
        *histogram += *(histogram - 1);
    }

    histogram = start_of_histogram;
}

static void SortArray(int *arr, int *arr_sorted, size_t size, size_t *histogram, 
                      int min)
{
    int *start_of_arr = NULL;
    int *end_of_arr = NULL;

    assert(NULL != arr);
    assert(NULL != arr_sorted);
    assert(NULL != histogram);
    assert(size > 0);


    for (start_of_arr = arr, end_of_arr = start_of_arr + size; 
         arr < end_of_arr; ++arr)
    {
        int index = *arr - min;
        arr_sorted[histogram[index]] = *arr;
        ++histogram[index];
    }

    arr = start_of_arr;
}

static void *FreeFunction(void *dynamic_allocation)
{
    free(dynamic_allocation);
    return NULL;
}

int Counting(int *arr, size_t size, int min, int max)
{
    size_t *histogram = NULL;
    int *arr_sorted = NULL;
    int histogram_range = max - min + 2;

    assert(NULL != arr);

    histogram = (size_t *)calloc(histogram_range, sizeof(size_t));
    if (NULL == histogram)
    {
        return -1;
    }

    arr_sorted = (int *)malloc(size * sizeof(int));
    if (NULL == arr_sorted)
    {
        free(histogram); histogram = NULL;
        return -1;
    }

    FillHistogramWithCounts(histogram, min, arr, size);
    ShiftHistogramPosition(histogram, histogram_range);
    SortArray(arr, arr_sorted, size, histogram, min);
    arr = memcpy(arr, arr_sorted, size * sizeof(int));

    histogram = FreeFunction(histogram);
    arr_sorted = FreeFunction(arr_sorted);

    return 0;
}

static void FillHistogramWithCountsMasking(size_t *histogram, unsigned int mask,
                                            size_t shifting, int *arr, size_t size)
{
    int *start_of_arr = NULL;
    int *end_of_arr = NULL;

    assert(NULL != arr);
    assert(NULL != histogram);
    assert(size != 0);

    for (start_of_arr = arr, end_of_arr = start_of_arr + size; 
         arr < end_of_arr; ++arr)
    {
        ++histogram[((*arr >> shifting) & mask) + 1];
    }

    arr = start_of_arr; /*return arr to the start point*/
}

static void SortArrayMask(int *arr, int *arr_sorted, size_t size, 
                          size_t *histogram, unsigned int mask, size_t shifting)
{
    int *start_of_arr = NULL;
    int *end_of_arr = NULL;

    assert(NULL != arr);
    assert(NULL != arr_sorted);
    assert(NULL != histogram);

    for (start_of_arr = arr, end_of_arr = start_of_arr + size; 
         arr < end_of_arr; ++arr)
    {
        int index = (*arr >> shifting) & mask;
        arr_sorted[histogram[index]] = *arr;
        ++histogram[index];
    }

    arr = start_of_arr; /*return arr to the start point*/
}

static void SwapArraysPointers(int **arr1, int **arr2)
{
	int *tmp = NULL;

	assert(NULL != arr1); assert(NULL != *arr1);
	assert(NULL != arr2); assert(NULL != *arr2);

	tmp = *arr1;
	*arr1 = *arr2;
	*arr2 = tmp;
}

int Radix(int *arr, size_t size, unsigned int num_of_bits)
{
    size_t *histogram = NULL;
    int *arr_for_sorting = NULL;
    unsigned int mask = ~((~0) << num_of_bits);
    size_t i = 0;
    size_t number_of_sorting = INT_BIT / num_of_bits;
    unsigned int histogram_size = mask + 2;

    assert(NULL != arr);

    histogram = (size_t *)malloc(histogram_size * sizeof(size_t));
    if (NULL == histogram)
    {
        return -1;
    }

    arr_for_sorting = (int *)malloc(size * sizeof(int));
    if (NULL == arr_for_sorting)
    {
        histogram = FreeFunction(histogram);
        return -1;
    }

    for (i = 0; i < number_of_sorting; ++i)
    {
        memset(histogram, 0, histogram_size * sizeof(size_t)); /*initialize histogram with 0*/
        FillHistogramWithCountsMasking(histogram, mask, i * num_of_bits, arr, 
                                       size);
        ShiftHistogramPosition(histogram, histogram_size);
        SortArrayMask(arr, arr_for_sorting, size, histogram, mask, 
                      i * num_of_bits);
        SwapArraysPointers(&arr, &arr_for_sorting);
    }

    histogram = FreeFunction(histogram);
    arr_for_sorting = FreeFunction(arr_for_sorting);

    return 0;
}

static size_t GetArrayLength(size_t start_index, size_t end_index)
{
    return (end_index - start_index + 1);
}

static void MergeArrays(int *arr, size_t subarray_left_start_index, 
                        size_t subarray_left_end_index, 
                        size_t subarray_right_end_index,
			            int (*is_before)(int a, int b), int *memory_pool)
{
	size_t i = 0, j = 0, k = 0; /*indexes*/
	size_t subarray_left_length = GetArrayLength(subarray_left_start_index,
	                                             subarray_left_end_index);
	size_t subarray_right_start_index = subarray_left_end_index + 1;
	size_t subarray_right_length =  GetArrayLength(subarray_right_start_index,
	                                               subarray_right_end_index);

	memcpy(memory_pool, (arr + subarray_left_start_index), 
	       subarray_left_length * sizeof(int));
	memcpy(memory_pool + subarray_left_length, 
	      (arr + subarray_right_start_index), 
	       subarray_right_length * sizeof(int));

	for (i = subarray_left_start_index, j = 0, k = 0; 
	    (j < subarray_left_length) && (k < subarray_right_length); 
	     ++i)
	{
	    size_t memory_pool_offset_index = k + subarray_left_length;

		if(is_before(memory_pool[j], memory_pool[memory_pool_offset_index]))
		{ 
			arr[i] = memory_pool[j];
			++j; 
		}
		else
		{ 
			arr[i] = memory_pool[memory_pool_offset_index];
			++k; 
		} 
	}

	while (j < subarray_left_length) 
	{ 
		arr[i] = memory_pool[j];
		++i;
		++j; 
	} 

	while (k < subarray_right_length) 
	{ 
		arr[i] = memory_pool[k + subarray_left_length];
		++i;
		++k; 
	}
}

static void RecMerge(int *arr, size_t start_index, size_t end_index, 
                    int (*is_before)(int num1, int num2), int *memory_pool)
{
	size_t arr_split_index = 0;
	
	assert(NULL != arr);
	assert(NULL != is_before);

    arr_split_index = (start_index + end_index) >> 1;

	if (start_index >= end_index) 
	{
        return;
	}
	
	RecMerge(arr, start_index, arr_split_index, is_before, memory_pool);
	RecMerge(arr, arr_split_index + 1, end_index, is_before, memory_pool);
	
	MergeArrays(arr, start_index, arr_split_index, end_index, 
	            is_before, memory_pool);
}

int Merge(int *arr, size_t arr_length, int (*is_before)(int a, int b))
{
    int *memory_pool = NULL;
    
    assert(NULL != arr);
    assert(NULL != is_before);
    
    memory_pool = (int *)malloc(arr_length * sizeof(int));
    if (NULL == memory_pool)
    {
        return -1;
    }

	RecMerge(arr, 0, arr_length - 1, is_before, memory_pool);

	free(memory_pool);

	return 0;
}

typedef int(*cmp_func_t)(const void *data1, const void *data2);

static void ByteSwap(char *byte1, char *byte2, size_t element_size)
{
    size_t i = 0;

    assert(NULL != byte1);
    assert(NULL != byte2);

    for (i = 0; i < element_size; ++i)
    {
        char tmp = *byte1;
        *byte1 = *byte2;
        *byte2 = tmp;
        
        ++byte1;
        ++byte2;
    }

    return;
}

static size_t Partition(void *base, size_t start_index, size_t end_index, 
                        size_t element_size, cmp_func_t cmp_func)
{
    char *base_cast_char = (char *)base;
    size_t pivot = 0;
    
    assert(NULL != base);
    assert(NULL != cmp_func);

    pivot = end_index;

    /* Put the pivot as start index. */
    ByteSwap(&base_cast_char[pivot * element_size], 
             &base_cast_char[start_index * element_size], element_size);
    pivot = start_index;

    while(start_index < end_index)
    {
        if(cmp_func(&base_cast_char[start_index * element_size], 
                    &base_cast_char[end_index * element_size]) <= 0)
        {
            ByteSwap(&base_cast_char[pivot * element_size], 
                     &base_cast_char[start_index * element_size], element_size);
            ++pivot;
        }

        ++start_index;
    }

    ByteSwap(&base_cast_char[pivot * element_size], 
             &base_cast_char[end_index * element_size], element_size);

    return pivot;
}

static void RecQSort(void *base, size_t start_index, size_t end_index, 
                     size_t element_size, cmp_func_t cmp_func)
{
    size_t pivot = 0;
    
    assert(NULL != base);
    assert(NULL != cmp_func);

    if (start_index >= end_index)
    {
        return;
    }
    
    pivot = Partition(base, start_index, end_index, element_size, cmp_func);
    RecQSort(base, start_index, pivot - 1, element_size, cmp_func);
    RecQSort(base, pivot + 1, end_index, element_size, cmp_func);

}

void QSort(void *base, size_t num_of_elements, size_t element_size, 
           cmp_func_t cmp_func)
{
    assert(NULL != base);
    assert(NULL != cmp_func);
    
    RecQSort(base, 0, num_of_elements - 1, element_size, cmp_func);
}
