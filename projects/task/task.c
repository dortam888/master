/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 28/08/2019
**** Description: This file contains implementations of functions
				  for tasks
*******************************************************************************/

#include <stdlib.h> /*malloc free*/
#include <assert.h> /*assert*/

#include "task.h" /*task_t*/

struct task
{
	uid_t uid;
	action_func_t action_func;
	void *params;
	time_t time_to_execute;
	time_t time_between_execution;
};

task_t *TaskCreate(action_func_t action_func, 
				   void *params,
				   time_t exe_interval_in_seconds, 
				   time_t execution_start_time)
{
	task_t *new_task = NULL;

	assert(NULL != action_func);

	new_task = (task_t *)malloc(sizeof(task_t));
	if(NULL == new_task)
	{
		return NULL;
	}

	new_task->uid = UIDCreate();
	if (UIDIsSame(BAD_UID, new_task->uid))
	{
		free(new_task); new_task = NULL;
		return NULL;
	}

	new_task->action_func = action_func;
	new_task->params = params;
	new_task->time_to_execute = execution_start_time;
	new_task->time_between_execution = exe_interval_in_seconds;

	return new_task;
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);

	task->action_func = NULL;
	task->params = NULL;

	free(task); task = NULL;
}

time_t TaskGetTimeToExecution(const task_t *task)
{
	assert(NULL != task);

	return task->time_to_execute;
}

int TaskExecute(task_t *task)
{
	assert(NULL != task);

	return task->action_func(task->params);
}

uid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);

	return task->uid;
}

void TaskUpdateTimeToExecute(task_t *task)
{
	assert(NULL != task);

	task->time_to_execute += task->time_between_execution;
}
