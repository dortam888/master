/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Wednesday October 16 2019 02:09:17 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure signals.
                                  Look at signals.h for more information about the 
                                  functions.
*******************************************************************************/
#define _POSIX_C_SOURCE 1
#define NUMBER_OF_TIMES 10

#include <stdio.h> /*printf*/
#include <unistd.h> /*fork*/
#include <signal.h> /*sigaction*/
#include <sys/types.h>
#include <time.h> /*sleep*/
#include <stdlib.h> /*exit*/
#include <string.h> /*memset*/

#include "signals.h"

static sig_atomic_t sig_flag = 0;

static void SignalHandler(int signum)
{
    sig_flag = signum;
}

static void Handle(pid_t pid, int signum)
{
    char *pingpong[2] = {"PONG", "PING"};
    
    printf("%s!\n", pingpong[signum % 2]);
    sleep(2);
    kill(pid, signum);
}

static void ParentProcess(pid_t pid)
{
    size_t number_of_pingpongs = 0;
    
    for (number_of_pingpongs = 0; number_of_pingpongs < NUMBER_OF_TIMES; 
         ++number_of_pingpongs)
    {
        Handle(pid, sig_flag);
        pause();
    }
}

static void ChildProcess()
{
    while (1)
    {
        pid_t pid = getppid();
        Handle(pid, sig_flag);
        pause();
    }
}

void PingPong()
{
    pid_t child_pid = 0;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    
    sa.sa_handler = SignalHandler;
    
    if (-1 == sigaction(SIGUSR1, &sa, NULL))
    {
        perror("sigaction");
        exit(1);
    }
    
    if (-1 == sigaction(SIGUSR2, &sa, NULL))
    {
        perror("sigaction");
        exit(1);
    }
    
    if ((child_pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (0 == child_pid)
    {
        ChildProcess();
    }
    else
    {
        ParentProcess(child_pid);
    }
}
