#include <stdio.h>
#include <string.h>

#include "sorted_list.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static int is_before_int(const void *num1, const void *num2, 
					 void *param)
{
	(void)param;
	return (*(int *)num1 < *(int *)num2);
}

static int is_before_string_len(const void *str1, const void *str2, 
								void *param)
{
	(void)param;
	return (strlen((char *)str1) > strlen((char *)str2));
}

static void Flow1()
{
	sorted_list_t *new_list = SortedListCreate(is_before_int, NULL);
	int a = 77;
	sorted_list_iter_t i = SortedListBegin(new_list);
	size_t error_counter = 0;

	if (!SortedListIsEmpty(new_list))
	{
		FAIL("list should be empty at beginning");
		++error_counter;
	}

	if (0 != SortedListSize(new_list))
	{
		FAIL("list size should be 0");
		++error_counter;
	}

	i = SortedListInsert(new_list, &a);

	if (SortedListIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (1 != SortedListSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	if (*(int *)(SortedListGetData(i)) != a)
	{
		FAIL("value should be a");
		++error_counter;
	}

	SortedListRemove(i);

	if (!SortedListIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (0 != SortedListSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	i = SortedListInsert(new_list, &a);
	i = SortedListInsert(new_list, &a);
	i = SortedListInsert(new_list, &a);

	if (3 != SortedListSize(new_list))
	{
		FAIL("list size should be 3");
		++error_counter;
	}

	SortedListDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow1. functions: Create, Destroy, Insert, Remove, IsEmpty, Size");
	}
}

static void Flow2()
{
	sorted_list_t *new_list = SortedListCreate(is_before_int, NULL);
	int a = 77;
	int b = 42;
	int c = 55;
	size_t error_counter = 0;

	SortedListInsert(new_list, &a);
	SortedListInsert(new_list, &b);
	SortedListInsert(new_list, &c);
	if (SortedListIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (3 != SortedListSize(new_list))
	{
		FAIL("list size should be 3");
		++error_counter;
	}

	SortedListPopBack(new_list);
	if (SortedListGetData(SortedListBegin(new_list)) != &a)
	{
		FAIL("Pop Back or insert (sorted) is not functionate right");
		++error_counter;
	}

	if (2 != SortedListSize(new_list))
	{
		FAIL("list size should be 2");
		++error_counter;
	}

	SortedListPopFront(new_list);
	if (SortedListGetData(SortedListBegin(new_list)) != &c)
	{
		FAIL("Pop Front or insert are not functionate right");
		++error_counter;
	}

	if (1 != SortedListSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	SortedListDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow2. functions: PopFront, PopBack");
	}
}

static void Flow3()
{
	sorted_list_t *new_list1 = SortedListCreate(is_before_int, NULL);
	sorted_list_t *new_list2 = SortedListCreate(is_before_int, NULL);
	int a = 77;
	int b = 42;
	int c = 55;
	int d = 10;
	int e = 22;
	int f = 100;
	size_t error_counter = 0;

	SortedListInsert(new_list1, &a);
	SortedListInsert(new_list1, &b);
	SortedListInsert(new_list1, &c);
	SortedListInsert(new_list2, &d);
	SortedListInsert(new_list2, &f);
	SortedListInsert(new_list2, &e);

	SortedListMerge(new_list1, new_list2);
	if (6 != SortedListSize(new_list1))
	{
		FAIL("Merge add didn't work");
		++error_counter;
	}

	if (0 != SortedListSize(new_list2))
	{
		FAIL("Merge delete didn't work");
		++error_counter;
	}

	SortedListDestroy(new_list1);
	SortedListDestroy(new_list2);

	if (0 == error_counter)
	{
		PASS("Flow2. functions: Merge");
	}
}

static void Flow4()
{
	sorted_list_t *new_list = SortedListCreate(is_before_int, NULL);
	int a = 77;
	int b = 42;
	int c = 6;
	int in_list = 6;
	int not_in_list = 12;
	sorted_list_iter_t i = {NULL};
	size_t error_counter = 0;

	SortedListInsert(new_list, &a);
	SortedListInsert(new_list, &b);
	i = SortedListInsert(new_list, &c);

	if (*(int *)SortedListGetData(SortedListFind(new_list, 
		SortedListBegin(new_list), SortedListEnd(new_list), &in_list)) != c)
	{
		FAIL("Find function didn't find data which is on list");
		++error_counter;
	}

	if (SortedListEnd(new_list).dlist_iter != 
		SortedListFind(new_list, SortedListBegin(new_list), i, &in_list).dlist_iter)
	{
		FAIL("Find function find data which is not in range");
		++error_counter;
	}

	if (SortedListEnd(new_list).dlist_iter != SortedListFind(new_list, 
		SortedListBegin(new_list), SortedListEnd(new_list), &not_in_list).dlist_iter)
	{
		FAIL("Find function find data which is not on list");
		++error_counter;
	}

	SortedListDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow4. functions: Find");
	}
}

int main()
{
	Flow1();
	Flow2();
	Flow3();
	Flow4();

    return 0;
}
