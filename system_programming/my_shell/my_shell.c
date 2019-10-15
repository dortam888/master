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
#include <sys/wait.h>
#include <unistd.h>

#include "my_shell.h"


#define MAX_STR_LEN 200000


static void Parser(char **parameter_list, char *string_input)
{
    size_t i = 0;
    char* token = strtok(string_input, " ");

    while (token != NULL) { 
        parameter_list[i] = token;
        ++i;
        token = strtok(NULL, " ");
    }
}

void Shell()
{
    char string_input[MAX_STR_LEN] = {0};
    
    while (1)
    {
        char *parameter_list[MAX_STR_LEN] = {NULL};
        pid_t pid = 0;
        char current_work_directory[MAX_STR_LEN] = {0};
        
        if (NULL != getcwd(current_work_directory, 
                           sizeof(current_work_directory)))
        {
            printf("%s$ ", current_work_directory);
        }
        
        fgets(string_input, MAX_STR_LEN, stdin);
        
        Parser(parameter_list, string_input);
        
        if (-1 == (pid = fork()))
        {
            printf("couldn't create process");
        }
        else if (0 == pid)
        {
            execvp(parameter_list[0], parameter_list);
        }
        wait(NULL);
    }
}

