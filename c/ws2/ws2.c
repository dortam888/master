#include <stdio.h> 
#include <string.h> /* strlen, strtok */
#include <math.h> /* abs */
#include <stdlib.h> /*atoi */

int IsPalindrome(const char *str);
void SevenBoom(int from, int to);
int CheckSeven(int number);
void SwapPtr(int **ptr1, int **ptr2);
void RmSpaces(char *str);
char *StringAdd(char *str1, char *str2);
void AddChars(char *str1, char *str2, char *destination, int *residual);
char int2char(int number); /* turns int to char: 1 -> '1' */
int char2int(char c); /* turns char to int: '1' -> 1 */
void Test_Add_String();


int main()
{
	Test_Add_String();

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

    return 0;
}


void SwapPtr(int **ptr1, int **ptr2)
{
	int *temp = *ptr1;
	
	*ptr1 = * ptr2;
	*ptr2 = temp;
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
		if (strmin <= strmin_end)
		{
            AddChars(strmax_end, strmin_end, dest_end, &residual);
            
            strmax_end--;
            strmin_end--;
		    dest_end--;
		}

		else
		{
		    AddChars(strmax_end, "0", dest_end, &residual);

		    strmax_end--;
			dest_end--;
		}
    }
	while (strmax_end >= strmax);


	*dest_end = residual ? 49 : 32; /* patch of patches puts 1 if there is residual or space if not*/	
		
    return dest_start;
}       


void AddChars(char *str1, char *str2, char *destination, int *residual)
{		
		int result = 0;

        result = char2int(*str1) + char2int(*str2) + *residual; 

        if (10 <= result)
        {
            result -= 10;
			*residual = 1;
        }
		else
		{
			*residual = 0;
		}

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

void Test_Add_String()
{
	char *str1 = "1541";
	char *str2 = "29391";
    char *dest = StringAdd(str1, str2);
	printf("first number is: %s\n", str1);
	printf("second number is: %s\n", str2);
	printf("result: %s\n", dest);
}
