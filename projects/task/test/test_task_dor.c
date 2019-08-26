#include <stdio.h>

#include <time.h>
#include "task.h"


#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

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

static void Flow1()
{
	struct two_ints numbers = {42, 77};
	task_t *new_task = TaskCreate(AddNumbers, &numbers, 5, time(NULL) + 30);
	int status = 0;
	size_t error_counter = 0;

	if (time(NULL) + 30 != TaskGetTimeToExecution(new_task))
	{
		FAIL("execution time is not correct");
		++error_counter;
	}

	status = TaskExecute(new_task);
	if (REPEAT != status)
	{
		FAIL("status fail");
		++error_counter;
	}

	status = TaskExecute(new_task);
	if (REPEAT != status)
	{
		FAIL("status fail");
		++error_counter;
	}

	status = TaskExecute(new_task);
	if (STOP != status)
	{
		FAIL("status fail");
		++error_counter;
	}

	if (UIDIsSame(BAD_UID,TaskGetUID(new_task)))
	{
		FAIL("Should Be Valid UID");
		++error_counter;
	}

	TaskUpdateTimeToExecute(new_task);

	if (time(NULL) + 35 != TaskGetTimeToExecution(new_task))
	{
		FAIL("execution time is not correct");
		++error_counter;
	}

	TaskDestroy(new_task);

	if (0 == error_counter)
	{
		PASS("task operations are ok");
	}
}

int main()
{
	Flow1();
    return 0;
}
