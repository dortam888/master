/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 29/08/2019
**** Reviewer: Alex
**** Description: This file contains the implementations of functions
				  for fixed sized allocator
				  for more information about the functions read FSA.h
*******************************************************************************/

#include <assert.h> /* assert */
#include <limits.h> /* ULONG_MAX */

#include "FSA.h" /*fsa_t*/

#define WORDSIZE sizeof(size_t)
#define ALLIGN(x) (x) + ((WORDSIZE - (x) % WORDSIZE) % WORDSIZE)
#define DEADPOOL (ULONG_MAX)

struct fsa
{
	size_t next_free_block;
};

typedef struct block_head
{
	size_t head;
}block_head_t;

/****************************SWAP**********************************/
static void Swap(size_t *adress1, size_t *adress2)
{
	size_t tmp = 0;

	assert(NULL != adress1);
	assert(NULL != adress2);

	tmp = *adress1;
	*adress1 = *adress2;
	*adress2 = tmp;
}

/****************************FSAInit**********************************/
fsa_t *FSAInit(void *memory, size_t memory_size, size_t block_size)
{
	fsa_t *new_fsa = memory;
	size_t real_block_size = sizeof(*new_fsa) + ALLIGN(block_size);
	size_t number_of_blocks = 0;
	size_t i = 0;

	assert(NULL != memory);

	number_of_blocks = memory_size / real_block_size;
	new_fsa->next_free_block = sizeof(*new_fsa);
	memory = (char *)memory + new_fsa->next_free_block;

	for (i = 1; i < number_of_blocks; ++i) /*except from last block*/
	{
		((block_head_t *)memory)->head = new_fsa->next_free_block + 
										   real_block_size * i;
		memory = (char *)memory + real_block_size;
	}

	((block_head_t *)memory)->head = DEADPOOL; /* last block initialized with DEADPOOL */

	return new_fsa;
}

/****************************FSAAlloc**********************************/
void *FSAAlloc(fsa_t *fsa)
{
	block_head_t *next_free_block_head = NULL;

	assert(NULL != fsa);

	if (DEADPOOL == fsa->next_free_block)
	{
		return NULL;
	}

	next_free_block_head = (block_head_t *)((char *)fsa + 
											fsa->next_free_block);
	Swap(&(next_free_block_head->head), &(fsa->next_free_block));

	return (char *)next_free_block_head + sizeof(*next_free_block_head);
}

/****************************FSAFree**********************************/
void FSAFree(void *adress_to_free)
{
	fsa_t *fsa = NULL;

	if (NULL == adress_to_free)
	{
		return;
	}

	fsa = (fsa_t *)((size_t)adress_to_free - 
					((block_head_t *)adress_to_free - 1)->head - sizeof(*fsa));
	Swap(&(fsa->next_free_block), 
		 &(((block_head_t *)adress_to_free - 1)->head));
}

/****************************FSACountFree**********************************/
size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	const block_head_t *next_free_block_head = NULL;

	assert(NULL != fsa);

	if (DEADPOOL == fsa->next_free_block)
	{
		return 0;
	}

	next_free_block_head = (const block_head_t *)((const char *)fsa + 
												   fsa->next_free_block);
	++counter; /* should count first block */

	while (DEADPOOL != next_free_block_head->head)
	{
		++counter;
		next_free_block_head = (const block_head_t *)((const char *)fsa + 
								next_free_block_head->head);
	}

	return counter;
}

/****************************FSASuggestSize**********************************/
size_t FSASuggestSize(size_t number_of_blocks, size_t block_size)
{
	size_t real_block_size = WORDSIZE + ALLIGN(block_size);

	return number_of_blocks * (real_block_size) + sizeof(fsa_t);
}

