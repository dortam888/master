#include <stdio.h> /* printf */
#include <string.h> /* memcpy, memset */
#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#define WORDSIZE 8

int main()
{
	return 0;
}


void *My_memcpy(void *dest, const void *src, size_t number_of_bits)
{
	void *start_of_dest = dest;
	size_t i = 0;

	assert(NULL != dest && NULL != src);

	for (i = 0; i < number_of_bits; ++i)
	{
		*(char *)dest = *(char *)src;
		dest = (char *)dest + 1;
		src = (char *)src + 1;
	}

	return start_of_dest;
}


void *My_memset(void *s, int c, size_t number_of_bits)
{
	void *start_of_s = s;
	size_t i = 0;

	assert(NULL != s);

	for (i = 0; i < number_of_bits; ++i)
	{
		*(char *)s = c;
		s = (char *)s + 1;
	}

	return start_of_s;
}


