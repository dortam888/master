#include <stdio.h>
#include <stdlib.h>

#include "avl.h" /*avl_t*/

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

static int IntCmpFunc(const void *num1, const void *num2, void *params)
{
    UNUSED(params);
    return (*(int *)num2 - *(int *)num1);
}

static int PrintTreePlusParam(void *iter_data, void *param)
{
    printf("%d\n", (*(int *)iter_data + *(int *)param));
    return 0;
}

static int DivideParamInData(void *iter_data, void *param)
{
    if (0 == *(int *)iter_data)
    {
        return 1;
    }

    *(int *)iter_data = *(int *)param/(*(int *)iter_data);

    return 0;
}

static void TestFlow1()
{
    int arr_of_numbers[] = {11, 42, 0, 50, 33, 12, 15, 29, 32, 9};
    size_t i = 0LU;
    size_t error_counter = 0LU;
    avl_t *new_avl = AVLCreate(NULL, IntCmpFunc);

    if (!AVLIsEmpty(new_avl))
    {
        ++error_counter;
        FAIL("IsEmpty didn't work");
    }
    
    if (AVLCount(new_avl) != 0)
    {
        ++error_counter;
        FAIL("Size didn't work");
    }

    for(i = 0LU; i < sizeof(arr_of_numbers)/sizeof(*arr_of_numbers); ++i)
    {
        AVLInsert(new_avl, &arr_of_numbers[i]);
    }

    if (AVLIsEmpty(new_avl))
    {
        ++error_counter;
        FAIL("IsEmpty didn't work");
    }

    if (10 != AVLCount(new_avl))
    {
        ++error_counter;
        printf("size was %lu\n", AVLCount(new_avl));
        FAIL("size didn't work");
    }

    AVLRemove(new_avl, arr_of_numbers);
    
    if (9 != AVLCount(new_avl))
    {
        ++error_counter;
        printf("size was %lu\n", AVLCount(new_avl));
        FAIL("size didn't work");
    }
    
    AVLRemove(new_avl, &arr_of_numbers[2]);
    
    if (8 != AVLCount(new_avl))
    {
        ++error_counter;
        printf("size was %lu\n", AVLCount(new_avl));
        FAIL("size didn't work");
    }
    
    AVLRemove(new_avl, &arr_of_numbers[9]);
    
    if (7 != AVLCount(new_avl))
    {
        ++error_counter;
        printf("size was %lu\n", AVLCount(new_avl));
        FAIL("size didn't work");
    }

    AVLDestroy(new_avl);

    if (0 == error_counter)
    {
        PASS("TestFlow1");
    }
}

static void TestFlowForEachFind()
{
    int arr_of_numbers[] = {11, 42, 0, 50, 33, 12, 15, 29, 32, 9};
    int to_find = 50;
    int param_to_divide = 42;
    int param_to_function = 1;
    size_t i = 0LU;
    size_t error_counter = 0LU;
    avl_t *new_avl = AVLCreate(NULL, IntCmpFunc);
    int foreach_stat = 0;

    for(i = 0LU; i < sizeof(arr_of_numbers)/sizeof(*arr_of_numbers); ++i)
    {
        AVLInsert(new_avl, &arr_of_numbers[i]);
    }

    if (*(int *)AVLFind(new_avl, &to_find) != 50)
    {
        ++error_counter;
        FAIL("Find didn't work");
    }

    AVLRemove(new_avl, &arr_of_numbers[3]);

    if (NULL != AVLFind(new_avl, &to_find))
    {
        ++error_counter;
        printf("%p\n", AVLFind(new_avl, &param_to_function));
        FAIL("Find didn't work on not find");
    }

    AVLForEach(&param_to_function, new_avl, PrintTreePlusParam);

    foreach_stat = AVLForEach(&param_to_divide, new_avl, DivideParamInData);
    if (1 != foreach_stat)
    {
        ++error_counter;
        printf("foreach status is %d\n", foreach_stat);
        FAIL("ForEach didn't work on fail");
    }

    AVLDestroy(new_avl);

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
