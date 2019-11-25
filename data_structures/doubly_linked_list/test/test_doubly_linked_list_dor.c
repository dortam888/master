#include <stdio.h>
#include <stdlib.h>

#include "doubly_linked_list.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void Flow1()
{
	dlist_t *new_list = DlistCreate();
	int a = 77;
	dlist_iter_t i = DlistBegin(new_list);
	size_t error_counter = 0;

	if (!DlistIsEmpty(new_list))
	{
		FAIL("list should be empty at beginning");
		++error_counter;
	}

	if (0 != DlistSize(new_list))
	{
		FAIL("list size should be 0");
		++error_counter;
	}

	i = DlistInsert(new_list, i, &a);

	if (DlistIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (1 != DlistSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	DlistRemove(i);

	if (!DlistIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (0 != DlistSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	i = DlistInsert(new_list, DlistBegin(new_list), &a);
	i = DlistInsert(new_list, i, &a);
	i = DlistInsert(new_list, i, &a);

	if (3 != DlistSize(new_list))
	{
		FAIL("list size should be 3");
		++error_counter;
	}

	DlistDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow1. functions: Create, Destroy, Insert, Remove, IsEmpty, Size");
	}
}

static void Flow2()
{
	dlist_t *new_list = DlistCreate();
	int a = 77;
	size_t b = 42;
	size_t error_counter = 0;

	DlistPushFront(new_list, &a);
	DlistPushBack(new_list, &b);
	if (DlistIsEmpty(new_list))
	{
		FAIL("list shouldn't be empty after insertion");
		++error_counter;
	}

	if (2 != DlistSize(new_list))
	{
		FAIL("list size should be 2");
		++error_counter;
	}

	DlistPopBack(new_list);
	if (DlistGetData(DlistBegin(new_list)) != &a)
	{
		FAIL("Pop Back or Push Front are not functionate right");
		++error_counter;
	}

	if (1 != DlistSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	DlistPushBack(new_list, &b);

	DlistPopFront(new_list);
	if (DlistGetData(DlistBegin(new_list)) != &b)
	{
		FAIL("Pop Front or Push Back are not functionate right");
		++error_counter;
	}

	if (1 != DlistSize(new_list))
	{
		FAIL("list size should be 1");
		++error_counter;
	}

	DlistDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow2. functions: PushBack, PushFront, PopFront, PopBack");
	}
}

static int Compare(const void *element_data, const void *user_data)
{
	return(!(*(int *)element_data == *(int *)user_data));
}

static void Flow3()
{
	dlist_t *new_list = DlistCreate();
	int a = 77;
	size_t b = 42;
	char *c = "dor tokea node";
	int not_in_list = 6;
	dlist_iter_t i = NULL;
	size_t error_counter = 0;

	DlistPushFront(new_list, &a);
	DlistPushBack(new_list, &b);
	i = DlistPushBack(new_list, c);

	if (i != DlistFind(new_list, DlistBegin(new_list), DlistEnd(new_list), Compare, c))
	{
		FAIL("Find function didn't find data which is on list");
		++error_counter;
	}

	if (DlistEnd(new_list) != DlistFind(new_list, DlistBegin(new_list), i, Compare, c))
	{
		FAIL("Find function find data which is not in range");
		++error_counter;
	}

	if (DlistEnd(new_list) != DlistFind(new_list, DlistBegin(new_list), DlistEnd(new_list), Compare, &not_in_list))
	{
		FAIL("Find function find data which is not on list");
		++error_counter;
	}

	DlistDestroy(new_list);

	if (0 == error_counter)
	{
		PASS("Flow3. functions: Find");
	}
}

static void Flow4()
{
	dlist_t *new_list1 = DlistCreate();
	dlist_t *new_list2 = DlistCreate();
	int a = 77;
	size_t b = 42;
	char *c = "dor tokea node";
	int d = 6;
	dlist_iter_t i_dest = NULL;
	size_t error_counter = 0;

	DlistPushFront(new_list1, &a);
	i_dest = DlistPushBack(new_list1, &b);

	DlistPushFront(new_list2, c);
	DlistPushBack(new_list2, &d);

	DlistSplice(i_dest, DlistBegin(new_list2), DlistEnd(new_list2));

	if (4 != DlistSize(new_list1))
	{
		FAIL("Splice add didn't work");
		++error_counter;
	}

	if (0 != DlistSize(new_list2))
	{
		FAIL("Splice delete didn't work");
		++error_counter;
	}

	DlistDestroy(new_list1);
	DlistDestroy(new_list2);

	if (0 == error_counter)
	{
		PASS("Flow4. functions: Splice");
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
