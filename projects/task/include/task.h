
/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Sunday August 25 2019 02:38 PM */
/* This file contains functions for tasks .*/
/*############################################################################*/

#ifndef ILRD_TASK_H
#define ILRD_TASK_H
#include <stddef.h> /* size_t */
#include <sys/types.h> /* size_t, time_t, pid_t */

#include <UID.h> /* uid API */

typedef struct task task_t;

enum {STOP, REPEAT}; /*return value for action func to know if task containing it should be repeated or stops*/

/*#############################################################################
  # Description: 
	creates a task 
  # Parameters:
	action_func - function for the task to execute: function returns 0 for 
	single execution, or 1 for continual execution.
	exe_interval_in_sec - interval in seconds for executing the task
	init_execution_time - the initial execution ofthe task after the scheduler 
	starts.
  # Return Value:
	in case of success returns a pointer to the task itself,
	in case of failure returns NULL
##############################################################################*/
typedef int (*action_func_t)(void *params);

task_t *TaskCreate(action_func_t action_func, 
				   void *params,
				   time_t exe_interval_in_seconds, 
				   time_t execution_start_time);

/*#############################################################################
  # Description: 
	gets a task and destroys it
  # Parameters:
	task - pointer to the task to be destroyed 
  # Return Value:
	None
###########################################################################*/
void TaskDestroy(task_t *task);

/*#############################################################################
  # Description: 
	gets the time of the next execution time of the task
  # Parameters:
	task - pointer to the task 
  # Return Value:
	task time
############################################################################*/
time_t TaskGetTimeToExecution(const task_t *task);

/*#############################################################################
  # Description: 
	execute a task 
  # Parameters:action_func_t
	task - pointer to the task 
  # Return Value:
	0 if execution succeded, otherwise returns other integer number
############################################################################*/
int TaskExecute(task_t *task);

/*#############################################################################
  # Description: 
	return the uid of a task 
  # Parameters:
	task - pointer to the task 
  # Return Value:
	uid of the task
##############################################################################*/
ilrd_uid_t TaskGetUID(const task_t *task);

/*#############################################################################
  # Description: 
	updates the time of execution of a task
  # Parameters:
	task - pointer to the task
  # Return Value:
	None
##############################################################################*/
void TaskUpdateTimeToExecute(task_t *task);

#endif /* ILRD_TASK_H */

