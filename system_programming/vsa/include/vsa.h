/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Sunday September 1 2019 */
/* This file contains functions for  VSA (variable size allocator)*/
/*############################################################################*/

#ifndef ILRD_VSA_H
#define ILRD_VSA_H
#include <stddef.h> /* size_t */

typedef struct vsa vsa_t;

/*############################################################################
  # Description:
	Initialize the memory pool
  # Parameters:
	memory - The adress to the start of the VSA
	memory_size - the size of the VSA
  # Return Value:
	a pointer to VSA
############################################################################*/
vsa_t *VSAInit(void *memory, size_t memory_size);

/*############################################################################
  # Description:
	gives the user a block of memory in the requested size. complexity O(1);
  # Parameters:
	vsa - a pointer to VSA
	block_size - requested size of memory block
  # Return Value:
	adress of the start of the block for the users use. if the requested 
	size is not available returns NULL.
############################################################################*/
void *VSAAlloc(vsa_t *vsa, size_t block_size);

/*############################################################################
  # Description:
	free memory from the block. complexity O(1). 
  # Parameters:
	a pointer to the block that should be freed
  # Return Value:
	None
############################################################################*/
void VSAFree(void *address_to_free);

/*############################################################################
  # Description:
	returns the biggest consecutive available chunk of memory (in bytes)
  # Parameters:
	a pointer to VSA
  # Return Value:
	returns the biggest consecutive available chunk of memory (in bytes)
############################################################################*/
size_t VSABiggestChunkAvailable(const vsa_t *vsa);

#endif /* ILRD_VSA_H */
