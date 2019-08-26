#include <stdlib.h> /*malloc free*/
#include <assert.h> /*assert*/
#ifdef _WIN32
#include <Windows.h> /*sleep*/
#else
#include <unistd.h> /*sleep*/
#endif
#include <time.h>

#include "scheduler.h" /*scheduler_t*/
#include "priority_queue.h" /*pq_t*/
#include "task.h" /*task_t*/

#define UNUSED(x) ((void)(x))

enum run_function_status {RUN_STOPPED_ON_SUCCESS, RUN_STOPPED_ON_FAILURE};

struct scheduler
{
	pq_t *pq;
	task_t *current_task;
	int stop_alert;
};

static int TimeCmpFunc(const void *task1, const void *task2, void *param)
{
	task1 = (task_t *)task1;
	task2 = (task_t *)task2;
	UNUSED(param);

	return (TaskGetTimeToExecution(task2) - TaskGetTimeToExecution(task1));

}

scheduler_t *SchedCreate(void)
{
	scheduler_t *new_scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_scheduler)
	{
		return NULL;
	}

	new_scheduler->pq = PriorityQCreate(TimeCmpFunc, NULL);
	if (NULL == new_scheduler->pq)
	{
		free(new_scheduler); new_scheduler = NULL;
		return NULL;
	}

	new_scheduler->current_task = NULL;
	new_scheduler->stop_alert = 0;

	return new_scheduler;
}

void SchedDestroy(scheduler_t *sched)
{
	assert(NULL != sched);

	SchedClear(sched);
	sched->current_task = NULL;

	PriorityQDestroy(sched->pq);
	sched->pq = NULL;

	free(sched); sched = NULL;
}

ilrd_uid_t SchedAddTask(scheduler_t *sched, action_func_t action_func, void *params,
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

static int TaskUidCompare(const void *data1, const void *data2)
{
	task_t *task1 = (task_t *)data1;
	ilrd_uid_t *uid2 = (ilrd_uid_t *)data2;

	return (UIDIsSame(TaskGetUID(task1), *uid2));
}


void SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_uid)
{
	task_t *task_to_free = NULL;

	assert(NULL != sched);

	task_to_free = PriorityQErase(sched->pq, &task_uid, TaskUidCompare);
	TaskDestroy(task_to_free);
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

	while (!SchedIsEmpty(sched))
	{
		TaskDestroy((task_t *)PriorityQPeek(sched->pq));
		PriorityQDequeue(sched->pq);
	}
}

int SchedRun(scheduler_t *sched)
{
	assert(NULL != sched);

	while (SchedIsEmpty(sched) == sched->stop_alert)
	{
		time_t time_till_execute = 0;
		sched->current_task = (task_t *)(PriorityQPeek(sched->pq));
		time_till_execute = TaskGetTimeToExecution(sched->current_task) - 
							time(NULL);
		sleep(time_till_execute);
		PriorityQDequeue(sched->pq);
		if (TaskExecute(sched->current_task) == REPEAT)
		{
			TaskUpdateTimeToExecute(sched->current_task);
			if (PriorityQEnqueue(sched->pq, sched->current_task) != 0)
			{
				return RUN_STOPPED_ON_FAILURE;
			}
		}
		else
		{
			TaskDestroy(sched->current_task);
		}
	}

	return RUN_STOPPED_ON_SUCCESS;
}

void SchedStop(scheduler_t *sched)
{
	sched->stop_alert = 1;
}
