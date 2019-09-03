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
#define ALIGN_BACK(x,y) (x) - (x) % (y)
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
	size_t aligned_memory = ALIGN_BACK(memory_size - sizeof(*new_vsa), 
	                        offsetof(vsa_t, block_head)) + sizeof(*new_vsa);

	assert(NULL != memory);

	new_vsa->end_of_memory = (char *)memory + aligned_memory;

	InitBlock(&(new_vsa->block_head), aligned_memory - sizeof(*new_vsa));

	return new_vsa;
}

static block_header_t *MoveToNextBlock(block_header_t *block)
{
    assert(NULL != block);
	block = (block_header_t *)((char *)block + sizeof(*block) +
							   ABS(block->block_size));
	return block;
}

static block_header_t *Defragment(block_header_t *block, 
								  size_t last_block_size)
{
	block_header_t *last_block = NULL;
    assert(NULL != block);
    last_block = (block_header_t *)((char *)block - sizeof(*block) - 
                                    last_block_size);
    last_block->block_size += block->block_size + sizeof(*block);
    
    return last_block;
}

static block_header_t *FindNextAvailableBlock(block_header_t *block, 
                                              size_t block_size, char *end)
{
    assert(NULL != block);
    
    while ((char *)block + sizeof(*block) < end)
    {
        static size_t defrag;
        static size_t last_block_size;

        if (block->block_size >= (long)block_size)
        {
            return block;
        }
        else if (block->block_size > 0)
        {
            ++defrag;
            if (defrag > 1)
            {
                block = Defragment(block, last_block_size);
                return FindNextAvailableBlock(block, block_size, end);
            }
        }
        else
        {
            defrag = 0;
        }

        last_block_size = block->block_size;
        block = MoveToNextBlock(block);
    }

	return NULL;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
    block_header_t *next_block = NULL;
    block_header_t *new_block_header = NULL;
    size_t old_block_size = 0;
    size_t aligned_size = ALIGN(block_size);

    assert(NULL != vsa);

    next_block = (block_header_t *)((char *)vsa + 
                  offsetof(vsa_t, block_head));

    next_block = FindNextAvailableBlock(next_block, aligned_size, 
                                        vsa->end_of_memory);

    if (NULL == next_block)
    {
        return NULL;
    }

    old_block_size = next_block->block_size;
    InitBlock(next_block, -aligned_size);

    new_block_header = (block_header_t *)((char *)next_block + 
                        ABS(next_block->block_size) + sizeof(*next_block));

    if((char *)new_block_header < vsa->end_of_memory - 
                                  sizeof(*new_block_header))
    {
        InitBlock(new_block_header, old_block_size - aligned_size - 
                                    sizeof(*new_block_header));
    }


    return next_block + 1;
}

size_t VSABiggestChunkAvailable(const vsa_t *vsa)
{
    long max_available_chunk = 0;
    block_header_t *next_block = NULL;
    size_t defrag = 0;
    size_t prev_block_size = 0;

    assert(NULL != vsa);

    next_block = (block_header_t *)((const char *)vsa + 
                  offsetof(vsa_t, block_head));

    while ((char *)(next_block) + sizeof(*next_block) < vsa->end_of_memory)
    {
        if (next_block->block_size > 0)
        {
            ++defrag;
        }
        else
        {
            defrag = 0;
        }
        
        if (defrag > 1)
        {
            next_block = Defragment(next_block, prev_block_size); 
        }

        prev_block_size = next_block->block_size;
        max_available_chunk = MAX(max_available_chunk, next_block->block_size);
        next_block = MoveToNextBlock(next_block);
    }

    return (size_t)max_available_chunk;
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
