#include <stdio.h> /* tolower */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /*assert*/
#include <string.h>
#include "String.h"

void Test_My_strlen();
void Test_My_strcmp();

int main()
{

Test_My_strlen();

Test_My_strcmp();

return 0;
}


size_t My_strlen(const char *str)
{   
    const char *cpystr = str;

    assert(str != NULL);
    
    while (*cpystr != '\0')
    {
        str++;
    }
    
    return (size_t)(cpystr - str);
}


void Test_My_strlen()
{
    char *str1 = "a"; char *str2 = "abcdeFAYF"; char *str3 = "";
    
    assert(My_strlen(str1) == strlen(str1) &&
           My_strlen(str2) == strlen(str2) &&
           My_strlen(str3) == strlen(str3));
}


int My_strcmp(const char *str1, const char *str2)
{

    assert(str1 != NULL && str2 != NULL);    
    
    while (*str1 != 0 && *str2 != 0 && 
          (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    
    return *str1 - *str2;
}


void Test_My_strcmp()
{
    
    const char *str1 = "Hello"; const char *str2 = "HElLo";
    const char *str3 = ""; const char *str4 = "hi";
    const char *str5 = "hello"; const char *str6 = "hello";
    
    assert(My_strcmp(str1,str2) == strcmp(str1,str2) &&
           My_strcmp(str3,str4) == strcmp(str3,str4) &&
           My_strcmp(str5,str6) == strcmp(str5,str6));
}

char *My_strcpy(char *destination, const char *source)  destination and source should not overlap 
{
    size_t i = 0;
    char *tmpadress = destination;
    
    assert(destination != NULL && source != NULL); 
    
    for (i = 0; i < My_strlen(source); i++)
    {
        *destination = *source;
        destination++;
        source++;
    }
    
    *destination = '\0';
    
    return tmpadress; 
}


char *My_strncpy(char *destination, const char *source, size_t num);  destination and source should not overlap 
{
    size_t i = 0;
    char *tmpadress = destination;
    
    assert(destination != NULL && source != NULL);
    
    while (i <= num && My_strlen(source) < num)
    {
        *destination = *source;
        destination++;
        source++;
        i++;
    }
    
    for (; i < num; i++)
    {
        *destination = '\0';
        destination ++;
    }
                                                                                                                                                                                    
    return tmpadress; 
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


char *My_strchr(const char *str, int c);
{   
    while (*str != '\0')
    {
    
        if (*str == c)
        { 
            return (char *)str;
        }

        str++;    
    }  
    
    return NULL;
}


char *My_strdup(const char *source)
{
    char *destination = (char *)malloc(My_strlen(source) + 1);
    
    return My_strcpy(destination, source);
}

char *My_strcat(char *destination, const char *source)
{
    char *tmpadress = destination;
    destination += My_strlen(destination);
    
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
    
    destination = '\0';
    
    return tmpadress;
}


char *My_strstr(const char *haystack, const char *needle)
{    
    char *start = haystack;
    char *pattern = needle;
    
    while (*haystack != '\0')
    {
        start = haystack;
        pattern = needle;
        
        while (*start != '\0' && *pattern !='\0' && *start == *pattern)
        {
            haystack++;
            pattern++;
        }
        
        if (*pattern == '\0')
        {
            return start;
        }
        
        haystack = start + 1;
    }
    
    return NULL;
}


char *My_strstr2(const char *haystack, const char *needle)
{

    assert(strlen(haystack) >= strlen(needle));
    
    while (*haystack != '\0')
    {
        
        if (strncmp(haystack, needle, strlen(needle)) == 0)
        {
            return (char *)haystack;
        }
        
        haystack++;
    }
    
    return NULL;
}
           

size_t My_strspn(const char *str1, const char *str2)
{
    unsigned int max = 0;    
    char *start = str1;
    char *pattern = str2;
    unsigned int i = 1;
    
    while (*str1 != '\0')
    {
        
        while (*start != '\0' && *pattern !='\0' && *start == *pattern)
        {
            start++;
            pattern++;
            i++;
        }
    
        if (i > max)
        {
            max = i;
        }
        
        str1 = start + 1;
    }
    
    return max;
}
