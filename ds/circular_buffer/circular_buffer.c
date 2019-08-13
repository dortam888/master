#include "circular_buffer.h"
#include <stdlib.h> /* malloc free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#define INITIAL_VALUE (1)
#define MIN(a,b) ((a) < (b)) ? (a) : (b)


#pragma pack(1)
struct c_buff
{
	void *end;
	void *read;
	size_t bytes_left_to_read;
	char circular_buffer_start[INITIAL_VALUE];
};

c_buff_t *CBuffCreate(size_t capacity)
{
	c_buff_t *circ_buff = malloc(capacity + sizeof(c_buff_t) - INITIAL_VALUE);

	if (NULL != circ_buff)
	{
		circ_buff->end = circ_buff->circular_buffer_start + capacity;
		circ_buff->read = circ_buff->circular_buffer_start;
		circ_buff->bytes_left_to_read = 0;
	}

	return circ_buff;
}

void CBuffDestroy(c_buff_t *c_buff)
{
	free(c_buff);
	c_buff = NULL;
}

size_t CBuffCapacity(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return ((char *)c_buff->end - c_buff->circular_buffer_start);
}

size_t CBuffFreeSpace(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return CBuffCapacity(c_buff) - c_buff->bytes_left_to_read;
}

ssize_t CBuffRead(c_buff_t *c_buff, void *buffer, size_t number_of_bytes)
{
	int return_value = 0;
	size_t number_of_bytes_to_read = 0;

	assert(NULL != buffer && NULL != c_buff);

	/* number_of_bytes is what requested bytes_left_to_read is what i can read */
	number_of_bytes_to_read = MIN(number_of_bytes, c_buff->bytes_left_to_read);

	c_buff->bytes_left_to_read -= number_of_bytes_to_read; /* update how much i can read */

	/* check if we read over end if not copy in one step if yes read in 2 steps */
	if ((char *)c_buff->end >= 
		(char *)c_buff->read + number_of_bytes_to_read)
	{
		memcpy(buffer, c_buff->read, number_of_bytes_to_read);
		c_buff->read = (char *)c_buff->read + number_of_bytes_to_read;
	}
	else
	{
		size_t end_to_read = (size_t)c_buff->end - 
							 (size_t)c_buff->read;
		/* step 1 read till the end */
		memcpy(buffer, c_buff->read, end_to_read); 
		number_of_bytes_to_read -= end_to_read; /*update number of bytes to read */

		/* step 2 from beginning till bytes to cpy ends */
		memcpy((char *)buffer + end_to_read, 
				c_buff->circular_buffer_start, number_of_bytes_to_read); 
		c_buff->read = c_buff->circular_buffer_start + 
					   number_of_bytes_to_read;
	}

	return_value = number_of_bytes_to_read; /* update how much bytes were read */

	return return_value;
}

ssize_t CBuffWrite(c_buff_t *c_buff, void *data ,size_t number_of_bytes)
{
	size_t number_of_bytes_to_write = 0;
	void *write_ptr = NULL;

	assert(NULL != c_buff && NULL != data);

	number_of_bytes_to_write = MIN(number_of_bytes, CBuffFreeSpace(c_buff));

	
	write_ptr = (char *)c_buff->read + c_buff->bytes_left_to_read;

	/* check from where we should write */
	if ((char *)write_ptr > (char *)c_buff->end)
	{
		write_ptr = c_buff->circular_buffer_start + (size_t)c_buff->read + 
					c_buff->bytes_left_to_read - (size_t)c_buff->end;
	}

	/* update how much bytes i can read after write */
	c_buff->bytes_left_to_read += number_of_bytes_to_write;

	/* check if we should write over the end if not simply copy the data if yes copy in two steps */
	if ((char *)c_buff->end >= (char *)write_ptr + number_of_bytes_to_write)
	{
		memcpy(write_ptr, data, number_of_bytes_to_write); /* copy data */
	}
	else
	{
		size_t end_to_write = (size_t)c_buff->end - 
							  (size_t)write_ptr;
		memcpy(write_ptr, data, end_to_write); /* step one from start of write to end */
		number_of_bytes_to_write -= end_to_write;
		memcpy(c_buff->circular_buffer_start, (char *)data + end_to_write, 
			   number_of_bytes_to_write); /* step two from start to how much bytes left to copy */
	}

	return number_of_bytes_to_write;
}

int CBuffIsEmpty(const c_buff_t *c_buff)
{
	assert(NULL != c_buff);

	return (!(c_buff->bytes_left_to_read));
}
