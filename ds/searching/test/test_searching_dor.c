#include <stdio.h>
#include <assert.h>
#include "search.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static int CmpFunc (const void * a, const void * b) 
{
    assert(NULL != a);
    assert(NULL != b);

    return (*(int*)a - *(int*)b);
}

static void TestSearching(void*(*search_func)(const void *, const void *, 
                          size_t, size_t,
                          int(*cmp)(const void *, const void *)))
{
    size_t error_counter = 0;
    int sorted_array1[] = {0};
    int sorted_array2[] = {-9, -4, 3, 5};
    int sorted_array3[] = {0, 1, 2, 3, 4, 5, 6};
    int data_to_find1 = 0;
    int data_to_find2[4] = {-9, -4, 3, 5};
    int data_to_find3[7] = {0, 1, 2, 3, 4, 5, 6};
    int data_not_to_find1 = -1;
    int data_not_to_find2 = -1;
    int data_not_to_find3 = 8;
    size_t sorted_array_size1 = sizeof(sorted_array1)/sizeof(*sorted_array1);
    size_t sorted_array_size2 = sizeof(sorted_array2)/sizeof(*sorted_array2);
    size_t sorted_array_size3 = sizeof(sorted_array3)/sizeof(*sorted_array3);
    size_t i = 0;

    if (&sorted_array1[0] != 
    search_func(&data_to_find1, sorted_array1, sorted_array_size1, 
                     sizeof(int), CmpFunc))
    {
        ++error_counter;
        FAIL("should find");
    }
    
    if (NULL != 
    search_func(&data_not_to_find1, sorted_array1, sorted_array_size1, 
                     sizeof(int), CmpFunc))
    {
        ++error_counter;
        FAIL("shouldn't find");
    }
    
    for (i = 0; i < sorted_array_size2; ++i)
    {
        if (&sorted_array2[i] != 
        search_func(&data_to_find2[i], sorted_array2, sorted_array_size2, 
                         sizeof(int), CmpFunc))
        {
            ++error_counter;
            printf("%d in index %lu\n", sorted_array2[i], i);
            FAIL("should find");
        }
    }

    if (NULL != 
    search_func(&data_not_to_find2, sorted_array2, sorted_array_size2, 
                     sizeof(int), CmpFunc))
    {
        ++error_counter;
        FAIL("shouldn't find");
    }

    for (i = 0; i < sorted_array_size3; ++i)
    {
        if (&sorted_array3[i] != 
        search_func(&data_to_find3[i], sorted_array3, sorted_array_size3, 
                         sizeof(int), CmpFunc))
        {
            ++error_counter;
            printf("%d in index %lu\n", sorted_array3[i], i);
            FAIL("should find");
        }
    }

    if (NULL != 
    search_func(&data_not_to_find3, sorted_array3, sorted_array_size3, 
                     sizeof(int), CmpFunc))
    {
        ++error_counter;
        FAIL("shouldn't find");
    }

    if (0 == error_counter)
    {
        PASS("Searching");
    }
}

int main()
{
    TestSearching(BSearchIterative);
    TestSearching(BSearchRecursive);
    TestSearching(JSearch);

    return 0;
}
