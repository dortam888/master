#include <stdio.h>
#include <string.h>

int IsPalindrome(const char *str);
void SevenBoom(int from, int to);
int CheckSeven(int number);


int main()
{
    return 0;
}


int IsPalindrome(const char *str)
{

    const char *beginning = str;
    const char *end = str + strlen(str) - 1;
    
    while (end > beginning)
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

void SevenBoom(int from, int to)
{
    printf("7 BOOM! from %d to %d is", &from, &num);    
    
    while (from <= to)
    {
        
        if (from % 7 == 0 || CheckSeven(from) == 1)
        {
            printf("BOOM ");
        }
        else
        {
            printf("%d ", &from);
        }
        
        from++;
    }
}

int CheckSeven(int num)
{
    while (num != 0)
    {
        if (num % 10 == 7)
        {
            return 1;
        } 
        
        num /= 10
    }
    
    return 0;
}
        
void RmSpaces(char *str)
{
    
     
    
