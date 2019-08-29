#ifndef ILRD_FSA_H
#define ILRD_FSA_H
#include <stddef.h> /* size_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Wednesday August 28 2019 02:42:39 PM */
/* This file contains functions for memory_pool*/
/*############################################################################*/

typedef struct fsa fsa_t;

/*############################################################################
  # Description:
	Initialize the memory pool
  # Parameters:
	memory - The adress to the start of the FSA
	memory_size - the size of the FSA
	block_size - size of the block
  # Return Value:
	a pointer to FSA

############################################################################*/
fsa_t *FSAInit(void *memory, size_t memory_size, size_t block_size);

/*############################################################################
  # Description:
	gives the user a block of memory. complexity O(1);
  # Parameters:
	a pointer to FSA
  # Return Value:
	adress of the start of the block for the users use
	if there is no free block return NULL

############################################################################*/
void *FSAAlloc(fsa_t *fsa);

/*############################################################################
  # Description:
	free memory from the block. complexity O(1);
  # Parameters:
	ap pointer to the block that should be freed
  # Return Value:
	None
############################################################################*/
void FSAFree(void *adress_to_free);

/*############################################################################
  # Description:
	Count how many blocks are free to use
  # Parameters:
	a pointer to FSA
  # Return Value:
	Number of block that are free to use
############################################################################*/
size_t FSACountFree(const fsa_t *fsa);

/*############################################################################
  # Description:
	Gives the user the memory size that he should allocate for the memory pool
	to get the specified number of blocks he requested
  # Parameters:
	number_of_blocks - number of blocks that the user needs
	block_size - number of bytes in each fixed size block
  # Return Value: number of bytes the user should allocate

############################################################################*/
size_t FSASuggestSize(size_t number_of_blocks, size_t block_size);

#endif /* ILRD_FSA_H */
