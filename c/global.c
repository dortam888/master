#include <stdio.h>

int foo();

int x = 3;

int main()
{
	return foo();
}

int foo()
{
	int y = 0;
	x = 5;
	x *= y;
	return x;
}

