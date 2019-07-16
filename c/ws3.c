#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <ctype.h> /* tolower */
#include <string.h> /* strlen */
#include <assert.h> /* assert */

char **CreateBuffer(char **env, int *number_of_strings);
static void PrintBuffer(char **array, int array_length);
static void FreeBuffer(char **array, int array_length);
int SoldierProblem(int num_of_soldiers);
static int FindNextLiveSoldier(int arr[], int num_of_soldiers, int start_index);
static void Test_SoldierProblem();
void Sizes();

enum Status{ Kill = 0, Alive = 1};


int main(int argc, char *argv[], char **env)
{		
	
	int number_of_strings_in_env = 0;
	char **buffer = NULL;

	buffer = CreateBuffer(env, &number_of_strings_in_env);

	PrintBuffer(buffer, number_of_strings_in_env);
	
	FreeBuffer(buffer, number_of_strings_in_env);
	
	Test_SoldierProblem();

	Sizes();
	
	(void)argc;
	(void)argv;

	return 0;	
}

void Sizes()
{
	printf("\033[1;36m\nList of data types in C:\033[0m \n");
	printf("short int: %ld bytes\n",sizeof(short int));
	printf("unsigned short int: %lu bytes\n",sizeof(unsigned short int));
	printf("unsigned int: %lu bytes\n",sizeof(unsigned int));
	printf("int: %ld bytes\n",sizeof(int));
	printf("long int: %ld bytes\n",sizeof(long int));
	printf("unsigned long int: %lu bytes\n",sizeof(unsigned long int));
	printf("long int: %ld bytes\n",sizeof(long int));
	printf("signed char: %ld bytes\n",sizeof(signed char));
	printf("unsigned char: %ld bytes\n",sizeof(unsigned char));
	printf("float: %ld bytes\n",sizeof(float));
	printf("double: %ld bytes\n",sizeof(double));
	printf("long double: %ld bytes\n",sizeof(long double));
	printf("\n");
}

char **CreateBuffer(char **env, int *number_of_strings)
{
	char **buffer = NULL;
	int i = 0, j = 0;
	
	char **end_of_env = env;
	
	while (*end_of_env != 0)
	{
		end_of_env++;
	}
	
	*number_of_strings = end_of_env - env;

	buffer = malloc(*number_of_strings * sizeof(char*));
	assert(NULL != buffer);

	for (i = 0; i < *number_of_strings; i++)
	{
		int env_i_length = strlen(env[i]);

		buffer[i] = (char *)malloc(env_i_length + 1);
		assert(NULL != buffer[i]);

		for (j = 0; j < env_i_length + 1; j++)
		{
			buffer[i][j] = tolower(env[i][j]);
		}
	}

	return buffer;
}


static void PrintBuffer(char **array, int array_length)
{
	int i = 0;
	
	for (i = 0; i < array_length; i++)
	{
		printf("%s\n", array[i]);
	}
}	
				

static void FreeBuffer(char **array, int array_length)
{
	int i = 0;
	
	for (i = 0; i < array_length; i++)
	{
		free(array[i]);
	}

	free(array);
}


int SoldierProblem(int num_of_soldiers)
{
	int soldier_initialization = 0;
	int *soldiers = malloc(num_of_soldiers * sizeof(int)); 
	int sword_position = 0;
	int killed_soldier = 0;

	assert(NULL != soldiers); /* also check that num_of_soldiers != 0 */
	
	for (soldier_initialization = 0; soldier_initialization < num_of_soldiers; 
		 soldier_initialization++)
	{
		soldiers[soldier_initialization] = Alive;
	}
	
    while (1)
    {
        killed_soldier = FindNextLiveSoldier(soldiers, num_of_soldiers, sword_position);
        
        if (killed_soldier == sword_position)
        {
    	    free(soldiers);	
    		return sword_position + 1;
        }
        
        soldiers[killed_soldier] = Kill; 
        sword_position = FindNextLiveSoldier(soldiers, num_of_soldiers, killed_soldier);
    }
    
}


static int FindNextLiveSoldier(int *soldier, int num_of_soldiers, int current_index) /* array size not useful name */
{
	int i = current_index;
	
	while (1)
	{
		i++;
		
		if (i == num_of_soldiers) /*out of bounds case*/
		{
			i = 0;
		}
		
		if (soldier[i] == Alive) /*while there is alive soldier*/
		{
			return i;
		}
	}
}


static void Test_SoldierProblem()
{
	int n1 = 100, n2 = 1, n3 = 2, n4 = 1000;

	printf("The last soldier is: %d\n",SoldierProblem(n1));
	printf("The last soldier is: %d\n",SoldierProblem(n2));
	printf("The last soldier is: %d\n",SoldierProblem(n3));
	printf("The last soldier is: %d\n",SoldierProblem(n4));
}



	
