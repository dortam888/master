#include <stdio.h>
#include <string.h>
#define CMD 100

/* Our functions usually starts with capital letters vars with small letters 
   to prevent confusion between functions and variables
   for example Strcpyfunc(char *str_destination, char *str_source) */
   
int calculateStrLength(char *str); 
void testStrLength(char *str);
void strcpyFunction(char *strDestination, char *strSource);
void testStrcpy(char *strDes, char *str);
int main()
{

	/*variables for strlen function*/
	char str[CMD]; // Use dynamic allocation instead to include strings above 100
	int strLength = 0;
	
	/*variables for strcpy function*/
	char strDes[CMD]; // Use dynamic allocation instead to include strings above 100
	char str1[] = "hello";
	
	printf("strlen:\n");
	printf("Please enter a string: ");
	scanf("%s",str);
	printf("The string you entered is: %s\n",str);
	strLength = calculateStrLength(str);
	printf("The string length is: %d\n",strLength);
	testStrLength(str);
	
	printf("\nStrcpy: \n");
	printf("The string you want to copy is: %s",str1);
	strcpyFunction(strDes,str1);
	testStrcpy(strDes,str1);
	
	return 0;
}


/*strlen*/
int calculateStrLength(char *str)
{
	int countChars = 0;
	int i = 0; // Unnecessary
	for(i = 0; str[i] != '\0'; i++) // Double declaration also unnecessary try without i
	{
		countChars++;
	}
	
	return countChars;
}


void testStrLength(char *str) 
{
	calculateStrLength(str) == (int)strlen(str) ? printf("Function works\n") : printf("Function doesn't work\n"); // Longer than 80 tabs. break this.
}

/*strcpy*/
void strcpyFunction(char *strDestination, char *strSource) // Close but not working :( 
{
	int i;
	for(i = 0; i < (int)strlen(strSource); i++)
	{
		strDestination[i] = strSource[i];	
	}
}


void testStrcpy(char *strDes, char *str) // I said the function doesn't work yet the test is biased to be correct. you should fix it as well
{
	int i;
	int count = 0;
	if(strlen(strDes) != strlen(str))
		printf("Function doesn't work");
	for(i = 0; i < (int)(strlen(strDes)); i++)
	{
		if(strDes[i] != str[i])
		{
			count++;
		}	
	}
	count > 0 ? printf("Function doesn't work\n") : printf("\nFunction works\n"); // Longer than 80 tabs. break this.
}

