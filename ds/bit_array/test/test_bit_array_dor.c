#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"

#define PASS(function) printf("\033[0;32m (function) \033[0m\n")
#define FAIL(function) printf("\033[0;31m (function) \033[0m\n")
#define SIZE_OF_WORD sizeof(size_t)

void Test_BitArrSetOn()
{
	bit_array_t bit_array = 0;
	size_t i = 0;
	int mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSetOn(bit_array, i);

		if (1 != (bit_array & (mask << i)))
		{
			FAIL(SetOn);
			return;
		}

		bit_array = 0;
	}

	PASS(SetOn);
}

void Test_BitArrSetOff()
{
	bit_array_t bit_array = 0xffffffffffffffff;
	size_t i = 0;
	int mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSetOff(bit_array, i);

		if (0 != (bit_array & (mask << i)))
		{
			FAIL(SetOn);
			return;
		}

		bit_array = 0xffffffffffffffff;
	}

	PASS(SetOn);
}


void Test_BitArrSet()
{
	bit_array_t bit_array = 0xffffffffffffffff;
	size_t i = 0;
	int mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSetOff(bit_array, i);

		if (0 != (bit_array & (mask << i)))
		{
			FAIL(SetOn);
			return;
		}

		bit_array = 0xffffffffffffffff;
	}

	PASS(SetOn);
}


void Test_BitArrSetAll()
{
}

void Test_BitArrResetAll()
{
}

void Test_BitArrFlipBit()
{
}


void Test_BitArrMirror()
{
}

void Test_BitArrRotateLeft()
{
}


void Test_BitArrRotateRight()
{
}


void Test_BitArrIsBitOn()
{
}


void Test_BitArrIsBitsOff()
{
}


void Test_BitArrAreAllBitsOn()
{
}


void Test_BitArrAreAllBitsOff()
{
}


void Test_BitArrCountBitsOn()
{
}


void Test_BitArrCountBitsOff()
{
}


void Test_BitArrToString()
{
}

int main()
{
	return 0;
}


