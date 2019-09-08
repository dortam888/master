#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sorting.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static int CmpFunc (const void * a, const void * b) 
{
    assert(NULL != a);
    assert(NULL != b);

    return (*(int*)a - *(int*)b);
}

static int CmpArr(const int* arr1, const int* arr2, size_t size)
{
    size_t i = 0;

    assert(NULL != arr1);
    assert(NULL != arr2);

    for (i = 0; i < size; ++i)
    {
        if (arr1[i] != arr2[i])
        {
            printf("in index %lu: %d != %d \n", i, arr1[i], arr2[i]);
            return 0;
        }
    }
    
    return 1;
}

static void TestSort(void(*sort_function)(int *arr, size_t size))
{
    int arr01[1] = {0};
    int arr02[1] = {0};
    int arr11[8] = {-6,-10,-77,-42,-8,-1,-100,-2};
    int arr12[8] = {-6,-10,-77,-42,-8,-1,-100,-2};
    int arr21[8] = {6,10,77,42,8,1,100,2};
    int arr22[8] = {6,10,77,42,8,1,100,2};
    int arr31[8] = {-6,10,-77,42,-8,1,-100,0};
    int arr32[8] = {-6,10,-77,42,-8,1,-100,0};
    int arr41[8] = {-1,0,1,3,6,10,42,77};
    int arr42[8] = {-1,0,1,3,6,10,42,77};
    static size_t sort_index;
    /*size_t i = 0;*/
    char *sort_func[] = {"Insertion Sort", "Selection Sort", "Bubble Sort"};
    size_t error_counter = 0;

    sort_function(arr01, 1);
    qsort(arr02, 1, sizeof(int), CmpFunc);

    if (!CmpArr(arr01, arr02, 1))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr11, 8);
    qsort(arr12, 8, sizeof(int), CmpFunc);
    
    /*
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr11[i]);
    }
    for (i = 0; i < 8; ++i)
    {
        printf("%d", arr12[i]);
    }*/
    
    if (!CmpArr(arr11, arr12, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr21, 8);
    qsort(arr22, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr21, arr22, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr31, 8);
    qsort(arr32, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr31, arr32, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr41, 8);
    qsort(arr42, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr41, arr42, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    if (0 == error_counter)
    {
        PASS(sort_func[sort_index]);
        ++sort_index;
    }
    
}

int main()
{
    TestSort(Insertion);
    TestSort(Selection);
    TestSort(Bubble);

    return 0;
}
