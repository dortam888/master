#define _POSIX_C_SOURCE 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h> /* pid_t */
#include <pthread.h> /* pthread_create */
#include <signal.h> /* sigaction */
#include <stdlib.h> /*malloc free*/
#include <assert.h> /*assert*/
#ifdef _WIN32
#include <Windows.h> /* sleep for windows OS*/
#else
#include <unistd.h> /* fork, execvp sleep*/
#endif
#include <time.h> /* time */
#include <string.h>

#include "scheduler.h" /*scheduler_t*/
#include "priority_queue.h" /*pq_t*/
#include "task.h" /*task_t*/

#define UNUSED(x) ((void)(x))
#define SEM_KEY 0x1111

typedef enum watchdog_status 
{
    SUCCESS,
    SIGEMPTYSET_FAILURE,
    SIGACTION_FAILURE,
    FORK_FAILURE,   
    SEMAPHORE_FAILURE,
    MEMORY_ALLOCATION_FAILURE
} watchdog_status_t;

static struct sembuf post_plus = {0, 1, SEM_UNDO};
static struct sembuf post_minus = {0, -1, SEM_UNDO};
static struct sembuf wait = {0, 0, SEM_UNDO};

static void WatchMeDawg(int signal)
{
    /* TODO */
}

static void *RunSchedThread(void *scheduler)
{
    scheduler_t *sched = (scheduler_t *)scheduler;

    if (semop(sem_id, &post_minus, 1) < 0)
    {
        perror("semop");
    }

    SchedRun(sched);

    return NULL;
}

static watchdog_status_t SigactionCreate(struct sigaction *sig_act)
{
	if (0 != sigemptyset(&sig_act->sa_mask))
	{
		return SIGEMPTYSET_FAILURE;
	}

    sig_act->sa_flags = 0;
    sig_act->sa_handler = WatchMeDawg;

    return SUCCESS;
}

static watchdog_status_t SetUSRSingalsHandler(struct sigaction *sa)
{
    if ((0 == sigaction(SIGUSR1, sa, NULL)) && 
       ((0 == sigaction(SIGUSR2, sa, NULL))))
    {
        return SUCCESS;
    }

    return SIGACTION_FAILURE;
}

int WDMakeMeImmortal(char *argv[], time_t interval)
{
    pid_t pid = 0;
    struct sigaction sa;
    scheduler_t *new_scheduler = NULL;
    int sem_id = 0;

    if (semget(SEM_KEY, 1, 0666 | IPC_CREAT) < 0)
    {
        perror("semget");
        return SEMAPHORE_FAILURE;
    }

    if (0 > (pid = fork()))
    {
        perror("fork");
        return FORK_FAILURE;
    }
    else if (0 == pid)
    {
        ChildProcess();
    }
    else
    {
        if (SUCCESS != SigactionCreate(&sa))
        {
            perror("sigaction");
            return SIGACTION_FAILURE;
        }

        new_scheduler = SchedCreate();
        if (NULL == new_scheduler)
        {
            perror("scheduler error");
            return MEMORY_ALLOCATION_FAILURE;
        }

        if (UIDIsSame(BAD_UID, 
        SchedAddTask(new_scheduler, CheckFlag, NULL, interval, time(NULL))))
        {
            perror("task error");
            return MEMORY_ALLOCATION_FAILURE;
        }
        
        pthread_create();
        if (semop(sem_id, &post_plus, 1) < 0)
        {
            perror("semop");
            return SEMAPHORE_FAILURE;
        }

        if (semop(sem_id, &wait, 1) < 0)
        {
            perror("semop");
            return SEMAPHORE_FAILURE;
        }
    }

    return SUCCESS;
}


