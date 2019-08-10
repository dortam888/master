#include "d_vector.h"
#include <stdio.h>

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n", #function)

static void TestEmptyVector()
{
	size_t size_of_element = 4;
	size_t start_capacity = 1;
	d_vector_t *vector = DVectorCreate(size_of_element, start_capacity);
	size_t vector_size = 0;
	int flag = 0;

	if (DVectorCapacity(vector) != start_capacity)
	{
		FAIL(Capacity);
		++flag;
	}
	vector_size = DVectorSize(vector);

	/* check size is 0 */
	if (vector_size != 0)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the vector is empty before any action */
	if (!DVectorIsEmpty(vector))
	{
		FAIL(IsEmpty);
		++flag;
	}

	DVectorPopBack(vector);

	/* check that it is still empty if we pop*/
	if (!DVectorIsEmpty(vector) && flag == 0)
	{
		FAIL(VectorPop);
		++flag;
	}

	if (DVectorGetItemAddress(vector, 0) != NULL)
	{
		FAIL(GetItem);
		++flag;
	}
	
	DVectorDestroy(vector);

	if (0 == flag)
	{
		PASS(EmptyVectorActions);
	}
}


static void TestVector()
{
	size_t size_of_element = 4;
	size_t start_capacity = 1;
	d_vector_t *vector = DVectorCreate(size_of_element, start_capacity);
	size_t vector_size = 0;
	int flag = 0;
	int a = 77;

	/* check that capacity is start capacity */
	if (DVectorCapacity(vector) != start_capacity)
	{
		FAIL(Capacity);
		++flag;
	}
	
	DVectorPushBack(vector, &a);

	/* check that vector doesn't grow */
	if (DVectorCapacity(vector) != start_capacity)
	{
		FAIL(CapacityAfterPush);
		++flag;
	}
	
	vector_size = DVectorSize(vector);

	/* check size is 1 aftr we push */
	if (vector_size != 1)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the vector is not empty after push */
	if (DVectorIsEmpty(vector))
	{
		FAIL(IsEmpty);
		++flag;
	}

	/* check that the element in the vector is the right one */
	if (*(int *)DVectorGetItemAddress(vector, 0) != a)
	{
		FAIL(GetItem);
		++flag;
	}

	/* check that get items works with invalid indexes */
	if (DVectorGetItemAddress(vector, 2) != NULL && 
		DVectorGetItemAddress(vector, 1) != NULL)
	{
		FAIL(GetItemInvalidIndex);
		++flag;
	}

	DVectorPopBack(vector);

	/* check that it is empty after we pop*/
	if (!DVectorIsEmpty(vector))
	{
		FAIL(VectorPop);
		++flag;
	}

	DVectorDestroy(vector);

	if (0 == flag)
	{
		PASS(VectorActions);
	}
}


static void TestPushVector()
{
	size_t size_of_element = 4;
	size_t start_capacity = 1;
	d_vector_t *vector = DVectorCreate(size_of_element, start_capacity);
	int flag = 0;
	int a = 77;
	char b[4] = "abc";
	short c[2] = {1, 2};

	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &b);

	/* check that capacity doubled the size */
	if (DVectorCapacity(vector) != start_capacity * 2)
	{
		FAIL(CapacityAfterResize1);
		++flag;
	}

	/* check size is 2 aftr we push 2 items*/
	if (DVectorSize(vector) != 2)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the vector is not empty after push */
	if (DVectorIsEmpty(vector))
	{
		FAIL(IsEmpty);
		++flag;
	}

	/* check that the element in the vector is the right one */
	if (*(int *)DVectorGetItemAddress(vector, 0) != a)
	{
		FAIL(GetItem);
		++flag;
	}

	if (*(int *)DVectorGetItemAddress(vector, 1) != *(int *)b)
	{
		FAIL(GetItem2);
		++flag;
	}

	if (DVectorGetItemAddress(vector, 3) != NULL &&
		DVectorGetItemAddress(vector, 2) != NULL)
	{
		FAIL(GetItemInvalidIndex);
		++flag;
	}

	DVectorPushBack(vector, &c);

	/* check that capacity doubled the size again*/
	if (DVectorCapacity(vector) != start_capacity * 4)
	{
		FAIL(CapacityAfterResize2);
		++flag;
	}

	/* check size is 3 aftr we push 3 items*/
	if (DVectorSize(vector) != 3)
	{
		FAIL(SizeAfterPush2);
		++flag;
	}

	if (*(int *)DVectorGetItemAddress(vector, 2) != *(int *)c)
	{
		FAIL(GetItemInvalidIndex);
		++flag;
	}

	DVectorDestroy(vector);

	if (0 == flag)
	{
		PASS(VectorPushGrowActions);
	}
}


static void TestPopVector()
{
	size_t size_of_element = 4;
	size_t start_capacity = 16;
	d_vector_t *vector = DVectorCreate(size_of_element, start_capacity);
	int flag = 0;
	int a = 77;
	char b[4] = "abc";
	short c[2] = {1, 2};
	unsigned int d = 90;

	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &b);
	DVectorPushBack(vector, &c);
	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &d);

	/* check that capacity stays the same */
	if (DVectorCapacity(vector) != start_capacity)
	{
		FAIL(Capacity);
		++flag;
	}

	/* check size is 5 aftr we push 5 items*/
	if (DVectorSize(vector) != 5)
	{
		FAIL(Size);
		++flag;
	}

	/* check that the vector is not empty after push */
	if (DVectorIsEmpty(vector))
	{
		FAIL(IsEmpty);
		++flag;
	}

	/* check that the element in the vector is the right one */
	if (*(int *)DVectorGetItemAddress(vector, 4) != (int)d)
	{
		FAIL(GetItem);
		++flag;
	}

	DVectorPopBack(vector);

	/* check that capacity shrinked */
	if (DVectorCapacity(vector) != start_capacity / 4)
	{
		FAIL(CapacityAfterPop);
		++flag;
	}

	/* check size is 4 aftr the pop*/
	if (DVectorSize(vector) != 4)
	{
		FAIL(SizeAfterPop2);
		++flag;
	}

	if (DVectorGetItemAddress(vector, 4) != NULL)
	{
		FAIL(GetItemInvalidIndex);
		++flag;
	}

	DVectorDestroy(vector);

	if (0 == flag)
	{
		PASS(VectorPopShrinkActions);
	}
}


static void TestReserveVector()
{
	size_t size_of_element = 4;
	size_t start_capacity = 16;
	d_vector_t *vector = DVectorCreate(size_of_element, start_capacity);
	int flag = 0;
	int a = 77;

	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &a);

	/* check that capacity stays the same */
	if (DVectorCapacity(vector) != start_capacity)
	{
		FAIL(Capacity);
		++flag;
	}

	DVectorReserve(vector, 50);

	if (DVectorCapacity(vector) != 50)
	{
		FAIL(CapacityAfterReserve);
		++flag;
	}

	if (DVectorSize(vector) != 2)
	{
		FAIL(SizeAfterReserve);
		++flag;
	}

	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &a);
	DVectorPushBack(vector, &a);

	DVectorReserve(vector, 4);

	if (DVectorCapacity(vector) != 50)
	{
		FAIL(CapacityAfterInvalidReserve);
		++flag;
	}

	if (DVectorSize(vector) != 6)
	{
		FAIL(SizeAfterReserve);
		++flag;
	}

	DVectorDestroy(vector);

	if (0 == flag)
	{
		PASS(VectorReserveActions);
	}
}


int main()
{
	TestEmptyVector();
	TestVector();
	TestPushVector();
	TestPopVector();
	TestReserveVector();

	return 0;
}
