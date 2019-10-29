#define _POSIX_C_SOURCE 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* fork, execvp sleep*/
#include <signal.h> /* sigaction */


