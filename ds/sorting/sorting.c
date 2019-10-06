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

typedef int(*cmp_func_t)(const void *, const void *);

static void SwapByte(char *data1, char *data2)
{
    for (j = 0; j < element_size; ++j)
    {
        char tmp = *data1;
        *data1 = *data2;
        *data2 = tmp;
        ++pivot;
        ++(start_of_base);
    }
}

static size_t ArrangeOnPivot(void *base, size_t element_size, size_t pivot_index, 
                            cmp_func_t cmp_func)
{
    char *pivot = NULL;
    size_t i = 0;

    assert(NULL != base);
    pivot = (char *)base + pivot_index * element_size;
    
    for (i = 0; i < pivot_index; ++i)
    {
        char *start_of_base = (char *)base;
        if (cmp_func(start_of_base + i * element_size, pivot) > 0)
        {
            SwapByteByByte(start_of_base + i * element_size, 
                           pivot, size_of_element);
            
            pivot = start_of_base + i * element_size;
        }
    }
    
    return (pivot - (char *)base) / element_size;
}

static void RecQSort(void *base, size_t element_size, size_t start_index, 
                     size_t end_index, cmp_func_t cmp_func)
{
    size_t pivot_index = end_index;

    assert(NULL != base);

    if (start_index >= end_index)
    {
        return;
    }
    
    pivot_index = ArrangeOnPivot(base, element_size, pivot_index, cmp_func);
    RecQSort(base, element_size, start_index, pivot_index - 1, cmp_func);
    RecQSort(base, element_size, pivot_index + 1, end_index, cmp_func);
}

void QSort(void *base, size_t num_of_elements, size_t element_size, 
           cmp_func_t cmp_func)
{ 
    assert(NULL != base);

    RecQSort(base, element_size, 0, num_of_elements - 1, cmp_func);
}
