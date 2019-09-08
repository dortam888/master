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

#include "sorting.h"

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
