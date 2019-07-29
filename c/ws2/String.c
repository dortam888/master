#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <stdio.h> /* tolower */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /*assert*/
#include <string.h>
#include <ctype.h> /* tolower */
#include "String.h"

void Test_My_strlen();
void Test_My_strcmp();
void Test_My_strcpy();
void Test_My_strncpy();
void Test_My_strcasecmp();


int main()
{
    Test_My_strlen();

    Test_My_strcmp();

    Test_My_strcpy();

    Test_My_strncpy();
    
    return 0;
}


size_t My_strlen(const char *str)
{   
    const char *cpystr = str;

    assert(str != NULL);
    
    while ('\0' != *cpystr)
    {
        cpystr++;
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
    
    while ('\0' != *str1 && *str1 == *str2)
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



char *My_strcpy(char *destination, const char *source)  /*destination and source should not overlap */ 
{
    char *tmpadress = destination;
    
    assert(destination != NULL && source != NULL); 
    

    while ('\0' != *source)
    {
        *destination = *source;
        destination++;
        source++;
    }
    
    *destination = '\0';
    
    return tmpadress; 
}


void Test_My_strcpy()
{
    const char *str1 = "strcpy"; const char *str2 = "";
    char dest[7] = "aaaaaa";
    
    assert(My_strcpy(dest, str1) == strcpy(dest, str1) &&
           strcmp(My_strcpy(dest, str1), strcpy(dest, str1)) == 0 && 
           strcmp(My_strcpy(dest, str2), strcpy(dest, str2)) == 0);
}




char *My_strncpy(char *destination, const char *source, size_t num)
{
    size_t i = 0;
    char *tmpadress = destination;
    
    assert(destination != NULL && source != NULL);
    
    while (i < num && '\0' != source)
    {
        *destination = *source;
        destination++;
        source++;
        i++;
    }
    
    for (; i < num; i++)
    {
        *destination = '\0';
        destination++;
    }
                                                                                                                                                                                    
    return tmpadress; 
}   


void Test_My_strncpy()
{
    const char *str1 = "strcpy"; const char *str2 = "st";
    char dest[7] = "aaaaaa";
    
    assert(My_strncpy(dest, str1, 4) == strncpy(dest, str1, 4) &&
           strcmp(My_strncpy(dest, str1, 4), strncpy(dest, str1, 4)) == 0 && 
           strcmp(My_strncpy(dest, str2, 4), strncpy(dest, str2, 4)) == 0);
}


int My_strcasecmp(const char *str1, const char *str2)
{

    while ('\0' != *str1 && (tolower(*str1) == tolower(*str2)))
    {
        str1++;
        str2++;
    }
    
    return tolower(*str1) - tolower(*str2);
}


void Test_My_strcasecmp()
{
    const char *str1 = "Hello"; const char *str2 = "HElLo";
    const char *str3 = ""; const char *str4 = "hi";
    const char *str5 = "hello"; const char *str6 = "hellor";
    
    assert(My_strcasecmp(str1,str2) == strcasecmp(str1,str2) &&
           My_strcasecmp(str3,str4) == strcasecmp(str3,str4) &&
           My_strcasecmp(str5,str6) == strcasecmp(str5,str6));
}


char *My_strchr(const char *str, int c)
{   
    while ('\0' != *str)

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
    
    My_strcpy(destination, source);
    
    return tmpadress;
}


/*
char *My_strstr(const char *haystack, const char *needle)
{    
    const char *start = haystack;
    const char *pattern = needle;
    
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
*/

char *My_strstr2(const char *haystack, const char *needle)
{    
    while ('\0' != *haystack)
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
    size_t count = 0;    
    
    while ('\0' != *str1)
    {       
        while (strchr(str2, *str1) != NULL)
        {
            count++;
            str1++;
        }
        
        if (0 != count)
        {
            return count;
        }
        
        str1++;        
    }
    
    return count;
}
