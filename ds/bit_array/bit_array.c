#include "bit_array.h"

bit_array_t BitArrSetOn(bit_array_t bit_array, size_t index)
{
	size_t mask = 1;

	return (bit_array | (mask << index));
}

bit_array_t BitArrSetOff(bit_array_t bit_array, size_t index)
{
	size_t mask = 1;

	return (bit_array & ~(mask << index));
}

bit_array_t BitArrSet(bit_array_t bit_array, size_t index, bit_status_t status)
{
	switch(status)
	{
	case ON:
			bit_array = BitArrSetOn(bit_array, index);	
			break;
	case OFF:
			bit_array = BitArrSetOff(bit_array, index);	
			break;
	}

	return bit_array;
}

bit_array_t BitArrSetAll(bit_array_t bit_array)
{
	return 0xffffffffffffffffLU;
}

bit_array_t BitArrResetAll(bit_array_t bit_array)
{
	return 0LU;
}

bit_array_t BitArrFlipBit(bit_array_t bit_array)
{
	return (~bit_array);
}

bit_array_t BitArrMirror(bit_array_t bit_array)
{
	return 0;
}

bit_array_t BitArrRotateLeft(bit_array_t bit_array, size_t number_of_rotates)
{
	return 0;
}

bit_array_t BitArrRotateRight(bit_array_t bit_array, size_t number_of_rotates)
{
	return 0;
}

int BitArrIsBitOn(bit_array_t bit_array, size_t index)
{
	return 0;
}

int BitArrIsBitsOff(bit_array_t bit_array, size_t index)
{
	return 0;
}

int BitArrAreAllBitsOn(bit_array_t bit_array)
{
	return 0;
}

int BitArrAreAllBitsOff(bit_array_t bit_array)
{
	return 0;
}

size_t BitArrCountBitsOn(bit_array_t bit_array)
{
	return 0;
}

size_t BitArrCountBitsOff(bit_array_t bit_array)
{
	return 0;
}

char *BitArrToString(bit_array_t bit_array, char *string)
{
	return NULL;
}
