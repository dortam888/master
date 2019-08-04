
/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72*/
/* Last Update: 4/8/19 */
/* This file contains functions for bit array operation.*/
/* Can be used for attendence systems */
/*############################################################################*/

enum {OFF, ON};

/*#############################################################################
  # Description: 
	SetOn gets an array of bits and an index and set the bit on the specified
	index to 1
  # Parameters:
	bit array
	index
  # Return Value:
	Bit array with the given index being 1
  # Error:
	Index is bigger than available bits in bit array
##############################################################################*/
size_t SetOn(size_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	SetOff gets an array of bits and an index and set the bit on the specified
	index to 0
  # Parameters:
	bit array
	index
  # Return Value:
	Bit array with the given index being 0
  # Error:
	Index is bigger than available bits in bit array
##############################################################################*/
size_t SetOff(size_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	Set gets an array of bits, an index and status and set the bit on the specified
	index to status
  # Parameters:
	bit array
	index
	status
  # Return Value:
	Bit array with the given index being 0
  # Error:
	Index is bigger than available bits in bit array
##############################################################################*/
size_t Set(size_t bit_array, size_t index, int status);

/*#############################################################################
  # Description: 
	SetAll gets an array of bits set all it's bits to 1
  # Parameters:
	bit array
  # Return Value:
	Bit array with of 1's.
##############################################################################*/
size_t SetAll(size_t bit_array);

/*#############################################################################
  # Description: 
	ResetAll gets an array of bits set all it's bits to 0
  # Parameters:
	bit array
  # Return Value:
	Bit array with of 0's.
##############################################################################*/
size_t ResetALL(size_t bit_array);

/*#############################################################################
  # Description: 
	Flip gets an array of bits flip all it's bits
  # Parameters:
	bit array
  # Return Value:
	Flipped Bit array
##############################################################################*/
size_t Flip(size_t bit_array);

/*#############################################################################
  # Description: 
	Mirror gets an array of bits and mirror all it's bits
  # Parameters:
	bit array
  # Return Value:
	Mirrored Bit array
##############################################################################*/
size_t Mirror(size_t bit_array);

/*#############################################################################
  # Description: 
	RotateLeft get an array of bits and rotate it's bits left specified number
	of times
  # Parameters:
	bit array
	number of rotates
  # Return Value:
	Rotated Bit array by number_of_rotates%sizeof(size_t))
##############################################################################*/
size_t RotateLeft(size_t bit_array, int number_of_rotates);

/*#############################################################################
  # Description: 
	RotateRight get an array of bits and rotate it's bits right specified number
	of times
  # Parameters:
	bit array
	number of rotates
  # Return Value:
	Rotated Bit array by number_of_rotates%sizeof(size_t))
##############################################################################*/
size_t RotateRight(size_t bit_array, int number_of_rotates);

/*#############################################################################
  # Description: 
	IsOn gets an array of bits and an index returns 1 if the bit is 1 and 0 otherwise
  # Parameters:
	bit array
	index
  # Return Value:
	1 or 0
  # Error:
	index is bigger than available bits in bit array
##############################################################################*/
int IsOn(size_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	IsOn gets an array of bits and an index returns 1 if the bit is 0 and 0 otherwise
  # Parameters:
	bit array
	index
  # Return Value:
	1 or 0
  # Error:
	index is bigger than available bits in bit array
##############################################################################*/
int IsOff(size_t bit_array, size_t index);

/*#############################################################################
  # Description: 
	IsAllOn gets an array of bits and returns 1 if all bits are 1.
  # Parameters:
	bit array
  # Return Value:
	1 or 0
##############################################################################*/
int IsAllOn(size_t bit_array);

/*#############################################################################
  # Description: 
	IsAllOff gets an array of bits and returns 1 if all bits are 0.
  # Parameters:
	bit array
##############################################################################*/
int IsAllOff(size_t bit_array);

/*#############################################################################
  # Description: 
	CountOn gets an array of bits and counts all the bits that are 1.
  # Parameters:
	bit array
  # Return Value:
	number of bits that are 1
##############################################################################*/
int CountOn(size_t bit_array);

/*#############################################################################
  # Description: 
	CountOff gets an array of bits and counts all the bits that are 0.
  # Parameters:
	bit array
  # Return Value:
	number of bits that are 0
##############################################################################*/
int CountOff(size_t bit_array);

/*#############################################################################
  # Description: 
	ToString gets an array of bits and an adress of string that will contain the
	bit of array as a string
  # Parameters:
	bit array
  # Return Value:
	None
##############################################################################*/
void ToString(size_t bit_array, char *string)

/* End Of File */
