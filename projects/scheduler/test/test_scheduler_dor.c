#include <stdio.h>
#include <time.h>

#include "scheduler.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

struct two_ints
{
	int a;
	int b;
};

int AddNumbers(void *params)
{
	static size_t count;
	struct two_ints *new_params = (struct two_ints *)params;
	printf("%d + %d = %d\n", new_params->a, new_params->b, 
          (new_params->a + new_params->b));
	++new_params->a;
	++new_params->b;
	++count;

	if (3 == count)
	{
		return STOP;
	}

	return REPEAT;
}

int Stop(void *params)
{
	SchedStop(params);

	return STOP;
}

static void Flow1()
{
	size_t error_counter = 0;
	scheduler_t *new_scheduler = SchedCreate();
	ilrd_uid_t uid = BAD_UID;

	if (!SchedIsEmpty(new_scheduler))
	{
		FAIL("scheduler should be empty at first (IsEmpty)");
		++error_counter;
	}

	if (0 != SchedSize(new_scheduler))
	{
		FAIL("scheduler should be empty at first (Size)");
		++error_counter;
	}

	SchedAddTask(new_scheduler, AddNumbers, NULL, 5, time(NULL) + 5);
	uid = SchedAddTask(new_scheduler, AddNumbers, NULL, 3, time(NULL) + 10);

	if (SchedIsEmpty(new_scheduler))
	{
		FAIL("scheduler shouldn't be empty after add tasks");
		++error_counter;
	}

	if (2 != SchedSize(new_scheduler))
	{
		FAIL("scheduler size should be 2");
		++error_counter;
	}

	SchedRemoveTask(new_scheduler, uid);

	if (SchedIsEmpty(new_scheduler))
	{
		FAIL("scheduler shouldn't be empty after first remove");
		++error_counter;
	}

	if (1 != SchedSize(new_scheduler))
	{
		FAIL("scheduler size should be 1");
		++error_counter;
	}

	SchedAddTask(new_scheduler, AddNumbers, NULL, 3, time(NULL) + 10);
	SchedAddTask(new_scheduler, AddNumbers, NULL, 3, time(NULL) + 10);

	SchedClear(new_scheduler);

	if (!SchedIsEmpty(new_scheduler))
	{
		FAIL("scheduler should be empty after clear (IsEmpty)");
		++error_counter;
	}

	if (0 != SchedSize(new_scheduler))
	{
		FAIL("scheduler should be empty after clear (Size)");
		++error_counter;
	}

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("Flow1. functions tests:\n IsEmpty Size AddTask RemoveTask Clear");
	}
}

static void Flow2()
{
	size_t error_counter = 0;
	scheduler_t *new_scheduler = SchedCreate();

	SchedAddTask(new_scheduler, AddNumbers, NULL, 5, time(NULL) + 1);
	SchedAddTask(new_scheduler, Stop, new_scheduler, 1, time(NULL) + 11);
	SchedRun(new_scheduler);

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("Flow2. functions tests:\n Run Stop");
	}
}

int main()
{
	Flow1();
	Flow2();
    return 0;
}
