#include <stdio.h> /* printf, sprintf, fopen, fclose, fgets*/
#include <stdlib.h> /* exit, */
#include <string.h> /* strcmp */

enum return_type_t {Exit, Remove, Count, First_line, Print, ERROR};

struct operation_t
{
	const char *string;
	int (*compare)(const char *, const char *);
	enum return_type_t (*operation)(const char *, const char *, 
									struct operation_t *);
};


void Logger(const char *file_name);

static int AlwaysTrue(const char *a, const char *b);
static int PushUpKeyFind(const char *str1, const char *str2);
static FILE *OpenFile(const char *file_name);
static void CountRows(const char *file_name, FILE *file);
/*                                                                       */
static enum return_type_t AppendOperation(const char *file_name, 
									      const char *string,
							              struct operation_t *operation);
/*                                                                       */							              
static enum return_type_t CountOperation(const char *file_name, 
										 const char *string,
							      		 struct operation_t *operation);
/*                                                                       */							      		 
static enum return_type_t ExitOperation(const char *file_name, 
										const char *string,
							     		struct operation_t *operation);
/*                                                                       */							     		
static enum return_type_t RemoveOperation(const char *file_name, 
										  const char *string,
							       		  struct operation_t *operation);
/*                                                                       */									   									   
static enum return_type_t PushUpLineOperation(const char *file_name, 
											  const char *string,
									   		  struct operation_t *operation);
/*                                                                       */									   		  
static enum return_type_t NextInChain(const char *file_name, 
									  const char *string,
							   		  struct operation_t *operation);									 

									 
#define MAXSTR 200

 
int main(int argc, char *argv[])
{
	Logger(argv[1]);
	return 0;
	(void)argc;
}


static FILE *OpenFile(const char *file_name)
{
    FILE *file = fopen(file_name, "a+");
 
    if (file == NULL) 
    { 
        printf("Could not open file %s", file_name); 
    } 
    
    return file;
}


static int AlwaysTrue(const char *str1, const char *str2)
{
	str1++;
	str2++;
	return 1;
}


static int PushUpKeyFind(const char *str1, const char *str2)
{
	if (*str1 == *str2)
	{
		return 0;
	}
	
	return 1;
}

static enum return_type_t NextInChain(const char *file_name, 
							          const char *string,
									  struct operation_t *operation)
{
	enum return_type_t next_return = ERROR;
	struct operation_t *new_operation = operation + 1;
	next_return = new_operation[0].operation(file_name, string, new_operation);
	return next_return;
}


static enum return_type_t AppendOperation(const char *file_name, 
										  const char *string, 
								          struct operation_t *operation)
{
	FILE *log_file = OpenFile(file_name);
	fprintf(log_file, "%s", string);
	fclose(log_file);

	operation++; /* to remove warning */
	
	return Print;
}


static void CountRows(const char *file_name, FILE *file)
{
    int count = 0;  
    char c = '\0';
   
    for (c = getc(file); c != EOF; c = getc(file)) 
	{
    	if (c == '\n')
		{ 
            ++count; 
		}
 	}
 
    printf("The file %s has %d lines\n ", file_name, count); 
}


static enum return_type_t CountOperation(const char *file_name, 
										 const char *string, 
								         struct operation_t *operation)
{   
    if (!operation[0].compare(string, operation[0].string))
    {
    	FILE *log_file = OpenFile(file_name);
    	CountRows(file_name, log_file);
    	fclose(log_file);
    	return Count; 
	}
	else
	{
		enum return_type_t next = NextInChain(file_name, string, operation); 
		return next;
	} 
	
	return ERROR;   	
}


static enum return_type_t ExitOperation(const char *file_name, 
									    const char *string, 
							  			struct operation_t *operation)
{
	if (!operation[0].compare(string, operation[0].string))
	{
		return Exit;
	}
	else
	{
		enum return_type_t next = NextInChain(file_name, string, operation); 
		return next;
	}
	
	return ERROR;
}


static enum return_type_t RemoveOperation(const char *file_name, 
										  const char *string, 
								 		  struct operation_t *operation)
{
	if (!operation[0].compare(string, operation[0].string))
	{
		remove(file_name);
		return Remove;
	}
	else
	{
		enum return_type_t next = NextInChain(file_name, string, operation); 
		return next;
	}
	
	return ERROR;	
}


static enum return_type_t PushUpLineOperation(const char *file_name, 
											  const char *string, 
								              struct operation_t *operation)
{
	if (!operation[0].compare(string, operation[0].string))
	{
		char newstring[MAXSTR + 20];
		++string; /* to print without < */
		sprintf(newstring, "sed -i '1 i\\%s' %s", string, file_name); 
		system(newstring);
		return First_line;
	}
	else
	{
		enum return_type_t next = NextInChain(file_name, string, operation); 
		return next;
	}
	
	return ERROR;
}


void Logger(const char *file_name)
{
	char input[MAXSTR];
	enum return_type_t action = ERROR;	
	struct operation_t array_of_operation[] =
									 {{"-count\n", strcmp, CountOperation},
									  {"-exit\n", strcmp, ExitOperation},
									  {"-remove\n", strcmp, RemoveOperation},
									  {"<", PushUpKeyFind, PushUpLineOperation},
 									  {"", AlwaysTrue, AppendOperation}};

	while (action != Exit)
	{
		printf("Enter text to log file: ");
		fgets(input, MAXSTR, stdin); 
		action = array_of_operation[0].operation(file_name, input, 
												  array_of_operation);
	}
	
	printf("Bye Bye\n");
	exit(0);
}

/* END OF FILE */

