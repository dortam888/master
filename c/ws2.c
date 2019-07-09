#include <stdio.h>
#include <string.h>

int IsPalindrome(const char *str)


int main()
{
    return 0;
}


int IsPalindrome(const char *str)
{
    int i = 0, size = strlen(str);
    const char *beginning = str;
    const char *end = str + size;
    
    for (i = 0; i < size / 2; i++)
    {
        
        if (*beginning != *end)
        {
            return 0;
        }
    
        beginning++;
        ending--;
    }
    
    return 1;
    
}

void RmSpaces(char *str)
{
    
     
    
