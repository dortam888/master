#define _POSIX_C_SOURCE 1

#include <stdio.h> /* printf */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* fork, execvp sleep*/
#include <signal.h> /* sigaction */

#define UNUSED(x) ((void)(x))

static const size_t NUM_OF_PINGPONGS = 10000;

typedef enum ping_pong_status 
{
    SUCCESS,
    SIGEMPTYSET_FAILURE,
    SIGACTION_FAILURE,
    FORK_FAILURE
}ping_pong_status_t;

static void USRHandler(int signal)
{
    UNUSED(signal);
    return;
}

static ping_pong_status_t SigactionCreate(struct sigaction *sig_act)
{
	if (0 != sigemptyset(&sig_act->sa_mask))
	{
		return SIGEMPTYSET_FAILURE;
	}

    sig_act->sa_flags = 0;
    sig_act->sa_handler = USRHandler;

    return SUCCESS;
}

static ping_pong_status_t SetUSRSingalsHandler(struct sigaction *sa)
{
    if ((0 == sigaction(SIGUSR1, sa, NULL)) && 
       ((0 == sigaction(SIGUSR2, sa, NULL))))
    {
        return SUCCESS;
    }

    return SIGACTION_FAILURE;
}

static void PrintPingPong(int signum)
{
    switch(signum)
    {
        case SIGUSR1:
            printf("PING!\n");
			break;
		case SIGUSR2:
		    printf("PONG!\n");
			break;
		default:
		    printf("not USR signal\n");
		    break;
	}
}

static void ProcessAction(pid_t pid, int signum)
{
    PrintPingPong(signum);
    sleep(1);
    kill(pid, signum);
    pause();
}

static void ChildProcess()
{
    pause();

    while (1)
    {
        ProcessAction(getppid(), SIGUSR2);
    }
}

static void ParentProcess(pid_t child_pid)
{
	size_t num_of_pings = 0;
    
    for (num_of_pings = 0; num_of_pings < NUM_OF_PINGPONGS; ++num_of_pings)
    {
        ProcessAction(child_pid, SIGUSR1);
    }
    
    kill(child_pid, SIGABRT);
}

enum ping_pong_status PingPong(void)
{
    pid_t pid = 0;
    struct sigaction sa;

    if (SUCCESS != SigactionCreate(&sa))
    {
        return SIGACTION_FAILURE;
    }

    if (SUCCESS != SetUSRSingalsHandler(&sa))
    {
        return SIGEMPTYSET_FAILURE;
    }

    if (0 > (pid = fork()))
    {
        return FORK_FAILURE;
    }
    else if (0 == pid)
    {
        ChildProcess();
    }
    else
    { 
        ParentProcess(pid);
    }
    
    return SUCCESS;
}

int main(void)
{
    PingPong();

    return 0;
}
