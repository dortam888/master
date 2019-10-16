/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Tuesday October 15 2019 05:32:14 
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure my_shell.
                                  Look at my_shell.h for more information about the 
                                  functions.
*******************************************************************************/
#include <string.h> /*strcmp strtok*/
#include <stdio.h> /*printf*/
#include <sys/types.h> /*pid_t*/
#include <sys/wait.h> /*wait*/
#include <unistd.h> /*fork execvp*/

#include "my_shell.h"


#define MAX_STR_LEN 200000

static void Parser(char **parameter_list, char *string_input)
{
    size_t i = 0;
    char* token = strtok(string_input, " ,\n");

    while (token != NULL) { 
        parameter_list[i] = token;
        ++i;
        token = strtok(NULL, " ,\n");
    }
}

enum shell_status Shell()
{
    char string_input[MAX_STR_LEN] = {0};
    enum shell_status status = 0;
    
    while (status != EXIT)
    {
        char *parameter_list[MAX_STR_LEN] = {NULL};
        pid_t pid = 0;
        char current_work_directory[MAX_STR_LEN] = {0};
        
        if (NULL != getcwd(current_work_directory, 
                           sizeof(current_work_directory)))
        {
            printf("\033[0;32m%s\033[0m$ ", current_work_directory);
        }
        
        fgets(string_input, MAX_STR_LEN, stdin);
        
        Parser(parameter_list, string_input);
        
        if (0 == strcmp("exit", parameter_list[0]))
        {
            return EXIT;
        }
        
        if (0 != (pid = fork()))
        {
            status = COULD_NOT_CREATE_CHILD_PROCESS;
        }
        else if (0 == pid)
        {
            execvp(parameter_list[0], parameter_list);
            status = FAILED_TO_EXECUTE;
        }
        wait(NULL);
    }
    
    return SUCCESS;
}

