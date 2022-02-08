#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "scheduler.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))


struct two_ints
{
	int a;
	int b;
};

struct scheduid
{
	scheduler_t *sched;
	ilrd_uid_t uid;
};

struct two_ints numbers = {42, 77};

static int Stop(void *params)
{
	SchedStop(params);
	printf("%s\n", "STOP!!!!");

	return STOP;
}

static int AddNumbers(void *params)
{
	static size_t count;
	struct two_ints *new_params = (struct two_ints *)params;
	printf("%d + %d = %d\n", new_params->a, new_params->b, 
          (new_params->a + new_params->b));
	++new_params->a;
	++new_params->b;
	++count;

	if (8 == count)
	{
		return STOP;
	}

	return REPEAT;
}

static int Collatz(void *params)
{
	int *number = (int *)params;

	printf("%d\n", *number);

	if (!(*number % 2))
	{
		*number /= 2;
	}
	else
	{
		*number = 3 * (*number) + 1;
	}

	if (*number == 1)
	{
		printf("%d\n", *number);
		return STOP;
	}

	return REPEAT;
}

static int AddTwoTasks(void *params)
{
	SchedAddTask(params, AddNumbers, &numbers, 4, time(NULL));
	SchedAddTask(params, AddNumbers, &numbers, 2, time(NULL) + 3);

	return REPEAT;
}

static int TaskClear(void *params)
{
	SchedClear(params);

	return REPEAT;
}

static int RemoveMyself(void *params)
{
	struct scheduid *scheduid = (struct scheduid *)params;

	printf("%s\n", "i want to break free");
	SchedRemoveTask(scheduid->sched, scheduid->uid);

	return REPEAT;
}


static void RegularFunctionalities()
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

static void StopBeforeEnd()
{
	size_t error_counter = 0;
	scheduler_t *new_scheduler = SchedCreate();
	int number = 27;

	SchedAddTask(new_scheduler, Collatz, &number, 1, time(NULL) + 1);
	SchedAddTask(new_scheduler, Stop, new_scheduler, 1, time(NULL) + 10);
	SchedRun(new_scheduler);

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("StopBeforeEnd(). functions tests:\n Run Stop");
	}
}

static void ClearAllTask()
{
	size_t error_counter = 0;
	scheduler_t *new_scheduler = SchedCreate();
	struct two_ints numbers = {42, 77};
	int number = 27;

	SchedAddTask(new_scheduler, Collatz, &number, 2, time(NULL));
	SchedAddTask(new_scheduler, AddNumbers, &numbers, 2, time(NULL) + 1);
	SchedAddTask(new_scheduler, TaskClear, new_scheduler, 1, time(NULL) + 12);
	SchedRun(new_scheduler);

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("ClearAllTask. functions tests:\n Run Stop");
	}
}

static void TestAddTwoTasks()
{
	size_t error_counter = 0;
	scheduler_t *new_scheduler = SchedCreate();

	SchedAddTask(new_scheduler, AddTwoTasks, new_scheduler, 4, time(NULL));
	SchedAddTask(new_scheduler, Stop, new_scheduler, 1, time(NULL) + 15);
	SchedRun(new_scheduler);

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("TestAddTwoTasks. functions tests:\n Run Stop");
	}
}

static void TestRemoveMyself()
{
	size_t error_counter = 0;
	int number = 27;
	scheduler_t *new_scheduler = SchedCreate();
	ilrd_uid_t uid = BAD_UID;
	struct scheduid my_scheduid;
	my_scheduid.sched = new_scheduler;
	my_scheduid.uid = BAD_UID;

	SchedAddTask(new_scheduler, Collatz, &number, 3, time(NULL));
	uid = SchedAddTask(new_scheduler, RemoveMyself, &my_scheduid, 1, time(NULL) + 1);
	SchedAddTask(new_scheduler, Stop, new_scheduler, 1, time(NULL) + 15);
	my_scheduid.uid = uid;
	SchedRun(new_scheduler);

	SchedDestroy(new_scheduler);

	if (0 == error_counter)
	{
		PASS("TestRemoveMyself. functions tests:\n Run Stop");
	}
}

int main()
{
	RegularFunctionalities();
	StopBeforeEnd();
	ClearAllTask();
	TestRemoveMyself();
	TestAddTwoTasks();

    return 0;
}
