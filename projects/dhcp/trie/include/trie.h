/*############################################################################
 * Owner: OL712
 * Reviewer: OL712
 * Create:
 * This file contains functions for trie data structure operations.
 * Can be used for trie based algorithms 
  ############################################################################*/

#ifndef ILRD_TRIE_H
#define ILRD_TRIE_H
#include <stddef.h> /* size_t */

typedef struct trie trie_t;

/*############################################################################
   # Description:
        creates a single bit trie data structure.
   # Parameters:
      	@num_bits_in_key - the length of the key, number of bits that the key
      						consists of.
   # Return Value:
        returns the new trie.
############################################################################*/
trie_t *TrieCreate(size_t num_bits_in_key);

/*############################################################################
   # Description:
        destroys the received trie.
   # Parameters:
       	@trie - pointer to the trie
   # Return Value:
       	none.
############################################################################*/
void TrieDestroy(trie_t *trie);

/*############################################################################
   # Description:
        checks if the key is found in the trie.
   # Parameters:
       	@trie - pointer to the trie
       	@key - key to search
   # Return Value:
       returns 1 if true, otherwise 0.
############################################################################*/
int TrieIsKeyFound(trie_t *trie, unsigned int key);

typedef enum insertion_status
{
    FAILED_ALLOCATION = -1,
    INSERT_SUCCESS,
    ALREADY_INSERTED
} insertion_status_t;

/*############################################################################
   # Description:
        inserts a key inside the trie.
   # Parameters:
       	@trie - pointer to the trie
       	@key - key to insert
   # Return Value:
       returns SBT_INSERT_SUCCESS if successful, SBT_INSERT_ALLOC_FAIL if 
       memory allocation failed, and SBT_INSERT_ALREADY_EXISTS if the key
       already exists in the trie. 
############################################################################*/
insertion_status_t TrieInsert(trie_t *trie, unsigned int key);

/*############################################################################
   # Description:
        removes a key from the trie.
   # Parameters:
       	@trie - pointer to the trie
       	@key - key to remove
   # Return Value:
       returns 0 if successful, and other integer value for failure.
############################################################################*/
void TrieRemove(trie_t *trie, unsigned int key);

/*############################################################################
   # Description:
        counts how many used paths (keys) are in the trie.
   # Parameters:
       	@trie - pointer to the trie
   # Return Value:
       	returns the number of used paths (keys) in the trie.
############################################################################*/
size_t TrieCountUsedPaths(trie_t *trie);

/*############################################################################
   # Description:
        checks wether the trie is empty or not.
   # Parameters:
       	@trie - pointer to the trie
   # Return Value:
       	returns 1 if true, otherwise 0.
############################################################################*/
int TrieIsEmpty(const trie_t *trie);

#endif /* ILRD_TRIE_H */
