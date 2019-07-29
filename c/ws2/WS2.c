#include <stdio.h>
#include <string.h>
size_t My_strlen(const char *string);
char *My_strcpy(char *destination, const char *string2copy);
int My_strcmp(const char *string1, const char *string2);


int main()
{
return 0;
}


size_t My_strlen(const char *string)
{   
    size_t length = 0;

    while(string[length])
    {
        ++length;
    }
    
    return length;
}


char *My_strcpy(char *destination, const char *string2copy)
{
    for(int i = 0; i <= My_strlen(string2copy); i++)
    {
        destination[i] = string2copy[i];
    }
    
    return destination; 
}


int My_strcmp(const char *string1, const char *string2)
{
    int length = 0;
    while(string1[length] && string2[length] && //
         (string1[length] == string2[length]))
    {
        ++length;
    }
    
    return string1[length] - string2[length];
}



