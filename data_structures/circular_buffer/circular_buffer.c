/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 15/08/2019
**** Reviewer: Nitzan
**** Description: This file contains the implementations of functions
				  for the data structure circular buffer.
				  Look at circular_buffer.h for more information about the 
				  functions.
*******************************************************************************/

#include <stdlib.h> /* malloc free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "circular_buffer.h" /* c_buff_t */

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

struct c_buff
{
	char *end; /* fixed pointer to the end of the buffer. char * for pointer arithematic consideration*/
	char *read; /* pointer that indicates the last byte that was read */
	size_t written_bytes_not_read; /* how many bytes were written and weren't been read*/
	char circular_buffer_start[1]; /* fixed pointer to the start of the circ_buffer. */
};

c_buff_t *CBuffCreate(size_t capacity)
{
	c_buff_t *circ_buff = malloc(capacity + offsetof(c_buff_t, 
													 circular_buffer_start);
	if (NULL == circ_buff)
	{
		return NULL;
	}

	circ_buff->end = circ_buff->circular_buffer_start + capacity;
	circ_buff->read = circ_buff->circular_buffer_start;
	circ_buff->written_bytes_not_read = 0;

	return circ_buff;
}

void CBuffDestroy(c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	c_buff->end = NULL;
	c_buff->read = NULL;

	free(c_buff);
	c_buff = NULL;
}

size_t CBuffCapacity(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return (c_buff->end - c_buff->circular_buffer_start);
}

size_t CBuffFreeSpace(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return (CBuffCapacity(c_buff) - c_buff->written_bytes_not_read);
}

static int IsEndShouldPassed(void *end, void *read_or_write, 
							 size_t number_of_bytes_of_action)
{
	return !((char *)end >= (char *)read_or_write + number_of_bytes_of_action);
}

ssize_t CBuffRead(c_buff_t *c_buff, void *buffer, size_t number_of_bytes)
{
	size_t actual_number_of_bytes_to_read = 0;

	assert(NULL != buffer);
	assert(NULL != c_buff);

	/* take the minimum from 9 */
	actual_number_of_bytes_to_read = MIN(number_of_bytes, 
										 c_buff->written_bytes_not_read);

	c_buff->written_bytes_not_read -= actual_number_of_bytes_to_read; /* update how much i can read */

	/* check if we read over end if not copy in one step if yes read in 2 steps */
	if (!IsEndShouldPassed(c_buff->end, c_buff->read, 
						   actual_number_of_bytes_to_read))
	{
		memcpy(buffer, c_buff->read, actual_number_of_bytes_to_read);
		c_buff->read = c_buff->read + actual_number_of_bytes_to_read;
	}
	else
	{
		size_t end_to_read = (size_t)c_buff->end - 
							 (size_t)c_buff->read;
		/* step 1 read till the end */
		memcpy(buffer, c_buff->read, end_to_read); 
		actual_number_of_bytes_to_read -= end_to_read; /*update number of bytes to read */

		/* step 2 from beginning till bytes to cpy ends */
		memcpy((char *)buffer + end_to_read, 
				c_buff->circular_buffer_start, actual_number_of_bytes_to_read); 
		c_buff->read = c_buff->circular_buffer_start + 
					   actual_number_of_bytes_to_read;
	}

	return actual_number_of_bytes_to_read;
}

ssize_t CBuffWrite(c_buff_t *c_buff, void *data ,size_t number_of_bytes)
{
	size_t actual_number_of_bytes_to_write = 0;
	char *write_ptr = NULL; /* char * to avoid casting */

	assert(NULL != c_buff && NULL != data);

	actual_number_of_bytes_to_write = MIN(number_of_bytes, 
										  CBuffFreeSpace(c_buff));

	write_ptr = c_buff->read + c_buff->written_bytes_not_read;

	/* check from where we should write */
	if (write_ptr > c_buff->end)
	{
		write_ptr = c_buff->circular_buffer_start + (size_t)c_buff->read + 
					c_buff->written_bytes_not_read - (size_t)c_buff->end;
	}

	/* update how much bytes i can read after write */
	c_buff->written_bytes_not_read += actual_number_of_bytes_to_write;

	/* check if we should write over the end if not simply copy the data if yes copy in two steps */
	if (!IsEndShouldPassed(c_buff->end, write_ptr, 
						   actual_number_of_bytes_to_write))
	{
		memcpy(write_ptr, data, actual_number_of_bytes_to_write); /* copy data */
	}
	else
	{
		size_t end_to_write = (size_t)c_buff->end - (size_t)write_ptr;
		memcpy(write_ptr, data, end_to_write); /* step one cpy from start of write to end */
		actual_number_of_bytes_to_write -= end_to_write;
		memcpy(c_buff->circular_buffer_start, (char *)data + end_to_write, 
			   actual_number_of_bytes_to_write); /* step two cpy from start bytes left to copy */
	}

	return actual_number_of_bytes_to_write;
}

int CBuffIsEmpty(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return (!(c_buff->written_bytes_not_read));
}
