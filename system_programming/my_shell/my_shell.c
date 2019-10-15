/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Tuesday October 15 2019 05:32:14 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure my_shell.
                                  Look at my_shell.h for more information about the 
                                  functions.
*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "my_shell.h"


#define MAX_STR_LEN 200000


static void Parser(char *command, char **parameter_list, char *string_input)
{
    size_t i = 0;
    char* token = strtok(str, " ");

    while (token != NULL) { 
        sprintf("%s", parameter_list[i], token);
        ++i;
        token = strtok(NULL, " ");
    }
}

int Shell()
{
    while (1)
    {
        char string_input[MAX_STR_LEN] = {0};
        char *command = {0};
        char *parameter_list[MAX_STR_LEN] = {NULL};
        pid_t pid = 0;

        fgets(string_input, MAX_STR_LEN, stdin);
        
        Parser(command, parameter_list, string_input);
        
        if ((pid = fork()) == -1)
        {
            
        }
        else
        {
            execv(parameter_list[0], )
        }
        wait(NULL);
    }

}



int main()
{
    pid_t pid;
    char *const parmList[] = {"ls", "-l", NULL};

    if ((pid = fork()) == -1)
    {
        printf("fork error");
    }
    else if (pid == 0) {
        execvp("ls", parmList);
        printf("Return not expected. Must be an execv error.n");
    }
    wait(NULL);

    return 0;
  }
