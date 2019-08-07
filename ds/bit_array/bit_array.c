#include "bit_array.h"
#define NUMBER_OF_BIT_POSSIBILITIES_IN_NIBBLE 16
#define BITS_IN_NIBBLE 4
#define BITS_IN_WORD (sizeof(size_t) * __CHAR_BIT__)
#define BINARY_BASE 2

enum {FALSE, TRUE};

bit_array_t BitArrSetOn(bit_array_t bit_array, size_t index)
{
	size_t mask = 1LU; 

	return (bit_array | (mask << index));
}


bit_array_t BitArrSetOff(bit_array_t bit_array, size_t index)
{
	size_t mask = 1LU; 

	return (bit_array & ~(mask << index));
}


bit_array_t BitArrSet(bit_array_t bit_array, size_t index, bit_status_t status)
{
	switch (status)
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
	bit_array = ~(0x0LU);
	return bit_array;
}


bit_array_t BitArrResetAll(bit_array_t bit_array)
{
	bit_array = 0LU;
	return bit_array;
}

bit_array_t BitArrFlipBit(bit_array_t bit_array)
{
	return (~bit_array);
}


bit_array_t BitArrMirror(bit_array_t bit_array)
{
	size_t i = 0;
	size_t mirror_bit_array = 0LU;
	size_t mask = 1LU;

	for (i = 0; i < BITS_IN_WORD; ++i)
	{
		mirror_bit_array <<= 1;
		mirror_bit_array |= (bit_array & mask);
		bit_array >>= 1;
	}

	return mirror_bit_array;
}


bit_array_t BitArrMirrorLUT(bit_array_t bit_array)
{

	bit_array_t mirror_nibble_lut[NUMBER_OF_BIT_POSSIBILITIES_IN_NIBBLE] = 
													{0, 8, 4, 12, 2, 10, 6, 14,
													 1, 9, 5, 13, 3, 11, 7, 15};
	unsigned char mask = 0xf; /*1111 - all on in nibble*/
	unsigned char nibble_mirror = 0;
	bit_array_t mirror_bit_array = 0LU;
	size_t i = 0;

	for(i = 0; i < (BITS_IN_WORD / BITS_IN_NIBBLE); ++i)
	{
		mirror_bit_array <<= BITS_IN_NIBBLE;
		nibble_mirror = bit_array & mask; 
		nibble_mirror = bit_lut[nibble_mirror];
		mirror_bit_array |= nibble_mirror;
		bit_array >>= BITS_IN_NIBBLE;
	}

	return mirror_bit_array;
}


bit_array_t BitArrRotateLeft(bit_array_t bit_array, size_t number_of_rotates)
{
	size_t num_of_rotates = number_of_rotates % BITS_IN_WORD; /* optimize */

	return ((bit_array << num_of_rotates) | bit_array >> 
            (BITS_IN_WORD - num_of_rotates));
}

bit_array_t BitArrRotateRight(bit_array_t bit_array, size_t number_of_rotates)
{
	size_t num_of_rotates = number_of_rotates % BITS_IN_WORD; /* optimize */

	return ((bit_array >> num_of_rotates) | bit_array << 
            (BITS_IN_WORD - num_of_rotates));

}

int BitArrIsBitOn(bit_array_t bit_array, size_t index)
{
	size_t mask = 1LU;

	return (((bit_array >> index) & mask) == ON) ? TRUE : FALSE;
}

int BitArrIsBitOff(bit_array_t bit_array, size_t index)
{
	size_t mask = 1LU; 

	return (((bit_array >> index) & mask) == OFF) ? TRUE : FALSE;
}

int BitArrAreAllBitsOn(bit_array_t bit_array)
{
	return (bit_array == ~(0x0LU)) ? TRUE : FALSE;
}

int BitArrAreAllBitsOff(bit_array_t bit_array)
{
	return (bit_array == 0x0LU) ? TRUE : FALSE;
}

size_t BitArrCountBitsOn(bit_array_t bit_array)
{
	size_t bits_on_counter = 0; 

	while (0 != bit_array)
	{
		++bits_on_counter;
		bit_array &= (bit_array - 1);
	}

	return bits_on_counter;
}


size_t BitArrCountBitsOnLUT(bit_array_t bit_array)
{s

	unsigned char bits_on_lut[NUMBER_OF_BIT_POSSIBILITIES_IN_NIBBLE] = 
													{0, 1, 1, 2, 1, 2, 2, 3, 
													 1, 2, 2, 3, 2, 3, 3, 4};
	unsigned char mask = 0xf;
	size_t bits_on_counter = 0;
	size_t i = 0;
	size_t nibble_in_word = BITS_IN_WORD / BITS_IN_NIBBLE;

	for (i = 0; i < nibble_in_word; ++i)
	{
		bits_on_counter += nibble_lut[bit_array & mask];
		bit_array >>= BITS_IN_NIBBLE;
	}

	return bits_on_counter;
}


size_t BitArrCountBitsOff(bit_array_t bit_array)
{
	size_t count = BITS_IN_WORD;

	while (0 != bit_array)
	{
		--count;
		bit_array &= (bit_array - 1);
	}

	return count;
}


char *BitArrToString(bit_array_t bit_array, char *string)
{
	char *start_of_string = string;
	size_t i = 0;

	assert(NULL != string);

	string += BITS_IN_WORD;
	*string = '\0';
	--string;

	for (i = 0; i < BITS_IN_WORD; ++i)
	{
		*string = bit_array % BINARY_BASE + '0';
		--string;
		bit_array /= BINARY_BASE;
	}

	return start_of_string;
}
