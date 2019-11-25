#ifndef ILRD_BIT_ARRAY_H
#define ILRD_BIT_ARRAY_H
#include <stdlib.h>


/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72*/
/* Last Update: 4/8/19 */
/* This file contains functions for bit array operation.*/
/* Can be used for attendence systems */
/*############################################################################*/

typedef enum status {OFF, ON} bit_status_t;
typedef size_t bit_array_t;

/*#############################################################################
  # Description: 
	gets an array of bits and an index and set the bit on the specified
	index to 1
  # Parameters:
	bit array
	index inside the bit array
  # Return Value:
	Bit array with the given index being 1
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
bit_array_t BitArrSetOn(bit_array_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	gets an array of bits and an index and set the bit on the specified
	index to 0
  # Parameters:
	bit array
	index inside the bit array
  # Return Value:
	Bit array with the given index being 0
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
bit_array_t BitArrSetOff(bit_array_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	gets an array of bits, an index and status and set the bit on the specified
	index to status
  # Parameters:
	bit array
	index inside the bit array
	status that needs to be updated: ON/OFF
  # Return Value:
	Bit array with the given index being status
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
bit_array_t BitArrSet(bit_array_t bit_array, size_t index, bit_status_t status);

/*#############################################################################
  # Description: 
	gets an array of bits set all it's bits to 1
  # Parameters:
	bit array
  # Return Value:
	Bit array with of 1's.
##############################################################################*/
bit_array_t BitArrSetAll(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits set all it's bits to 0
  # Parameters:
	bit array
  # Return Value:
	Bit array with 0's.
##############################################################################*/
bit_array_t BitArrResetAll(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits and flip all it's bits
  # Parameters:
	bit array
  # Return Value:
	Flipped Bit array #01001011# -> #10110100#
##############################################################################*/
bit_array_t BitArrFlipBit(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits and mirror all it's bits
  # Parameters:
	bit array
  # Return Value:
	Mirrored Bit array #10110011# -> #11001101#
##############################################################################*/
bit_array_t BitArrMirror(bit_array_t bit_array);


/*#############################################################################
  # Description: 
	gets an array of bits and mirror all it's bits
  # Parameters:
	bit array
  # Return Value:
	Mirrored Bit array #10110011# -> #11001101#
##############################################################################*/
bit_array_t BitArrMirrorLUT(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	get an array of bits and rotate it's bits left specified number of times
  # Parameters:
	bit array
	number of rotations for the bit array
  # Return Value:
	Rotated Bit array by number of rotations
##############################################################################*/
bit_array_t BitArrRotateLeft(bit_array_t bit_array, size_t number_of_rotates);

/*#############################################################################
  # Description: 
	get an array of bits and rotate it's bits right specified number of times
  # Parameters:
	bit array
	number of rotations for the bit array
  # Return Value:
	Rotated Bit array by number of rotations
##############################################################################*/
bit_array_t BitArrRotateRight(bit_array_t bit_array, size_t number_of_rotates);

/*#############################################################################
  # Description: 
	gets an array of bits and an index returns 1 if the bit is 1 and 0 otherwise
  # Parameters:
	bit array
	index inside the bit array
  # Return Value:
	1 or 0
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
int BitArrIsBitOn(bit_array_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	gets an array of bits and an index returns 1 if the bit is 0 and 0 otherwise
  # Parameters:
	bit array
	index inside the bit array
  # Return Value:
	1 or 0
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
int BitArrIsBitOff(bit_array_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	gets an array of bits and returns 1 if all bits are 1.
  # Parameters:
	bit array
  # Return Value:
	1 or 0
	If Index is bigger than available bits in bit array - undefined behavior
##############################################################################*/
int BitArrAreAllBitsOn(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits and returns 1 if all bits are 0 and 0 otherwise.
  # Parameters:
	bit array
 # Return Value:
	1 or 0
##############################################################################*/
int BitArrAreAllBitsOff(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits and counts all the bits that are 1.
  # Parameters:
	bit array
  # Return Value:
	number of bits that are 1
##############################################################################*/
size_t BitArrCountBitsOn(bit_array_t bit_array);


/*#############################################################################
  # Description: 
	gets an array of bits and counts all the bits that are 1.
  # Parameters:
	bit array
  # Return Value:
	number of bits that are 1
##############################################################################*/
size_t BitArrCountBitsOnLUT(bit_array_t bit_array);


/*#############################################################################
  # Description: 
	CountOff gets an array of bits and counts all the bits that are 0.
  # Parameters:
	bit array
  # Return Value:
	number of bits that are 0
##############################################################################*/
size_t BitArrCountBitsOff(bit_array_t bit_array);

/*#############################################################################
  # Description: 
	gets an array of bits and an adress of string that will contain the
	bit of array as a string
  # Parameters:
	bit array
	adress that will be able to contain string of the bit array
  # Return Value:
	adress that contains string of the bit array
##############################################################################*/
char *BitArrToString(bit_array_t bit_array, char *string);

#endif /* ILRD_BIT_ARRAY_H */

