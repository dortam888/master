/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: Sunday September 01 2019 01:40:02 PM
**** Reviewer:
**** Description: This file contains the implementations of functions
				  for the data structure vsa.
				  Look at vsa.h for more information about the 
				  functions.
*******************************************************************************/
#include <assert.h> /* assert */

#include "vsa.h" /* vsa_t */

#define WORDSIZE sizeof(size_t)
#define ALIGN(x) (x) + ((WORDSIZE - (x) % WORDSIZE) % WORDSIZE)
#define MAX(a,b) ((a > b) ? (a) : (b))
#define ABS(x) ((x > 0)? (x) : (-x))

typedef struct block_header
{
	long block_size;

	#ifndef NDEBUG
	long magic_number;
	#endif
}block_header_t;

struct vsa
{
	char *end_of_memory;
	block_header_t block_head;
};

static void InitBlock(block_header_t *block, long block_size)
{
	block->block_size = block_size;
	#ifndef NDEBUG
	block->magic_number = 0x900DB10C;
	#endif
}

vsa_t *VSAInit(void *memory, size_t memory_size)
{
	vsa_t *new_vsa = memory;

	assert(NULL != memory);

	new_vsa->end_of_memory = (char *)memory + memory_size;

	InitBlock(&(new_vsa->block_head), memory_size - sizeof(*new_vsa));

	return new_vsa;
}

static block_header_t *MoveToNextBlock(block_header_t *block)
{
	block = (block_header_t *)((char *)block + sizeof(*block) +
							   ABS(block->block_size));
	return block;
}

static block_header_t *FindNextAvailableBlock(block_header_t *block, size_t block_size, char *end)
{
	while (((char *)block + block->block_size) < end)
	{
		static size_t defrag;
		static size_t last_block_size;

		if (block->block_size >= (long)(block_size + sizeof(*block)))
		{
			break;
		}
		else if (block->block_size > 0)
		{
			++defrag;
			if (defrag > 1)
			{
				block_header_t *last_block = (block_header_t *)((char *)block - 
											  last_block_size);
				last_block->block_size += block->block_size;
				block = last_block;
				return FindNextAvailableBlock(block, block_size, end);
			}
		}
		else
		{
			defrag = 0;
		}

		last_block_size = block->block_size + sizeof(*block);
		block = MoveToNextBlock(block);
	}

	return NULL;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	block_header_t *next_block = NULL;
	size_t aligned_size = ALIGN(block_size);

	assert(NULL != vsa);

	next_block = &(vsa->block_head);

	next_block = FindNextAvailableBlock(next_block, block_size, 
										vsa->end_of_memory);

	if (NULL == next_block)
	{
		return NULL;
	}

	InitBlock(next_block, -aligned_size);

	/*next_block = MoveToNextBlock(next_block);*/

	return next_block + 1;
}

void VSAFree(void *address_to_free)
{
	block_header_t *block_to_free = NULL;

	if (NULL == address_to_free)
	{
		return;
	}

	block_to_free = (block_header_t *)address_to_free - 1;
	assert(0x900dB10c == block_to_free->magic_number);

	block_to_free->block_size = -block_to_free->block_size;
}

size_t VSABiggestChunkAvailable(const vsa_t *vsa)
{
	long max_available_chunk = 0;
	block_header_t *next_block = NULL;

	assert(NULL != vsa);

	next_block = (block_header_t *)((const char *)vsa + offsetof(vsa_t, block_head));

	while ((char *)(next_block) < vsa->end_of_memory)
	{
		max_available_chunk = MAX(max_available_chunk, next_block->block_size);
		next_block = MoveToNextBlock(next_block);
	}

	return (size_t)max_available_chunk;
}

