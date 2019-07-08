#include <stdio.h>
#include <string.h>
#include <math.h>

size_t StrLenght(const char *str);
void TestStrLenght();
int CmprStr(const char *str1, const char *str2);
void TestComper(char *str1, char *str2);
void CasesTestComper();
char *StrCopy(char *strdest, const char *strsourc);
void TestCpy(char *strdest, const char *strsourc);
void TestCopyStr();

int main()
{
	
	TestStrLenght();
	
	CasesTestComper();
	
	TestCopyStr();
	
	return 0;
	
}

size_t StrLenght(const char *str)   
{
	int Lenght = 0; /* Length!!!! */
	
	while (*str++ != 0)
	{
		Lenght++;
	}
	
	return Lenght;
}

void TestStrLenght()
{
	char str[] = "Testing Function";
	
	if ( StrLenght(str) == strlen(str))
	{
		printf("success in StrLenght test\n");
	}
	
	else
	{
		printf("failure in StrLenght test\n");
	}
	
	return; /* No need */
}

int CmprStr( const char *str1, const char *str2) /* no space! */
{
	int diff = 0;
	
	while ((*str1 != 0) && (*str2 != 0) && (*str1 == *str2))
	{
		str1++;
		str2++;
	}

	diff = (int)*str1 - (int)*str2; /* will it works without casting? */
		
	return diff;
}
		
void TestComper(char *str1, char *str2)
{
	
	if (CmprStr(str1, str2) == (int)strcmp(str1, str2))
	{
		printf("success in test of CmprStr for %s and %s\n", str1, str2);
	}
	
	else
	{
		printf("failure in test of CmprStr\n");
	}
}

void CasesTestComper() /* what about identical strings case? */
{
	char str1[] = "Hello world";
	char str2[] = "Hello worldt";
	char str3[] = "";
	char str4[] = "hello world";
	
	TestComper(str1, str2);
	TestComper(str1, str3);
	TestComper(str1, str4);
}
	
	
char *StrCopy(char *strdest, const char *strsourc) /* Not correct */
{
	char *tempaddres = strdest;
	
	while (*strsourc != 0)
	{
		*strdest = *strsourc;
		strdest++;
		strsourc++;
	}
	
	return tempaddres;
}

void TestCpy(char *strdest,const char *strsourc)
{
	if (((StrCopy(strdest, strsourc) == strcpy(strdest, strsourc))) && 
	(strcmp(StrCopy(strdest, strsourc), strcpy(strdest, strsourc)) == 0))
	{
		printf("success in StrCopy test for str: %s\n", strsourc);
	}
	
	else
	{
		printf("failure in StrCopy test for str: %s\n", strsourc);
	}
}

void TestCopyStr() /* what about cases where you don't have enough space? what will happen? what should happen? */
{
	const char str1[] = "testing function";
	const char str2[] = "";
	
	char coppiedstr[20];
	TestCpy(coppiedstr, str1);
	TestCpy(coppiedstr, str2);
}

