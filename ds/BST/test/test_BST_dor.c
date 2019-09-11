#include <stdio.h>
#include <stdlib.h>

#include "BST.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

static int IntCmpFunc(const void *num1, const void *num2, void *params)
{
    UNUSED(params);
    return (*(int *)num2 - *(int *)num1);
}

static void TestFlow1()
{
    int arr_of_numbers[] = {11, 42, 50, 33, 12, 15, 29, 40, 32};
    size_t i = 0LU;
    size_t error_counter = 0LU;
    bst_t *new_bst = BSTCreate(IntCmpFunc, NULL);

    if (!BSTIsEmpty(new_bst))
    {
        ++error_counter;
        FAIL("IsEmpty didn't work");
    }

    for(i = 0LU; i < sizeof(arr_of_numbers)/sizeof(*arr_of_numbers); ++i)
    {
        BSTInsert(new_bst, &arr_of_numbers[i]);
    }

    if (arr_of_numbers[0] != *(int *)BSTGetData(BSTBegin(new_bst)))
    {
        ++error_counter;
        FAIL("didn't work");
    }
    
    if (9 != BSTSize(new_bst))
    {
        ++error_counter;
        printf("size was %lu\n", BSTSize(new_bst));
        FAIL("size didn't work");
    }

    BSTDestroy(new_bst);

    if (0 == error_counter)
    {
        PASS("TestFlow1");
    }
}

int main()
{
    TestFlow1();
    return 0;
}
