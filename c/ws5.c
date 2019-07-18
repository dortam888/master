#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum return_type {Exit, Remove, Count, First_line, Print, ERROR};

int AlwaysTrue(const char *a, const char *b);
void Logger(const char *file_name);
enum return_type AppendOperation();
enum return_type CountOperation(const char *file_name, const char *string);


#define TRU 1
#define Max 200

struct operation
{
	const char *string;
	int (*compare)(const char *, const char *);
	enum return_type (*operation)(const char *, const char *);
};

 
int main()
{
	Logger("foo");
	return 0;
}


int AlwaysTrue(const char *str1, const char *str2)
{
	return 1;
}


int PushUpKeyFind(const char *str1, const char *str2)
{
	if (*str1 == *str2)
	{
		return 1;
	}
	
	return 0;
}

enum return_type AppendOperation(const char *file_name, const char *string)
{
	return Print;
}


enum return_type CountOperation(const char *file_name, const char *string)
{
    FILE *fp = fopen(file_name, "r");
    int count = 0;  
    char c = '\0';
 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", file_name); 
        return ERROR;
    } 
   
    for (c = getc(fp); c != EOF; c = getc(fp)) 
	{
    	if (c == '\n')
		{ 
            ++count; 
		}
 	}

    fclose(fp); 
    printf("The file %s has %d lines\n ", file_name, count); 
  
    return Count; 	
}

enum return_type ExitOperation(const char *file_name, const char *string)
{
	return Exit;
}


enum return_type RemoveOperation(const char *file_name, const char *string)
{
	remove(file_name);
	return Remove;
}


enum return_type PushUpLineOperation(const char *file_name, const char *string)
{
	return First_line;
}

void Logger(const char *file_name)
{
	char string[Max] = {0};
	size_t i = 0;
	
	struct operation array_of_operation[] =
									{{"-count", strcmp, CountOperation},
									 {"-exit", strcmp, ExitOperation},
									 {"-remove", strcmp, RemoveOperation},
									 {"<", PushUpKeyFind, PushUpLineOperation},
 									 {"", AlwaysTrue, AppendOperation}};

	enum return_type operate = Print;	

	while (Exit != operate)
	{
		fgets(string, Max, stdin);
		while (1)
		{
			if (array_of_operation[i].compare
									 (string, array_of_operation[i].string))
			{
				array_of_operation[i].operation(file_name, string);
				break;
			}
			++i;
		}
	}
}
