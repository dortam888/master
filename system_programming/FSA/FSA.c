#include <assert.h> /* assert */
#include "FSA.h" /*fsa_t*/


#define WORDSIZE sizeof(size_t)
#define ALLIGN(x) (x) + ((WORDSIZE - (x) % WORDSIZE) % WORDSIZE)

struct fsa
{
	size_t next_free_block;
};

static void Swap(size_t *adress1, size_t *adress2)
{
	assert(NULL != adress1);
	assert(NULL != adress2);

	/*adresses can't be the same so xor swap will be the most efficient*/
	*adress1 ^= *adress2;
	*adress2 ^= *adress1;
	*adress1 ^= *adress2;
}

fsa_t *FSAInit(void *memory, size_t memory_size, size_t block_size)
{
	fsa_t *new_fsa = memory;
	size_t real_block_size = WORDSIZE + ALLIGN(block_size);
	size_t number_of_blocks = 0;
	size_t i = 0;

	assert(NULL != memory);

	number_of_blocks = memory_size / real_block_size;
	new_fsa->next_free_block = sizeof(*new_fsa);
	memory = (char *)memory + new_fsa->next_free_block;

	for (i = 0; i < number_of_blocks; ++i)
	{
		*(size_t *)memory = new_fsa->next_free_block + real_block_size *
													   (i + 1);
		memory = (char *)memory + real_block_size;
	}

	*(size_t *)((char *)memory - real_block_size) = 0;

	return new_fsa;
}

void *FSAAlloc(fsa_t *fsa)
{
	size_t *memory = NULL;

	assert(NULL != fsa);
	assert(FSACountFree(fsa) != 0);

	memory = (size_t *)((char *)fsa + fsa->next_free_block);
	Swap(memory, (size_t *)fsa);

	return memory;
}

void FSAFree(void *adress_to_free)
{
	if (NULL != adress_to_free)
	{
		size_t *fsa = (size_t *)((size_t)adress_to_free - 
					 *(size_t *)adress_to_free);
		Swap(fsa, (size_t *)adress_to_free);
	}
}

size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	const fsa_t *start_of_fsa = fsa;

	assert(NULL != fsa);

	fsa = (const fsa_t *)((const char *)fsa + *(const size_t *)fsa);
	if (fsa != start_of_fsa)
	{
		++counter;
	}

	while (0 != *(const size_t *)fsa)
	{
		++counter;
		fsa = (const fsa_t *)((const char *)fsa + *(size_t *)fsa - 
							  (size_t)((const char *)fsa - 
							  (const char *)start_of_fsa));
	}

	return counter;
}

size_t FSASuggestSize(size_t number_of_blocks, size_t block_size)
{
	size_t real_block_size = WORDSIZE + ALLIGN(block_size);

	return number_of_blocks * (real_block_size) + sizeof(fsa_t);
}
