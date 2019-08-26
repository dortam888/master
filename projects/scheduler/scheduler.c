#include <stdlib.h> /*malloc free*/
#include <assert.h> /*assert*/

#include "scheduler.h" /*scheduler_t*/
#include "priority_queue.h" /*pq_t*/
#include "task.h" /*task_t*/

#define UNUSED(x) ((void)(x))

struct scheduler
{
	pq_t pq;
	task_t *current_task;
}

static int TimeCmpFunc(const void *task1, const void *task2, void *param)
{
	task1 = (task_t *)task1;
	task2 = (task_t *)task2;
	UNUSED(param);

	if (TaskGetTimeToExecution(task2) > TaskGetTimeToExecution(task1))
	{
		return 1;
	}
	else if(TaskGetTimeToExecution(task2) < TaskGetTimeToExecution(task1))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

scheduler_t *SchedCreate(void)
{
	scheduler_t *new_scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_scheduler)
	{
		return NULL;
	}

	new_scheduler->pq = PriorityQCreate(TimeCmpFunc, NULL);
	if (NULL == scheduler->pq)
	{
		free(new_scheduler); new_scheduler = NULL;
		return NULL;
	}

	new_scheduler->current_task = NULL;

	return new_scheduler;
}

void SchedDestroy(scheduler_t *sched)
{
	assert(NULL != sched);

	sched->current_task = NULL;

	PriorityQDestroy(sched->pq);
	sched->pq = NULL;

	free(sched); sched = NULL;
}

ilrd_uid_t AddTask(scheduler_t *sched, action_func_t action_func, void *params,
				   time_t exe_interval_in_seconds, time_t execution_start_time)
{
	task_t *new_task = TaskCreate(action_func, params, exe_interval_in_seconds,
                                  execution_start_time);
	if (NULL == new_task)
	{
		return BAD_UID;
	}

	assert(NULL != sched);

	if (0 != PriorityQEnqueue(sched->pq, new_task))
	{
		return BAD_UID;
	}

	return TaskGetUID(new_task);
}

static TaskUidCompare(const void *data1, const void *data2)
{
	task_t *task1 = (task_t *)data1;
	ilrd_uid_t *uid2 = (ilrd_uid_t *)data2;

	return (UIDIsSame(TaskGetUID(task1), *uid2));
}


void RemoveTask(scheduler_t *sched, ilrd_uid_t task_uid)
{
	assert(NULL != sched);

	PriorityQErase(sched->pq, &task_uid, TaskUidCompare);
}

size_t SchedSize(const scheduler_t *sched)
{
	assert(NULL != sched);

	return PriorityQSize(sched->pq);
}

int SchedIsEmpty(const scheduler_t *sched)
{
	assert(NULL != sched);

	return PriorityQIsEmpty(sched->pq);
}

void SchedClear(scheduler_t *sched)
{
	assert(NULL != sched);

	PriorityQClear(sched->pq);
}

int SchedRun(scheduler_t *sched)
{
	assert(NULL != sched);

	while (!SchedIsEmpty(sched))
	{
		sched->current_task = (task_t *)(PriorityQPeek(sched->pq));
		PriorityQDequeue(sched->pq);
		if (REPEAT == TaskExecute(sched->current_task))
		{
			TaskUpdateTimeToExecute(sched->current_task);
			PriorityQEnqueue(sched->current_task);
		}
	}
}

