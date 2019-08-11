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
	size_t write_to_read;
	char circular_buffer[INITIAL_VALUE];
};

c_buff_t *CBuffCreate(size_t capacity)
{
	c_buff_t *circ_buff = malloc(capacity + sizeof(c_buff_t) - INITIAL_VALUE);

	if (NULL != circ_buff)
	{
		circ_buff -> end = circ_buff -> circular_buffer + capacity;
		circ_buff -> read = circ_buff -> circular_buffer;
		circ_buff -> write_to_read = 0;
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
	return ((char *)c_buff -> end - c_buff -> circular_buffer);
}

size_t CBuffFreeSpace(const c_buff_t *c_buff)
{
	return c_buff -> write_to_read;
}

ssize_t CBuffRead(c_buff_t *c_buff, void *buffer, size_t number_of_bytes)
{
	int return_value = 0;
	size_t number_of_bytes_to_cpy = MIN(number_of_bytes, CBuffFreeSpace(c_buff));

	if (0 == number_of_bytes_to_cpy)
	{
		return_value = -1;
	}
	else
	{
		c_buff -> write_to_read -= number_of_bytes_to_cpy;
		return_value = number_of_bytes_to_cpy;

		if ((char *)c_buff -> end >= 
			(char *)c_buff -> read + number_of_bytes_to_cpy)
		{
			memcpy(buffer, c_buff -> read, number_of_bytes_to_cpy);
			c_buff -> read = (char *)c_buff -> read + number_of_bytes_to_cpy;
		}
		else
		{
			size_t end_to_read = (size_t)c_buff -> end - 
								 (size_t)c_buff -> read;
			memcpy(buffer, c_buff -> read, end_to_read);
			number_of_bytes_to_cpy -= end_to_read;
			memcpy((char *)buffer + end_to_read, 
				   c_buff -> circular_buffer, number_of_bytes_to_cpy);
			c_buff -> read = c_buff -> circular_buffer + number_of_bytes_to_cpy;
		}
	}

	return return_value;
}

ssize_t CBuffWrite(c_buff_t *c_buff, void *data ,size_t number_of_bytes)
{
	size_t number_of_bytes_to_write = MIN(number_of_bytes, 
		   CBuffCapacity(c_buff) - c_buff -> write_to_read);
	void *write_ptr = NULL;

	if ((char *)c_buff -> read + c_buff -> write_to_read <= (char *)c_buff -> end)
	{
		write_ptr = (char *)c_buff -> read + c_buff -> write_to_read;
	}
	else
	{
		write_ptr = c_buff -> circular_buffer + (size_t)c_buff -> read + 
					c_buff -> write_to_read - (size_t)c_buff -> end;
	}

	c_buff -> write_to_read += number_of_bytes_to_write;

	if ((char *)c_buff -> end >= (char *)write_ptr + number_of_bytes_to_write)
	{
		memcpy(write_ptr, data, number_of_bytes_to_write);
	}
	else
	{
		size_t end_to_write = (size_t)c_buff -> end - 
							  (size_t)write_ptr;
		memcpy(write_ptr, data, end_to_write);
		number_of_bytes_to_write -= end_to_write;
		memcpy(c_buff -> circular_buffer, (char *)data + end_to_write, 
			   number_of_bytes_to_write);
	}

	return number_of_bytes_to_write;
}

int CBuffIsEmpty(const c_buff_t *c_buff)
{
	return (!(c_buff -> write_to_read));
}
