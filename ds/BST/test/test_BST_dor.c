#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

static int IntCmpFunc(void *num1, void *num2, void *params)
{
    UNUSED(params);
    return (*(int *)num1 - *(int *)num2);
}

static int AddParamToData(void *iter_data, void *param)
{
    *(int *)iter_data = *(int *)iter_data + *(int *)param;
    return 0;
}

static void TestFlow1()
{
    int arr_of_numbers[] = {11, 42, 50, 33, 12, 15, 29, 40, 32, 9};
    bst_iter_t iter[10] = {NULL};
    size_t i = 0LU;
    size_t error_counter = 0LU;
    bst_t *new_bst = BSTCreate(IntCmpFunc, NULL);

    if (!BSTIsEmpty(new_bst))
    {
        ++error_counter;
        FAIL("IsEmpty didn't work");
    }
    
    if (BSTSize(new_bst) != 0)
    {
        ++error_counter;
        FAIL("Size didn't work");
    }

    for(i = 0LU; i < sizeof(arr_of_numbers)/sizeof(*arr_of_numbers); ++i)
    {
        iter[i] = BSTInsert(new_bst, &arr_of_numbers[i]);
    }

    if (BSTIsEmpty(new_bst))
    {
        ++error_counter;
        FAIL("IsEmpty didn't work");
    }

    if (10 != BSTSize(new_bst))
    {
        ++error_counter;
        printf("size was %lu\n", BSTSize(new_bst));
        FAIL("size didn't work");
    }

    if (arr_of_numbers[9] != *(int *)BSTGetData(BSTBegin(new_bst)))
    {
        ++error_counter;
        printf("minimum value should be %d, you get %d\n", arr_of_numbers[9], 
                                        *(int *)BSTGetData(BSTBegin(new_bst)));
        FAIL("didn't work");
    }

    if (!BSTIsSameIter(BSTNext(iter[2]), BSTEnd(new_bst)))
    {
        ++error_counter;
        FAIL("didn't work");
    }


    if (arr_of_numbers[0] != *(int *)BSTGetData(BSTNext(BSTBegin(new_bst))))
    {
        ++error_counter;
        printf("value should be %d, you get %d\n", arr_of_numbers[0], 
                                *(int *)BSTGetData(BSTNext(BSTBegin(new_bst))));
        FAIL("didn't work");
    }

    if (arr_of_numbers[4] != *(int *)BSTGetData(BSTNext(BSTNext(BSTBegin(new_bst)))))
    {
        ++error_counter;
        printf("value should be %d, you get %d\n", arr_of_numbers[4], 
                      *(int *)BSTGetData(BSTNext(BSTNext(BSTBegin(new_bst)))));
        FAIL("didn't work");
    }

    BSTRemove(iter[4]);

    if (arr_of_numbers[9] != *(int *)BSTGetData(BSTBegin(new_bst)))
    {
        ++error_counter;
        printf("minimum value should be %d, you get %d\n", arr_of_numbers[9], 
                                        *(int *)BSTGetData(BSTBegin(new_bst)));
        FAIL("didn't work");
    }
    

    if (arr_of_numbers[0] != *(int *)BSTGetData(BSTNext(BSTBegin(new_bst))))
    {
        ++error_counter;
        printf("value should be %d, you get %d\n", arr_of_numbers[0], 
                                *(int *)BSTGetData(BSTNext(BSTBegin(new_bst))));
        FAIL("didn't work");
    }

    if (arr_of_numbers[5] != *(int *)BSTGetData(BSTNext(BSTNext(BSTBegin(new_bst)))))
    {
        ++error_counter;
        printf("value should be %d, you get %d\n", arr_of_numbers[5], 
                      *(int *)BSTGetData(BSTNext(BSTNext(BSTBegin(new_bst)))));
        FAIL("didn't work");
    }
    
    if (9 != BSTSize(new_bst))
    {
        ++error_counter;
        printf("size was %lu\n", BSTSize(new_bst));
        FAIL("size didn't work");
    }
    
    if (*(int *)BSTGetData(BSTNext(iter[3])) != 40)
    {
        ++error_counter;
        FAIL("FAIL");
    }
    
    if (*(int *)BSTGetData(BSTPrev(iter[3])) != 32)
    {
        ++error_counter;
        FAIL("FAIL");
    }

    BSTDestroy(new_bst);

    if (0 == error_counter)
    {
        PASS("TestFlow1");
    }
}

static void TestFlowForEachFind()
{
    int arr_of_numbers[] = {11, 42, 50, 33, 12, 15, 29, 40, 32, 9};
    int to_find = 50;
    int param_to_function = 1;
    bst_iter_t iter[10] = {NULL};
    size_t i = 0LU;
    size_t error_counter = 0LU;
    bst_t *new_bst = BSTCreate(IntCmpFunc, NULL);

    for(i = 0LU; i < sizeof(arr_of_numbers)/sizeof(*arr_of_numbers); ++i)
    {
        iter[i] = BSTInsert(new_bst, &arr_of_numbers[i]);
    }

    if (!BSTIsSameIter(BSTFind(new_bst, &to_find), iter[2]))
    {
        ++error_counter;
        FAIL("Find didn't work");
    }

    BSTRemove(iter[2]);

    if (!BSTIsSameIter(BSTFind(new_bst, &to_find), BSTEnd(new_bst)))
    {
        ++error_counter;
        FAIL("Find didn't work on not find");
    }


    BSTForEach(&param_to_function, BSTBegin(new_bst), iter[6], AddParamToData);

    if (*(int *)BSTGetData(iter[5]) != 16)
    {
        ++error_counter;
        FAIL("ForEach Failed");
    }

    if (*(int *)BSTGetData(iter[6]) != 29)
    {
        ++error_counter;
        FAIL("ForEach Boundary Failed");
    }

    BSTForEach(&param_to_function, BSTBegin(new_bst), BSTEnd(new_bst), AddParamToData);

    if (*(int *)BSTGetData(iter[5]) != 17)
    {
        ++error_counter;
        FAIL("ForEach Failed");
    }

    if (*(int *)BSTGetData(iter[6]) != 30)
    {
        ++error_counter;
        FAIL("ForEach Boundary Failed");
    }

    if (*(int *)BSTGetData(iter[1]) != 43)
    {
        ++error_counter;
        FAIL("ForEach Boundary Failed");
    }

    BSTDestroy(new_bst);

    if (0 == error_counter)
    {
        PASS("TestFlowForEachFind");
    }
}

int main()
{
    TestFlow1();
    TestFlowForEachFind();
    return 0;
}
