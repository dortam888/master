#include <stdio.h> 
#include <string.h> /* strlen, strtok */
#include <math.h> /* abs */
#include <stdlib.h> /*atoi */

int IsPalindrome(const char *str);
void SevenBoom(int from, int to);
int CheckSeven(int number);
void RmSpaces(char *str);
char *StringAdd(char *str1, char *str2);
void AddChars(char *str1, char *str2, char *destination, int *residual);
<<<<<<< HEAD
char int2char(int number); /* turns int to char: 1 -> '1' */
int char2int(char c); /* turns char to int: '1' -> 1 */
=======
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d

int main()
{
    
<<<<<<< HEAD
	char *str1 = "1541";
	char *str2 = "29391";
    char *dest = StringAdd(str1, str2);
	printf("%s\n", dest);
=======
	char *str1 = "11111111";
	char *str2 = "222222222";
    char *dest = StringAdd(str1, str2);
	printf("%s", dest);
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
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
<<<<<<< HEAD
{
    printf("7 BOOM! from %d to %d is: \n", from, to);    
    
    while (from <= to)
    {
        
        if (0 == from % 7  || 1 == CheckSeven(from))
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
    while (0 != num)
    {
        if (7 == abs(num % 10)) 
        {
            return 1;
        } 
        
        num /= 10;
    }
    
=======
{
    printf("7 BOOM! from %d to %d is: \n", from, to);    
    
    while (from <= to)
    {
        
        if (0 == from % 7  || 1 == CheckSeven(from))
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
    while (0 != num)
    {
        if (7 == abs(num % 10)) 
        {
            return 1;
        } 
        
        num /= 10;
    }
    
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
    return 0;
}

/*        
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
*/     

char *StringAdd(char *str1, char *str2)
{
<<<<<<< HEAD
=======
	int result = 0;
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
    int residual = 0;
	int str1_len = strlen(str1);
	int str2_len = strlen(str2);
	int max_len = (str1_len > str2_len) ? str1_len : str2_len;
	int min_len = (max_len == str1_len) ? str2_len : str1_len;
	char *strmax = (max_len == str1_len) ? str1 : str2;
	char *dest_start = (char *)malloc(max_len + 1);
	char *strmin = (strmax == str1)? str2 : str1;
	char *strmax_end = strmax + max_len - 1;
	char *strmin_end = strmin + min_len - 1;
	char *dest_end = dest_start + max_len; 	
   
    do
    {
<<<<<<< HEAD
		if (strmin <= strmin_end)
		{
            AddChars(strmax_end, strmin_end, dest_end, &residual);
            
            strmax_end--;
            strmin_end--;
		    dest_end--;
=======
		if (strmin != strmin_end)
		{
		    result = (*strmax_end - 48) + (*strmin_end - 48) + residual; /* -48 to turn char to int */

		    if (10 <= result)
		    {
		        result -= 10;
				residual = 1;
		    }
			else
			{
				residual = 0;
			}

		    sprintf(dest_end, "%d", result);
		    strmax_end--;
		    strmin_end--;
			dest_end--;
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
		}

		else
		{
<<<<<<< HEAD
		    AddChars(strmax_end, "0", dest_end, &residual);
=======
		    result = (*strmax_end - 48) + residual; /* -48 to turn char to int */

		    if (10 <= result)
		    {
		        result -= 10;
				residual = 1;
		    }
			else
			{
				residual = 0;
			}

		    sprintf(dest_end, "%d", result);
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
		    strmax_end--;
			dest_end--;
		}
    }
	while (strmax_end >= strmax);

<<<<<<< HEAD
	*dest_end = residual ? 49 : 32; 	
=======
	if (residual == 0)
	{				
		*dest_end = '0'; /* patch of patches */
	} 
	else
	{
		sprintf(dest_end, "%d", 1);	
	}		
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
		
    return dest_start;
}       


void AddChars(char *str1, char *str2, char *destination, int *residual)
{		
		int result = 0;

<<<<<<< HEAD
        result = char2int(*str1) + char2int(*str2) + *residual; 
=======
        result = (*str1 - 48) + (*str2 - 48) + *residual; /* -48 to turn char to int */
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d

        if (10 <= result)
        {
            result -= 10;
			*residual = 1;
        }
		else
		{
			*residual = 0;
		}

<<<<<<< HEAD
        *destination = int2char(result); 
}


char int2char(int number)
{
    return number + 48;
}


int char2int(char c)
{
    return c - 48;
}

=======
        sprintf(destination, "%d", result);
        str1--;
        str2--;
		destination--;
}
>>>>>>> 1d34140a0a3366286b57acf07c5b7ec7ad49cb0d
