#include <stdio.h>
#include "heap.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

int IntCompareFunction(const void *data1, const void *data2, void *param)
{
    UNUSED(param);
    return *(int *)data1 - *(int *)data2;
}

static void TestHeap()
{
    int arr[8] = {22, 50, 10, 6, 80, 41, 59, 1};
    size_t error_counter = 0;
    heap_t *new_heap = HeapCreate(IntCompareFunction, NULL);
    if (NULL == new_heap)
    {
        printf("Failed to allocate");
        return;
    }

    if (!HeapIsEmpty(new_heap))
    {
        ++error_counter;
        FAIL("IsEmpty not work");
    }
    
    if (HeapSize(new_heap) != 0)
    {
        ++error_counter;
        FAIL("Size Failed In Empty");
    }
    
    HeapPush(new_heap, &arr[0]);

    if (*(int *)HeapPeek(new_heap) != arr[0])
    {
        ++error_counter;
        FAIL("HeapPeek Failed");
    }
    
    if (HeapSize(new_heap) != 1)
    {
        ++error_counter;
        FAIL("Size Failed");
    }
    
    HeapPush(new_heap, &arr[1]);
    
    if (*(int *)HeapPeek(new_heap) != arr[1])
    {
        ++error_counter;
        FAIL("HeapPeek Failed");
    }
    
    if (HeapSize(new_heap) != 2)
    {
        ++error_counter;
        FAIL("Size Failed");
    }
    
    HeapPush(new_heap, &arr[2]);
    
    if (*(int *)HeapPeek(new_heap) != arr[1])
    {
        ++error_counter;
        FAIL("HeapPeek Failed");
    }
    
    if (HeapSize(new_heap) != 3)
    {
        ++error_counter;
        FAIL("Size Failed");
    }

    HeapPush(new_heap, &arr[3]);
    HeapPush(new_heap, &arr[4]);
    HeapPush(new_heap, &arr[5]);
    HeapPush(new_heap, &arr[6]);
    HeapPush(new_heap, &arr[7]);

    if (*(int *)HeapPeek(new_heap) != arr[4])
    {
        ++error_counter;
        FAIL("HeapPeek Failed");
    }
    
    if (HeapSize(new_heap) != 8)
    {
        ++error_counter;
        FAIL("Size Failed");
    }
    
    

    if (0 == error_counter)
    {
        PASS("Heap");
    }
}

int main()
{
    TestHeap();
    return 0;
}
