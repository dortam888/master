#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include "sorting.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define TIME_TEST_TIMES 1000

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

static int IsBefore(int num1, int num2)
{
    return (num1 < num2);
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

static void TestCountSort(int(*sort_function)(int *arr, size_t size, int min, int max))
{
    int arr01[1] = {0};
    int arr02[1] = {0};
    /*int arr11[8] = {-6,-10,-15,-20,-8,-1,-11,-2};
    int arr12[8] = {-6,-10,-15,-20,-8,-1,-11,-2};*/
    int arr11[8] = {1,4,2,9,5,3,2,1};
    int arr12[8] = {1,4,2,9,5,3,2,1};
    int arr21[8] = {6,10,77,42,8,1,100,2};
    int arr22[8] = {6,10,77,42,8,1,100,2};
    int arr31[8] = {-6,10,-10,-20,-8,1,-20,0};
    int arr32[8] = {-6,10,-10,-20,-8,1,-20,0};
    int arr41[8] = {-1,0,1,3,6,3,1,0};
    int arr42[8] = {-1,0,1,3,6,3,1,0};
    static size_t sort_index;
    /*size_t i = 0;*/
    char *sort_func[] = {"Counting Sort"};
    size_t error_counter = 0;

    sort_function(arr01, 1, 0, 0);
    qsort(arr02, 1, sizeof(int), CmpFunc);

    if (!CmpArr(arr01, arr02, 1))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr11, 8, 1, 9);
    qsort(arr12, 8, sizeof(int), CmpFunc);
    
    
    /*for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr11[i]);
    }
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr12[i]);
    }*/
    
    if (!CmpArr(arr11, arr12, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr21, 8, 1, 100);
    qsort(arr22, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr21, arr22, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr31, 8, -20, 10);
    qsort(arr32, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr31, arr32, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr41, 8, -1, 6);
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

static void TestRadixSort(int(*sort_function)(int *arr, size_t size, unsigned int num_of_bits))
{
    int arr01[1] = {0};
    int arr02[1] = {0};
    /*int arr11[8] = {-6,-10,-15,-20,-8,-1,-11,-2};
    int arr12[8] = {-6,-10,-15,-20,-8,-1,-11,-2};*/
    int arr11[8] = {1,4,2,9,5,3,2,1};
    int arr12[8] = {1,4,2,9,5,3,2,1};
    int arr21[8] = {6,10,77,42,8,1,100,2};
    int arr22[8] = {6,10,77,42,8,1,100,2};
    int arr31[8] = {6,10,10,20,15,15,20,0};
    int arr32[8] = {6,10,10,20,15,15,20,0};
    int arr41[8] = {1,0,1,3,INT_MAX,3,1,0};
    int arr42[8] = {1,0,1,3,INT_MAX,3,1,0};
    static size_t sort_index;
    /*size_t i = 0;*/
    char *sort_func[] = {"Radix Sort"};
    size_t error_counter = 0;


    sort_function(arr01, 1, 4);
    qsort(arr02, 1, sizeof(int), CmpFunc);


    if (!CmpArr(arr01, arr02, 1))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr11, 8, 4);
    qsort(arr12, 8, sizeof(int), CmpFunc);
      
/*
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr11[i]);
    }
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr12[i]);
    }
*/
    if (!CmpArr(arr11, arr12, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr21, 8, 4);
    qsort(arr22, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr21, arr22, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr31, 8, 4);
    qsort(arr32, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr31, arr32, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr41, 8, 8);
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

static void TestQSort(void(*sort_function)(void *, size_t, size_t,
                      int(cmp_func)(const void *, const void *)))
{
    int arr01[1] = {0};
    int arr02[1] = {0};
    /*int arr11[8] = {-6,-10,-15,-20,-8,-1,-11,-2};
    int arr12[8] = {-6,-10,-15,-20,-8,-1,-11,-2};*/
    int arr11[8] = {1,4,2,9,5,3,2,1};
    int arr12[8] = {1,4,2,9,5,3,2,1};
    int arr21[8] = {6,10,77,42,8,1,100,2};
    int arr22[8] = {6,10,77,42,8,1,100,2};
    int arr31[8] = {6,10,10,20,15,15,20,0};
    int arr32[8] = {6,10,10,20,15,15,20,0};
    int arr41[8] = {1,0,1,3,INT_MAX,3,1,0};
    int arr42[8] = {1,0,1,3,INT_MAX,3,1,0};
    int arr51[6] = {2, 1, 3, 6, 5, 4};
    int arr52[6] = {2, 1, 3, 6, 5, 4};
    static size_t sort_index;
    /*size_t i = 0;*/
    char *sort_func[] = {"Quick Sort"};
    size_t error_counter = 0;


    sort_function(arr02, 1, sizeof(int), CmpFunc);
    qsort(arr02, 1, sizeof(int), CmpFunc);


    if (!CmpArr(arr01, arr02, 1))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr11, 8, sizeof(int), CmpFunc);
    qsort(arr12, 8, sizeof(int), CmpFunc);
      
/*
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr11[i]);
    }
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr12[i]);
    }
*/
    if (!CmpArr(arr11, arr12, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr21, 8, sizeof(int), CmpFunc);
    qsort(arr22, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr21, arr22, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr31, 8, sizeof(int), CmpFunc);
    qsort(arr32, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr31, arr32, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr41, 8, sizeof(int), CmpFunc);
    qsort(arr42, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr41, arr42, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }
    
    sort_function(arr51, 6, sizeof(int), CmpFunc);
    qsort(arr52, 6, sizeof(int), CmpFunc);

    if (!CmpArr(arr51, arr52, 6))
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

static void TestMergeSort(int(*sort_function)(int *arr, size_t size, int(is_before)(int num1, int num2)))
{
    int arr01[1] = {0};
    int arr02[1] = {0};
    /*int arr11[8] = {-6,-10,-15,-20,-8,-1,-11,-2};
    int arr12[8] = {-6,-10,-15,-20,-8,-1,-11,-2};*/
    int arr11[8] = {1,4,2,9,5,3,2,1};
    int arr12[8] = {1,4,2,9,5,3,2,1};
    int arr21[8] = {6,10,77,42,8,1,100,2};
    int arr22[8] = {6,10,77,42,8,1,100,2};
    int arr31[8] = {6,10,10,20,15,15,20,0};
    int arr32[8] = {6,10,10,20,15,15,20,0};
    int arr41[8] = {1,0,1,3,INT_MAX,3,1,0};
    int arr42[8] = {1,0,1,3,INT_MAX,3,1,0};
    static size_t sort_index;
    /*size_t i = 0;*/
    char *sort_func[] = {"Merge Sort"};
    size_t error_counter = 0;


    sort_function(arr01, 1, IsBefore);
    qsort(arr02, 1, sizeof(int), CmpFunc);


    if (!CmpArr(arr01, arr02, 1))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr11, 8, IsBefore);
    qsort(arr12, 8, sizeof(int), CmpFunc);
      
/*
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr11[i]);
    }
    for (i = 0; i < 8; ++i)
    {
        printf("%d ", arr12[i]);
    }
*/
    if (!CmpArr(arr11, arr12, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr21, 8, IsBefore);
    qsort(arr22, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr21, arr22, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr31, 8, IsBefore);
    qsort(arr32, 8, sizeof(int), CmpFunc);

    if (!CmpArr(arr31, arr32, 8))
    {
        FAIL(sort_func[sort_index]);
        ++error_counter;
    }

    sort_function(arr41, 8, IsBefore);
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

static void TimeTest()
{
	clock_t start = 0; 
	clock_t end = 0;
	double total[4] = {0};
	size_t i = 0;
	int arr[1000] = {0};
	
	for (i = 0; i < 1000; ++i)
	{
		arr[i] = rand() % 1000;
	}

    for (i = 0; i < TIME_TEST_TIMES; ++i)
    {
    	start = clock();

	    Counting(arr, 1000, 0, 1000);

	    end = clock();

	    total[0] += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    for (i = 0; i < TIME_TEST_TIMES; ++i)
    {
    	start = clock();

	    Radix(arr, 1000, 8);

	    end = clock();

	    total[1] += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    for (i = 0; i < TIME_TEST_TIMES; ++i)
    {
        start = clock();

	    Merge(arr, 1000, IsBefore);

	    end = clock();

	    total[2] += (double)(end - start) / CLOCKS_PER_SEC;
    }
    
    for (i = 0; i < TIME_TEST_TIMES; ++i)
    {
        start = clock();

	    QSort(arr, 1000, sizeof(int), CmpFunc);

	    end = clock();

	    total[3] += (double)(end - start) / CLOCKS_PER_SEC;
    }


	printf("Counting Sort Took %f CPU clocks\n", total[0]/TIME_TEST_TIMES);
	printf("Radix Sort Took %f CPU clocks\n", total[1]/TIME_TEST_TIMES);
	printf("Merge Sort Took %f CPU clocks\n", total[2]/TIME_TEST_TIMES);
	printf("Quick Sort Took %f CPU clocks\n", total[3]/TIME_TEST_TIMES);
}

int main()
{
    TestSort(Insertion);
    TestSort(Selection);
    TestSort(Bubble);
    TestCountSort(Counting);
    TestRadixSort(Radix);
    TestMergeSort(Merge);
    TestQSort(QSort);
    TimeTest();

    return 0;
}
