#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 256

void PrintTA_ifelse();
void PrintTA_switch();
static void Esc();
static void A();
static void T();
void PrintTA_LUT();

int main()
{
	system("stty -icanon -echo");
	/*PrintTA_ifelse();*/
	/*PrintTA_switch();*/
	PrintTA_LUT();
	return 0;
}


void PrintTA_ifelse()
{
	char ch = 0;

	while (1)
	{
		ch = getchar();
		
		if ('a' == ch)
		{
			printf("A pressed\n");
		}
		else if ('t' == ch)
		{
			printf("T pressed\n");
		}
		else if (27 == ch)
		{
			printf("Program execution stopped by ESC\n");
			system("stty icanon echo");
			exit(0);
		}
	}			
}


void PrintTA_switch()
{
	char ch = 0;
	
	while (1)
	{
		ch = getchar();
		
		switch (ch)
		{
			case 'a':
			printf("A pressed\n");
			break;
			
			case 't':
			printf("T pressed\n");
			break;

			case 27:
			printf("Program execution stopped by ESC\n");
			system("stty icanon echo");
			exit(0);
			break;
		}
	}
}			


static void Esc()
{
	printf("Program execution stopped by ESC\n");
	system("stty icanon echo");
	exit(0);
}

static void A()
{
	printf("A pressed\n");
}


static void T()
{
	printf("T pressed\n");
}

void PrintTA_LUT()
{
	int i = 0;
	int ch = 0;
	
	void (*lookup[MAXCHAR])(void);

	for (; i < MAXCHAR; i++)
	{
		lookup[i] = NULL;
	}
	
	lookup[27] = &Esc;
	lookup[97] = &A;
	lookup[116] = &T;

	while (1)
	{
		ch = getchar();
		lookup[ch]();
	}

}


