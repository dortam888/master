#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include "bit_array.h"

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n", #function)
#define SIZE_OF_WORD sizeof(size_t)

void Test_BitArrSetOn()
{
	bit_array_t bit_array = 0;
	size_t i = 0;
	size_t mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSetOn(bit_array, i);

		if ((mask << i) != ((bit_array & (mask << i))))
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
	size_t mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSetOff(bit_array, i);

		if (0 != (bit_array & (mask << i)))
		{
			FAIL(SetOff);
			return;
		}

		bit_array = 0xffffffffffffffff;
	}

	PASS(SetOff);
}


void Test_BitArrSet()
{
	bit_array_t bit_array = 0xffffffffffffffff;
	size_t i = 0;
	size_t mask = 1;

	for (i = 0; i < SIZE_OF_WORD * __CHAR_BIT__; ++i)
	{
		bit_array = BitArrSet(bit_array, i, i % 2);

		if ((i % 2) != ((bit_array & (mask << i) >> i)))
		{
			FAIL(Set);
			return;
		}

		bit_array = 0xffffffffffffffff;
	}

	PASS(Set);
}


void Test_BitArrSetAll()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 0xfffffffffffffffe;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrSetAll(bit_array) == bit_array3) && (BitArrSetAll(bit_array2) == bit_array3))
	{
		PASS(SetAll);
	}
	else
	{
		FAIL(SetAll);
	}
}

void Test_BitArrResetAll()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 0xfffffffffffffffe;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrResetAll(bit_array3) == bit_array) && (BitArrResetAll(bit_array2) == bit_array))
	{
		PASS(ResetAll);
	}
	else
	{
		FAIL(ResetAll);
	}
}

void Test_BitArrFlipBit()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 5226;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrFlipBit(bit_array) == ~bit_array) && 
		(BitArrFlipBit(bit_array2) == ~bit_array2) &&
		(BitArrFlipBit(bit_array3) == ~bit_array3))
	{
		PASS(FlipBit);
	}
	else
	{
		FAIL(FlipBit);
	}
}


void Test_BitArrMirror()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrMirror(bit_array) == bit_array) && 
		(BitArrMirror(bit_array3) == bit_array3))
	{
		PASS(BitMirror);
	}
	else
	{
		FAIL(BitMirror);
	}
}

void Test_BitArrRotateLeft()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrMirror(bit_array) == bit_array) && 
		(BitArrMirror(bit_array3) == bit_array3))
	{
		PASS(RotateLeft);
	}
	else
	{
		FAIL(RotateLeft);
	}
}


void Test_BitArrRotateRight()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrMirror(bit_array) == bit_array) && 
		(BitArrMirror(bit_array3) == bit_array3))
	{
		PASS(RotateRight);
	}
	else
	{
		FAIL(RotateRight);
	}
}


void Test_BitArrIsBitOn()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 4;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if (!(BitArrIsBitOn(bit_array, 5)) && 
		(BitArrIsBitOn(bit_array2, 3)) &&
		(BitArrIsBitOn(bit_array3, 63)))
	{
		PASS(IsBitOn);
	}
	else
	{
		FAIL(IsBitOn);
	}
}


void Test_BitArrIsBitsOff()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 4;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrIsBitsOff(bit_array, 5)) && 
		(!BitArrIsBitsOff(bit_array2, 3)) &&
		(!BitArrIsBitsOff(bit_array3, 63)))
	{
		PASS(IsBitOff);
	}
	else
	{
		FAIL(IsBitOff);
	}
}


void Test_BitArrAreAllBitsOn()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 0xfffffffffffffffe;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((!BitArrAreAllBitsOn(bit_array)) && 
		(!BitArrAreAllBitsOn(bit_array2)) &&
		(BitArrAreAllBitsOn(bit_array3)))
	{
		PASS(AreAllBitsOn);
	}
	else
	{
		FAIL(AreAllBitsOn);
	}
}


void Test_BitArrAreAllBitsOff()
{
	bit_array_t bit_array = 0;
	bit_array_t bit_array2 = 0xfffffffffffffffe;
	bit_array_t bit_array3 = 0xffffffffffffffff;

	if ((BitArrAreAllBitsOff(bit_array)) && 
		(!BitArrAreAllBitsOff(bit_array2)) &&
		(!BitArrAreAllBitsOff(bit_array3)))
	{
		PASS(AreAllBitsOff);
	}
	else
	{
		FAIL(AreAllBitsOff);
	}
}


void Test_BitArrCountBitsOn()
{
	bit_array_t bit_array[] = {0, 0xfffffffffffffffe, 0xffffffffffffffff, 5, 7};
	size_t expected[] = {0, 64, 63, 2, 3};
	size_t num_of_tests = sizeof(bit_array)/sizeof(bit_array[0]);
	size_t i = 0;

	for (i = 0; i < num_of_tests; ++i)
	{
		if (BitArrCountBitsOn(bit_array[i]) == expected[i])
		{
			PASS(CountBitsOn);
		}
		else
		{
			FAIL(CountBitsOn);
		}
	}
}


void Test_BitArrCountBitsOff()
{
	bit_array_t bit_array[] = {0LU, 0xfffffffffffffffeLU, 0xffffffffffffffffLU};
	size_t expected[] = {64, 0, 1};
	size_t num_of_tests = sizeof(bit_array)/sizeof(bit_array[0]);
	size_t i = 0;

	for (i = 0; i < num_of_tests; ++i)
	{
		if (BitArrCountBitsOff(bit_array[i]) == expected[i])
		{
			PASS(CountBitsOff);
		}
		else
		{
			FAIL(CountBitsOff);
		}
	}
}


void Test_BitArrToString()
{
	bit_array_t bit_array[] = {0LU, 0xfffffffffffffffeLU, 0xffffffffffffffffLU, 5LU};
	char *expected[] = {"0",
						"1111111111111111111111111111111111111111111111111111111111111110",
						"1111111111111111111111111111111111111111111111111111111111111111",
						"101"};
	char buffer[64];
	size_t num_of_tests = sizeof(bit_array)/sizeof(bit_array[0]);
	size_t i = 0;

	for (i = 0; i < num_of_tests; ++i)
	{
		if (0 == strcmp(BitArrToString(bit_array[i], buffer), expected[i]))
		{
			PASS(BitArrToString);
		}
		else
		{
			FAIL(BitArrToString);
		}
	}
}

int main()
{
	Test_BitArrSetOn();

	Test_BitArrSetOff();

	Test_BitArrSet();

	Test_BitArrSetAll();

	Test_BitArrResetAll();

	Test_BitArrFlipBit();

	return 0;
}


