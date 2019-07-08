#include <stdio.h> //tolower
#include <string.h>
#include "String.h"


int main()
{

printf("%d", My_strlen("abcdefg"));
return 0;
}


size_t My_strlen(const char *str)
{   
    const char *cpystr = str;

    while (*cpystr != '\0')
    {
        str++;
    }
    
    return (size_t)(cpystr - str);
}


char *My_strcpy(char *destination, const char *source) /* destination and source should not overlap */
{

    size_t i = 0;
    char *tmpadress = destination;
    
    for (i = 0; i < My_strlen(source); i++)
    {
        *destination = *source;
        destination++;
        source++;
    }
    
    *destination = '\0';
    
    return tmpadress; 
}


char *My_strncpy(char *destination, const char *source, size_t num); /* destination and source should not overlap */
{
    size_t i = 0;
    char *tmpadress = destination;
    
    while (i <= num || My_strlen(source) < num)
    {
        *destination = *source;
        destination++;
        source++;
        i++;
    }
    
    for (i; i < num; i++)
    {
        *destination = '\0';
        destination ++;
    }
                                                                                                                                                                                    
    return tmpadress; 
}   

int My_strcmp(const char *str1, const char *str2)
{

    while (*str1 != 0 && *str2 != 0 && 
          (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    
    return *str1 - *str2;
}


int My_strcasecmp(const char *f1, const char *f2)
{

    while (*f1 != 0 && *f2 != 0 && 
          (tolower(*f1) == tolower(*f2)))
    {
        f1++;
        f2++;
    }
    
    return tolower(*f1) - tolower(*f2);
}


char *My_strchr(const char *str, int c)
{

    while(*str != '\0' && *str != c)
    {
        str++;
    }
    
    return str;
}

char *My_strdup(const char *source)
{
    char *destination = malloc(sizeof(source));
    
    return My_strcpy(destination, source);
}

char *My_strcat(char *destination, const char *source)
{
    
           
    

