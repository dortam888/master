#include <stdio.h> /* printf */
#include <string.h> /* memcpy, memset */
#include <stdlib.h> /* exit */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t ptrdiff_t*/

#define WORDSIZE (sizeof(size_t))
#define BITS_IN_BYTE (8)
#define NUMBER_OF_CHARS 256

typedef enum Overlap {NO_OVERLAP, RIGHT_OVERLAP, LEFT_OVERLAP} overlap_t;

void *My_memcpy_ByteByByte(void *dest, const void *src, size_t number_of_bits);
void *My_memcpy(void *dest, const void *src, size_t number_of_bits);
void Test_My_memcpy();
int My_atoibase(const char *str, int base);
void ThreeArrays(char *str1, char *str2, char *str3);


int main()
{
	/*Test_My_memcpy();*/
	ThreeArrays("abcde", "cdeft", "ftdyr");
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
		word_c = (word_c << BITS_IN_BYTE) | (word_c & 0xFF);
	}

	s_st_ptr = (size_t *)s;

	while (number_of_bits >= WORDSIZE)
	{
		*s_st_ptr++ = word_c;
		number_of_bits -= WORDSIZE;
	}

	s = s_st_ptr;

	while (0 != number_of_bits)
	{
		*(char *)s = c;
		s = (char *)s + 1;
		--number_of_bits;
	}

	return start_of_s;
}


static overlap_t CheckOverlap(const void *dest, const void *src, size_t n)
{
	if ((char *)dest - (char *)src > n || (char *)dest - (char *)src > n)
	{
		return NO_OVERLAP;
	}
	else if (dest > src)
	{
		return RIGHT_OVERLAP;
	}
	else
	{
		return LEFT_OVERLAP;
	}
}

void *My_memmove(void *dest, const void *src, size_t n)
{
	overlap_t overlap = NO_OVERLAP;

	if (src == dest)
	{
		return;
	}

	overlap = CheckOverlap(dest, src, n);

	switch(overlap)
	{
		case NO_OVERLAP:
			memcpy(dest, src, n);
			break;

		case RIGHT_OVERLAP:
			My_memcpy_Backward(dest, src, n);
			break;

		case LEFT_OVERLAP:
			My_memcpy_Forward(dest, src, n);
			break;
	}
}


int My_atoi10(const char *str)
{
	int result = 0;
	int sign = 1;

	assert(NULL != str);

	if ('-' == *str)
	{
		sign = -1;
		++str;
	}

	while ('\0' != *str)
	{
		if (*str - '0' < '\0' || *str - '0' > 10)	
		{
			break;
		}
		result = result * 10 + *str - '0';
		++str;
	}

	return result * sign;
}


int My_atoibase(const char *str, int base)
{
	int result = 0;

	assert(NULL != str);

	while ('\0' != *str)
	{
		if (*str - '0' < '\0')	
		{
			break;
		}

		if (*str - '\0' < 10)
		{
			result = result * base + *str - '0';
		}
		else
		{
			if (*str - '0' - 6 > base || *str - '0' - 6 < '\0')
			{
				printf("invalid number in base %d\n", base);
				exit(0);
			}
			result = result * base + *str - '0' - 7;
		}
		++str;
	}

	return result;
}


char *My_itoa10(int number)
{
	char *result = 0;
	char *start_of_result = result;

	if (number < 0)
	{
		*result = '-';
		number = -number;
		++result;
	}

	while (0 != number)
	{
		*result = number % 10 + '0';
		++result;
		number /= 10;
	}

	result = '\0';

	return start_of_result;
}


char *My_itoabase(int number, int base)
{
	char *result = 0;
	char *start_of_result = result;

	if (number < 0)
	{
		*result = '-';
		number = -number;
		++result;
	}

	while (0 != number)
	{
		*result = number % base + '0';
		++result;
		number /= base;
	}

	result = '\0';

	return start_of_result;
}


void ThreeArrays(char *str1, char *str2, char *str3)
{
	int i = 0;
	int char_array1[NUMBER_OF_CHARS];
	int char_array2[NUMBER_OF_CHARS];
	int char_array3[NUMBER_OF_CHARS];
	memset(char_array1, 0, NUMBER_OF_CHARS * sizeof(char_array1[0]));	
	memset(char_array2, 0, NUMBER_OF_CHARS * sizeof(char_array2[0]));
	memset(char_array3, 0, NUMBER_OF_CHARS * sizeof(char_array3[0]));

	while ('\0' != *str1)
	{
		char_array1[(int)*str1] |= 1;
		++str1;
	}

	while ('\0' != *str2)
	{
		char_array2[(int)*str2] |= 1;
		++str2;
	}

	while ('\0' != *str3)
	{
		char_array3[(int)*str3] |= 1;
		++str3;
	}

	for (i = 1; i < NUMBER_OF_CHARS; ++i)
	{
		if (char_array1[i] & char_array2[i] & ~char_array3[i])
		{
			printf("%c\n", i);
		}
	}
}


void Test_My_memcpy()
{
	char dest[50];
	const char *src = "more than more than";

	My_memcpy(dest, src, 18);

	printf("%s\n", dest);
}
