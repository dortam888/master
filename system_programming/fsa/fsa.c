/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 29/08/2019
**** Reviewer: Alex
**** Description: This file contains the implementations of functions
				  for fixed sized allocator
				  for more information about the functions read FSA.h
*******************************************************************************/

#include <assert.h> /* assert */

#include "fsa.h" /*fsa_t*/

#define WORDSIZE sizeof(size_t)
#define ALIGN(x) (x) + ((WORDSIZE - (x) % WORDSIZE) % WORDSIZE)
#define DEADPOOL 0xDEAD9001

struct fsa
{
	size_t offset_to_next_free_block;
};

typedef struct block_head
{
	size_t offset_to_next_free_block_if_alloced_or_fsa_if_free;
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

static size_t CalcNumOfBlocks(size_t memory_size, size_t block_size)
{
    return (memory_size - sizeof(fsa_t)) / block_size;
}

static block_head_t *MoveToFirstBlockHead(fsa_t *memory)
{
    return (block_head_t *)((char *)memory + memory->offset_to_next_free_block);
}

static block_head_t *MoveToNextBlockHead(block_head_t *memory, 
                                         size_t block_size)
{
    return (block_head_t *)((char *)memory + block_size);
}

/****************************FSAInit**********************************/
fsa_t *FSAInit(void *memory, size_t memory_size, size_t block_size)
{
	fsa_t *new_fsa = memory;
	block_head_t *block_head = memory;
	size_t real_block_size = sizeof(*new_fsa) + ALIGN(block_size);
	size_t number_of_blocks = 0;
	size_t i = 0;

	assert(NULL != memory);

	number_of_blocks = CalcNumOfBlocks(memory_size, real_block_size);
	new_fsa->offset_to_next_free_block = sizeof(*new_fsa);
	block_head = MoveToFirstBlockHead(memory);

	for (i = 1; i < number_of_blocks; ++i) /*except from last block*/
	{
		block_head->offset_to_next_free_block_if_alloced_or_fsa_if_free = 
		new_fsa->offset_to_next_free_block + real_block_size * i;
		block_head = MoveToNextBlockHead(block_head, real_block_size);
	}

	block_head->offset_to_next_free_block_if_alloced_or_fsa_if_free = DEADPOOL; /* last block initialized with DEADPOOL */

	return new_fsa;
}

/****************************FSAAlloc**********************************/
void *FSAAlloc(fsa_t *fsa)
{
	block_head_t *next_free_block_head = NULL;

	assert(NULL != fsa);

	if (DEADPOOL == fsa->offset_to_next_free_block)
	{
		return NULL;
	}

	next_free_block_head = MoveToFirstBlockHead(fsa);
	Swap(&(next_free_block_head->offset_to_next_free_block_if_alloced_or_fsa_if_free), 
	     &(fsa->offset_to_next_free_block));

	return next_free_block_head + 1;
}

/****************************FSAFree**********************************/
void FSAFree(void *adress_to_free)
{
	fsa_t *fsa = NULL;
	block_head_t *start_of_block_to_free = NULL;

	if (NULL == adress_to_free)
	{
		return;
	}

	start_of_block_to_free = (block_head_t *)adress_to_free - 1;
	fsa = (fsa_t *)((size_t)adress_to_free - 
	start_of_block_to_free->offset_to_next_free_block_if_alloced_or_fsa_if_free - 
	sizeof(*fsa));
	Swap(&(fsa->offset_to_next_free_block), 
	     &(start_of_block_to_free->offset_to_next_free_block_if_alloced_or_fsa_if_free));
}

/****************************FSACountFree**********************************/
size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	const block_head_t *next_free_block_head = (block_head_t *)fsa;

	assert(NULL != fsa);

	while (DEADPOOL != next_free_block_head->offset_to_next_free_block_if_alloced_or_fsa_if_free)
	{
		++counter;
		next_free_block_head = (const block_head_t *)((const char *)fsa + 
								next_free_block_head->offset_to_next_free_block_if_alloced_or_fsa_if_free);
	}

	return counter;
}

/****************************FSASuggestSize**********************************/
size_t FSASuggestSize(size_t number_of_blocks, size_t block_size)
{
	size_t real_block_size = sizeof(block_head_t) + ALIGN(block_size);

	return number_of_blocks * real_block_size + sizeof(fsa_t);
}

