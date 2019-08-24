#include <stdio.h>
#include <string.h>

#include "priority_queue.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))
typedef struct vip_members
{
	char *name;
	size_t priority;
} vip_members_t;

static int VipCmpFunc(const void *data1, const void *data2, void *param)
{
	vip_members_t *member1 = (vip_members_t *)data1;
	vip_members_t *member2 = (vip_members_t *)data2;
	UNUSED(param);

	if (member1->priority < member2->priority)
	{
		return 1;
	}
	else if (member1->priority > member2->priority)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

static void Flow1()
{
	pq_t *pq = PriorityQCreate(VipCmpFunc, NULL);
	vip_members_t dor = {"dor", 77};
	vip_members_t gilad = {"gilad", 42};
	vip_members_t deddi = {"deddi", 63};
	size_t error_counter = 0;

	/* check that queue is empty before enqueue */
	if (!PriorityQIsEmpty(pq))
	{
		FAIL("queue should be empty");
		++error_counter;
	}
	/* check that queue size is 0 before enqueue */
	if (0 != PriorityQSize(pq))
	{
		FAIL("Empty queue size is 0");
		++error_counter;
	}

	PriorityQEnqueue(pq, &deddi);
	/* check that peek gives right adress of enqueuing data */
	if (&deddi != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works with one element");
		++error_counter;
	}

	if (PriorityQIsEmpty(pq))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (1 != PriorityQSize(pq))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	PriorityQEnqueue(pq, &dor);

	if (&dor != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works with two element");
		++error_counter;
	}

	if (PriorityQIsEmpty(pq))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (2 != PriorityQSize(pq))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	PriorityQEnqueue(pq, &gilad);
	if (&dor != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works with three element on sort");
		++error_counter;
	}

	PriorityQDequeue(pq);

	if (&deddi != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works after dequeue");
		++error_counter;
	}

	if (PriorityQIsEmpty(pq))
	{
		FAIL("queue should not be empty after dequeue");
		++error_counter;
	}

	if (2 != PriorityQSize(pq))
	{
		FAIL("queue size should be 2 after dequeue");
		++error_counter;
	}
	
	PriorityQDequeue(pq);
	
	if (&gilad != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works with one element after dequeue");
		++error_counter;
	}

	if (PriorityQIsEmpty(pq))
	{
		FAIL("queue should not be empty after dequeue");
		++error_counter;
	}

	if (1 != PriorityQSize(pq))
	{
		FAIL("queue size should be 1 after dequeue");
		++error_counter;
	}

	PriorityQDestroy(pq);

	if (0 == error_counter)
	{
		PASS("No errors in Flow1:\nfunctions tested: Create, Destroy, Enqueue, Dequeue, Peek, Size, IsEmpty\n");
	}
}

static void TestClear()
{
	pq_t *pq = PriorityQCreate(VipCmpFunc, NULL);
	vip_members_t dor = {"dor", 77};
	vip_members_t gilad = {"gilad", 42};
	vip_members_t deddi = {"deddi", 63};
	size_t error_counter = 0;

	PriorityQEnqueue(pq, &gilad);
	PriorityQEnqueue(pq, &dor);
	PriorityQEnqueue(pq, &deddi);
	
	if (3 != PriorityQSize(pq))
	{
		FAIL("queue size should be 3 after enqueue");
		++error_counter;
	}

	PriorityQClear(pq);

	if (!PriorityQIsEmpty(pq))
	{
		FAIL("queue should be empty after clear");
		++error_counter;
	}
	
	PriorityQEnqueue(pq, &deddi);
	
	if (&deddi != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't works after clear and enqueue");
		++error_counter;
	}

	if (PriorityQIsEmpty(pq))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (1 != PriorityQSize(pq))
	{
		FAIL("queue size should be 1 after clear and enqueue");
		++error_counter;
	}

	PriorityQDestroy(pq);

	if (0 == error_counter)
	{
		PASS("No errors in TestClear:\nfunctions tested: Clear\n");
	}
}

static int NamesCompare(const void *data1, const void *data2)
{
	vip_members_t *vip_member = (vip_members_t *)data2;

	return (strcmp(vip_member->name, data1));
}

static void TestErase()
{
	pq_t *pq = PriorityQCreate(VipCmpFunc, NULL);
	vip_members_t dor = {"dor", 77};
	vip_members_t gilad = {"gilad", 42};
	vip_members_t deddi = {"deddi", 63};
	char *member_to_erase = "dor";
	char *member_to_erase2 = "gilad";
	size_t error_counter = 0;

	PriorityQEnqueue(pq, &gilad);
	PriorityQEnqueue(pq, &dor);
	PriorityQEnqueue(pq, &deddi);
	
	if (&dor != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't work on sort");
		++error_counter;
	}

	PriorityQErase(pq, member_to_erase, NamesCompare);

	if (&deddi != PriorityQPeek(pq))
	{
		FAIL("Peek doesn't work on sort");
		++error_counter;
	}
	
	if (2 != PriorityQSize(pq))
	{
		FAIL("queue size should be 2 after erase");
		++error_counter;
	}

	PriorityQErase(pq, member_to_erase2, NamesCompare);

	if (1 != PriorityQSize(pq))
	{
		FAIL("queue size should be 1 after erasing");
		++error_counter;
	}

	PriorityQDestroy(pq);

	if (0 == error_counter)
	{
		PASS("No errors in TestErase:\nfunctions tested: Erase\n");
	}
}

int main()
{
	Flow1();
	TestClear();
	TestErase();
    return 0;
}
