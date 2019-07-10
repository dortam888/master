#include <stdio.h>
#include <string.h>

int IsPalindrome(const char *str);
void SevenBoom(int from, int to);
int CheckSeven(int number);
void RmSpaces(char *str);


int main()
{
    
    RmSpaces("    h as       i am    too ");
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
        end--;
    }
    
    return 1;
    
}

void SevenBoom(int from, int to)
{
    printf("7 BOOM! from %d to %d is: \n", from, to);    
    
    while (from <= to)
    {
        
        if (from % 7 == 0 || CheckSeven(from) == 1)
        {
            printf("BOOM ");
        }
        else
        {
            printf("%d ", from);
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
        
        num /= 10;
    }
    
    return 0;
}
        
void RmSpaces(char *str)
{
    const char *s = "a";
    char *token = strtok(str,s);
    
    while (token != NULL)
    {
        printf("%s ", token);
        token = strtok(NULL,s);
    }
}       
     

char *StringAdd(char *str1, char *str2)
{
    int remain = 0;
    int result = 0;
    
    while ('\0' != str1 && '\0' != str2)
    {
        result = atoi(*str1) + atoi(*str2) + remain;
        remain = 0;
        if (result > 10)
        {
            result -= 10;
        }
        *str1 = result;
        str1++;
        str2++;
    }
    
        
