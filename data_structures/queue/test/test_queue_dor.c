#include <stdio.h> /*printf*/

#include "queue.h" /*queue_t*/

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m\n in line %d", str, __LINE__)

static void Flow1()
{
	queue_t *queue = QueueCreate();
	int a = 77;
	char *string = "dor tokea Q";
	size_t error_counter = 0;

	/* check that queue is empty before enqueue */
	if (!QueueIsEmpty(queue))
	{
		FAIL("queue should be empty");
		++error_counter;
	}
	/* check that queue size is 0 before enqueue */
	if (0 != QueueSize(queue))
	{
		FAIL("Empty queue size is 0");
		++error_counter;
	}

	QueueEnqueue(queue, &a);
	/* check that peek gives right adress of enqueuing data */
	if (&a != QueuePeek(queue))
	{
		FAIL("Peek doesn't works with one element");
		++error_counter;
	}

	if (QueueIsEmpty(queue))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (1 != QueueSize(queue))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	QueueEnqueue(queue, string);

	if (&a != QueuePeek(queue))
	{
		FAIL("Peek doesn't works with one element");
		++error_counter;
	}

	if (QueueIsEmpty(queue))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (2 != QueueSize(queue))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	QueueDequeue(queue);

	if (string != QueuePeek(queue))
	{
		FAIL("Peek doesn't works with one element");
		++error_counter;
	}

	if (QueueIsEmpty(queue))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (1 != QueueSize(queue))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	QueueDestroy(queue);

	if (0 == error_counter)
	{
		PASS("Flow1 Works. functions tested: all beside QueueAppend");
	}
}


static void TestAppend()
{
	queue_t *queue1 = QueueCreate();
	queue_t *queue2 = QueueCreate();
	int a = 77;
	char *string = "dor tokea Q";
	size_t kiki = 42;
	char why = 'd';
	size_t error_counter = 0;

	QueueEnqueue(queue1, &a);
	QueueEnqueue(queue1, string);

	QueueEnqueue(queue2, &kiki);
	QueueEnqueue(queue2, &why);
	QueueEnqueue(queue2, &a);

	if (2 != QueueSize(queue1))
	{
		FAIL("queue1 size should be 2");
		++error_counter;
	}

	if (3 != QueueSize(queue2))
	{
		FAIL("queue2 size should be 3");
		++error_counter;
	}

	QueueAppend(queue1, queue2);

	if (5 != QueueSize(queue1))
	{
		FAIL("queue1 size should be 5 after appending queue2");
		++error_counter;
	}

	if (0 != QueueSize(queue2))
	{
		FAIL("queue2 size should be 0 after appending it");
		++error_counter;
	}

	if (&a != QueuePeek(queue1))
	{
		FAIL("Peek doesn't works after appending");
		++error_counter;
	}

	QueueEnqueue(queue2, string);

	if (string != QueuePeek(queue2))
	{
		FAIL("Peek doesn't works after enqueue appended");
		++error_counter;
	}

	if (QueueIsEmpty(queue2))
	{
		FAIL("queue should not be empty after enqueue");
		++error_counter;
	}

	if (1 != QueueSize(queue2))
	{
		FAIL("queue size should be 1 after enqueue");
		++error_counter;
	}

	QueueDestroy(queue1);
	QueueDestroy(queue2);

	if (0 == error_counter)
	{
		PASS("QueueAppend works");
	}
}

int main()
{
	Flow1();
	TestAppend();
    return 0;
}
