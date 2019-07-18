#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define Number_Of_Functions 256
#define ESC 27
#define A 97
#define T 116

void PrintTA_ifelse();
void PrintTA_switch();
static void ExitFunctionAndEnableTerminal();
static void PressKey(int ch);
static void Nothingness(int ch);
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
	int ch = 0;

	while (1)
	{
		ch = getchar();
		
		if (A == ch)
		{
			PressKey(ch);
		}
		else if (T == ch)
		{
			PressKey(ch);
		}
		else if (ESC == ch)
		{
			ExitFunctionAndEnableTerminal(ch);
		}
	}			
}


void PrintTA_switch()
{
	int ch = 0;
	
	while (1)
	{
		ch = getchar();
		
		switch (ch)
		{
			case A:
			PressKey(ch);
			break;
			
			case T:
			PressKey(ch);
			break;

			case ESC:
			ExitFunctionAndEnableTerminal(ch);
			break;
		}
	}
}			


static void ExitFunctionAndEnableTerminal(int ch)
{
	(void)ch;
	printf("Program execution stopped by ESC\n");
	system("stty icanon echo");
	exit(0);
}

static void PressKey(int ch)
{
	printf("%c pressed\n", toupper(ch));
}

static void Nothingness(int ch)
{
	(void)ch;
}


void PrintTA_LUT()
{
	int i = 0;
	int ch = 0;
	
	void (*functions_array[Number_Of_Functions])(int);

	for (; i < Number_Of_Functions; i++)
	{
		functions_array[i] = Nothingness;
	}
	
	functions_array[ESC] = ExitFunctionAndEnableTerminal;
	functions_array[A] = PressKey;
	functions_array[T] = PressKey;

	while (1)
	{
		ch = getchar();
		functions_array[ch](ch);
	}

}


