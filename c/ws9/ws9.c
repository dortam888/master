#include <stdio.h> /* printf */
#include <string.h> /* memcpy, memset */
#include <stdlib.h> /* exit */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t ptrdiff_t*/
#include <limits.h> /* INT_MAX */

#define WORDSIZE (sizeof(size_t))
#define BITS_IN_BYTE 8
#define NUMBER_OF_CHARS 256
#define MAX(a,b) ((a) > (b))? ((a) : (b))
#define MAX3(a,b,c) ((a) > (b))? (((a) > (c)? (a) : (c)) : ((b) > (c)? (b) : (c)))
#define CHECKENDIANESS ((short)"\x01\0" == (short)1)? printf("is little endian") : printf("is big endian")
#define SPACE ' '
#define TAB '\t'


enum {NEGATIVE = -1, POSITIVE = 1};
typedef enum Overlap {NO_OVERLAP, RIGHT_OVERLAP, LEFT_OVERLAP} overlap_t;

static void *My_memcpy_ByteByByte(void *dest, const void *src, 
								  size_t number_of_bits);
void *My_memcpy(void *dest, const void *src, size_t number_of_bits);
static void Test_My_memcpy();
static void Test_My_memset();
int My_atoibase(const char *str, int base);
char *My_itoabase(int number, int base, char *result);
void ThreeArrays(char *str1, char *str2, char *str3);
void CheckEndianess();
void Test_ThreeArrays();
void Test_My_atoibase();
void Test_My_itoabase();


int main()
{
	Test_My_memcpy();
	Test_My_memset();
	Test_My_atoibase();
	Test_My_itoabase();
	Test_ThreeArrays();
	CheckEndianess();
	/*CHECKENDIANESS;*/
	return 0;
}


static void *My_memcpy_ByteByByte(void *dest, const void *src, 
								  size_t number_of_bits)
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
	char *dest_char_ptr = (char *)dest;
	char *src_char_ptr = (char *)src;
	size_t *dest_st_ptr = NULL;
	size_t *src_st_ptr = NULL;

	assert(NULL != dest && NULL != src);

	while ((0 != (size_t)src % WORDSIZE) && (number_of_bits > 0))
	{
		*dest_char_ptr++ = *src_char_ptr++;
		--number_of_bits;
	}

	dest_st_ptr = (size_t *)dest_char_ptr;
	src_st_ptr = (size_t *)src_char_ptr;

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


void *My_memset(void *str, int c, size_t number_of_bits)
{
	void *start_of_str = str;
	size_t word_c = c;
	size_t *str_st_ptr = NULL;
	size_t i = 0;

	assert(NULL != str);

	while (0 != (size_t)str % WORDSIZE && 0 < number_of_bits)
	{
		*(char *)str = c;
		str = (char *)str + 1;
		--number_of_bits;
	}

	for (i = 1; i < WORDSIZE; ++i)
	{
		word_c = (word_c << BITS_IN_BYTE) | c;
	}

	str_st_ptr = (size_t *)str;

	while (number_of_bits >= WORDSIZE)
	{
		*str_st_ptr++ = word_c;
		number_of_bits -= WORDSIZE;
	}

	str = str_st_ptr;

	while (0 < number_of_bits)
	{
		*(char *)str = c;
		str = (char *)str + 1;
		--number_of_bits;
	}

	return start_of_str;
}


static overlap_t CheckOverlap(const void *dest, const void *src, size_t n)
{
	if ( ((char *)dest - (char *)src > (ptrdiff_t)n) || 
		 (((char *)dest - (char *)src) < (-(ptrdiff_t)n)) )
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


static void *My_memcpy_Backward(void *dest, const void *src, size_t number_of_bits)
{
	ptrdiff_t overlap = (char *)dest -(char *)src;
	size_t *dest_st_ptr = (size_t *)((char *)dest + number_of_bits);
	size_t *src_st_ptr = (size_t *)((char *)src + number_of_bits);
	char *dest_char_ptr = NULL;
	char *src_char_ptr = NULL;

	while (0 < (number_of_bits - overlap) / WORDSIZE)
	{
		*(dest_st_ptr - WORDSIZE) = *(src_st_ptr - WORDSIZE);
		--dest_st_ptr;
		--src_st_ptr;
		number_of_bits -= WORDSIZE;
	}

	dest_char_ptr = (char *)dest_st_ptr;
	src_char_ptr = (char *)src_st_ptr;

	while ( 0 < (number_of_bits - overlap) % WORDSIZE)
	{
		*dest_char_ptr = *src_char_ptr;
		--dest_char_ptr;
		--src_char_ptr;
		--number_of_bits;
	}

	memcpy(dest, src, number_of_bits);

	return dest;
}


void *My_memmove(void *dest, const void *src, size_t number_of_bits)
{
	overlap_t overlap = NO_OVERLAP;

	if (src == dest)
	{
		return dest;
	}

	overlap = CheckOverlap(dest, src, number_of_bits);

	switch(overlap)
	{
		case NO_OVERLAP:
			 memcpy(dest, src, number_of_bits);
			 break;

		case RIGHT_OVERLAP:
			 My_memcpy_Backward(dest, src, number_of_bits);
			 break;

		case LEFT_OVERLAP:
			 memcpy(dest, src, number_of_bits);
			 break;
	}

	return dest;
}


int My_atoi10(const char *str)
{
	int result = 0;
	int sign = POSITIVE;

	assert(NULL != str);

	if ('-' == *str)
	{
		sign = NEGATIVE;
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
	int sign = POSITIVE;

	assert(NULL != str);
	
	if ('-' == *str)
	{
		sign = NEGATIVE;
		++str;
	}

	while ('\0' != *str)
	{
		if (*str == SPACE || *str == TAB)
		{
			++str;
			continue;
		}
		
		if (*str - '0' < '\0' || ((*str - '0' >= base) && (*str < 'A')))	
		{
			break;
		}

		if (*str - '0' < 10)
		{
			result = result * base + *str - '0';
		}
		else
		{
			if (*str - '0' - 6 > base)
			{
				break;
			}
			result = result * base + *str - '0' - 7;
		}
		
		++str;
	}

	return result*sign;
}

size_t NumberOfDigitsInNum(int num, int base)
{
	size_t number_of_digits = 0;
	
	while (0 != num)
	{
		++number_of_digits;
		num /= base;
	}
	
	return number_of_digits;
}


char *My_itoa10(int number)
{
	char *result = 0;
	char *start_of_result = result;
	size_t num_of_digits_in_number = NumberOfDigitsInNum(number, 10);
	
	if (number < 0)
	{
		*result = '-';
		number = -number;
		++result;
	}

	result += num_of_digits_in_number;
	*result = '\0';
	--result;
	
	while (0 != number)
	{
		*result = number % 10 + '0';
		++result;
		number /= 10;
	}

	return start_of_result;
}


char *My_itoabase(int number, int base, char *result)
{
	char *start_of_result = result;
	size_t num_of_digits_in_number = NumberOfDigitsInNum(number, base);

	if (number == 0)
	{
		*result = '0';
		++result; 
	}
	
	if (number < 0)
	{
		*result = '-';
		number = -number;
		++result;
	}
	
	result += num_of_digits_in_number;
	*result = '\0';
	--result;

	while (0 != number)
	{
		if (number % base < 10)
		{
			*result = number % base + '0';
		}
		else
		{
			*result = number % base + '0' + 7;
		}
		
		--result;
		number /= base;
	}

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


void CheckEndianess()
{
	int number = 1;
	int *ptr_to_number = &number;
	
	if (1 == *ptr_to_number)
	{
		printf("system is little endian\n");
	} 
	else
	{
		printf("system is not little endian\n");
	}
}


static void Test_My_memcpy()
{
	char dest[50] = {0};
	char dest2[50] = {0};
	const char *src = "more than more than";
	const int arr[8] = {1, 2, 3, 4, 4, 3, 2, 1};
	size_t i = 0;

	for (i = 0; i < WORDSIZE; ++i)
	{
		My_memcpy(dest, src + i, 10);
		memcpy(dest2, src + i, 10);

		assert(strcmp(dest, dest2) == 0);

		My_memcpy(dest, src, 0);
		memcpy(dest2, src, 0);

		assert(strcmp(dest, dest2) == 0);

		My_memcpy(dest, arr + i, 3*sizeof(arr[0]));
		memcpy(dest2, arr + i, 3*sizeof(arr[0]));

		assert(strcmp(dest, dest2) == 0);
	}
}


static void Test_My_memset()
{
	char dest[50] = {0};
	char dest2[50] = {0};
	int dest3[50] = {0};
	int dest4[50] = {0};

	My_memset(dest, 't', 3);
	memset(dest2, 't', 3);
	
	assert(strcmp(dest, dest2) == 0);
	
	My_memset(dest3, 5, 20);
	memset(dest4, 5, 20);
	
	assert(*dest3 == *dest4 && *dest3 + 19 == *dest4 + 19);
}

void Test_ThreeArrays()
{
	char *str1 = "abcd ety nutcv";
	char *str2 = "adtp lsq vv";
	char *str3 = "ab ui";
	char *str4 = "dtv ";
	
	ThreeArrays(str1, str2, str3);
	printf("letters that should be printed: %s\n", str4);	
}

void Test_My_atoibase()
{
	char *tests[] = {"100", "   680		", "2147483647", "- 1 1", "ABF"};
	int expected[] = {100, 680, 2147483647, -11, 0};
	int expected7[] = {49, 6, 109, -8, 0};
	size_t number_of_tests = sizeof(tests)/sizeof(tests[0]);
	size_t i = 0, j = 0;
	
	for (i = 0; i < number_of_tests; ++i)
	{
		if (i == 0)
		{
			for (j = 2; j <= 36; ++j)
			{
				assert(My_atoibase(tests[i], j) == (int)(j*j));
			}
		}
		
		assert(My_atoibase(tests[i], 10) == expected[i]);
		assert(My_atoibase(tests[i], 7) == expected7[i]);
	}
	
	assert(My_atoibase(tests[4], 16) == 2751);
}


void Test_My_itoabase()
{
	int tests[] = {0, 1101, -25, INT_MAX};
	char *expected[] = {"0", "1101", "-25", "2147483647"};
	char *expected5[] = {"0", "13401", "-100", "13344223434042"};
	char *expected16[] = {"0", "44D", "-19", "7FFFFFFF"};
	char *expected36[] = {"0", "UL", "-P", "ZIK0ZJ"}; 
	char result[30] = {0};
	size_t number_of_tests = sizeof(tests)/sizeof(tests[0]);
	size_t i = 0;
	
	for (i = 0; i< number_of_tests; ++i)
	{
		My_itoabase(tests[i], 10, result);
		assert(0 == strcmp(result, expected[i]));
		memset(result, 0, 30);
		My_itoabase(tests[i], 5, result);
		assert(0 == strcmp(result, expected5[i]));
		memset(result, 0, 30);
		My_itoabase(tests[i], 16, result);
		assert(0 == strcmp(result, expected16[i]));
		memset(result, 0, 30);
		My_itoabase(tests[i], 36, result);
		assert(0 == strcmp(result, expected36[i]));
		memset(result, 0, 30);
	}	
}
