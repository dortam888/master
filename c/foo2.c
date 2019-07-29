#include <stdio.h>

int foo(int a, int b);
extern void bar();

int main()
{
	/*foo(3,6);*/
	bar();
	return 0;
}


int foo(int a, int b)
{
	int c = a*b;
	a = b = 0;
	return c;
}
