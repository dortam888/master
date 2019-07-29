#include <stdio.h> /* printf */
#include <string.h> /* memcpy, memset */
#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#define WORDSIZE sizeof(size_t)
#define BITS_IN_BYTE 8

void *My_memcpy_ByteByByte(void *dest, const void *src, size_t number_of_bits);
void *My_memcpy(void *dest, const void *src, size_t number_of_bits);
void Test_My_memcpy();


int main()
{
	Test_My_memcpy();
	return 0;
}


void *My_memcpy_ByteByByte(void *dest, const void *src, size_t number_of_bits)
{
	void *start_of_dest = dest;
	size_t i = 0;

	assert(NULL != dest && NULL != src);

	for (i = 0; i < number_of_bits; ++i)
	{
		*(char *)dest = *(const char *)src;
		dest = (char *)dest + 1;
		src = (const char *)src + 1;
	}

	return start_of_dest;
}

void *My_memcpy(void *dest, const void *src, size_t number_of_bits)
{
	void *start_of_dest = dest;
	size_t first_tail = 0;
	size_t *dest_st_ptr = NULL;
	size_t *src_st_ptr = NULL;

	assert(NULL != dest && NULL != src);

	first_tail = (size_t)src % WORDSIZE;
	My_memcpy_ByteByByte(dest, src, first_tail);
	dest = (char *)dest + first_tail;
	src = (const char *)src + first_tail;
	number_of_bits -= first_tail;

	dest_st_ptr = (size_t *)dest;
	src_st_ptr = (size_t *)src;

	while (number_of_bits >= WORDSIZE)
	{
		*dest_st_ptr++ = *src_st_ptr++;
		number_of_bits -= WORDSIZE;
	}

	My_memcpy_ByteByByte(dest_st_ptr, src_st_ptr, number_of_bits);

	return start_of_dest;
}


void *My_memset_ByteByByte(void *s, int c, size_t number_of_bits)
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


void *My_memset(void *s, int c, size_t number_of_bits)
{
	void *start_of_s = s;
	size_t word_c = (size_t)c;
	size_t *s_st_ptr = NULL;
	size_t i = 0;

	assert(NULL != s);

	while (0 != (size_t)s % WORDSIZE && 0 != number_of_bits)
	{
		*(char *)s = c;
		s = (char *)s + 1;
		--number_of_bits;
	}

	for (i = 1; i < WORDSIZE; ++i)
	{
		word_c = word_c << 1 + word_c & 1;
	}

	s_st_ptr = (size_t *)s;
	

	return start_of_s;
}


void Test_My_memcpy()
{
	char dest[50];
	const char *src = "more than more than";

	My_memcpy(dest, src, 18);

	printf("%s\n", dest);
}
