/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Sunday August 25 2019 */
/* This file contains functions for scheduler operations.*/
/*############################################################################*/

#ifndef ILRD_SCHEDULER_H
#define ILRD_SCHEDULER_H

#include <stddef.h> /* size_t */
#include <UID.h>

typedef struct scheduler scheduler_t;

enum {STOP, REPEAT};
#define STOP_REPEAT
/*#############################################################################
  # Description: 
	creates the scheduler 
  # Parameters:
	param_for_cmp - parameter for prioritize function
  # Return Value:
	in case of success returns a pointer to the scheduler
	in case of failure returns NULL
##############################################################################*/
scheduler_t *SchedCreate(void);

/*#############################################################################
  # Description:
	gets a scheduler and destroys it
  # Parameters:
	sched - pointer to the scheduler to be destroyed 
  # Return Value:
	None
##############################################################################*/
void SchedDestroy(scheduler_t *sched);

/*#############################################################################
  # Description: 
	adds task to the scheduler
  # Parameters:
	sched - a pointer to the scheduler 
  # Return Value:
	returns 0 if the data been added successefuly, otherwise returns other
	integer number
##############################################################################*/
typedef int (*task_action_func_t)(void *params);

ilrd_uid_t SchedAddTask(scheduler_t *sched, task_action_func_t action_func, void *params,
                        time_t exe_interval_in_seconds, time_t execution_start_time);

/*#############################################################################
  # Description: 
	gets the task to remove from the scheduler - if the scheduler is empty the 
	behavior will be undefined
  # Parameters:
	sched - the pointer to the scheduler which the data will be removed from
  # Return Value:
	None
##############################################################################*/
void SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_uid);

/*#############################################################################
  # Description: 
	returns how many tasks are currently inside the scheduler
  # Parameters:
	sched - the pointer to the scheduler
  # Return Value:
	number of tasks in the scheduler
##############################################################################*/
size_t SchedSize(const scheduler_t *sched);

/*#############################################################################
  # Description: 
	checks if the scheduler is empty
  # Parameters:
	sched - the pointer to the scheduler 
  # Return Value:
	1 if there are no tasks in the scheduler 0 otherwise
##############################################################################*/
int SchedIsEmpty(const scheduler_t *sched);

/*#############################################################################
  # Description: 
	gets a scheduler and clears all tasks in it.
  # Parameters:
	sched - pointer to the scheduler
  # Return Value:
	none
##############################################################################*/
void SchedClear(scheduler_t *sched);

/*#############################################################################
  # Description: 
	gets a scheduler and executes all tasks in it.
  # Parameters:
	sched - pointer to the scheduler
  # Return Value:
	none
##############################################################################*/
int SchedRun(scheduler_t *sched);

/*#############################################################################
  # Description: 
	stops run function.
  # Return Value:
	none
##############################################################################*/
void SchedStop(scheduler_t *sched);



#endif /* ILRD_SCHEDULER_H */
