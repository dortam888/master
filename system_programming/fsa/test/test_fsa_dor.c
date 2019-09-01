#include <stdio.h>
#include <stdlib.h>

#include "fsa.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void TestFlowFSA()
{
	void *memory_pool = NULL;
	size_t error_counter = 0;
	fsa_t *new_fsa = NULL;
	size_t expected_size = 200;
	size_t wanted_blocks = 8;
	size_t wanted_block_size = 12;
	int *var[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

	if (FSASuggestSize(wanted_blocks, wanted_block_size) != expected_size)
	{
		printf("FSASuggest gave %lu\n", FSASuggestSize(wanted_blocks, wanted_block_size));
		FAIL("FSASuggest should be 200");
		++error_counter;
		return;
	}

	memory_pool = malloc(expected_size);
	new_fsa = FSAInit(memory_pool, expected_size, wanted_block_size);

	var[0] = FSAAlloc(new_fsa);
	if ((char *)var[0] - (char *)new_fsa != 8)
	{
		FAIL("Alloc wasn't correct");
		++error_counter;
	}

	if (FSACountFree(new_fsa) != wanted_blocks - 1)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 7");
		++error_counter;
	}

	var[1] = FSAAlloc(new_fsa);
	if ((char *)var[1] - (char *)new_fsa != 32)
	{
		FAIL("Alloc wasn't correct");
		++error_counter;
	}

	if (FSACountFree(new_fsa) != wanted_blocks - 2)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 6");
		++error_counter;
	}

	var[2] = FSAAlloc(new_fsa);
	if ((char *)var[2] - (char *)new_fsa != 56)
	{
		FAIL("Alloc wasn't correct");
		++error_counter;
	}

	if (FSACountFree(new_fsa) != wanted_blocks - 3)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 5");
		++error_counter;
	}

	var[3] = FSAAlloc(new_fsa);
	if ((char *)var[3] - (char *)new_fsa != 80)
	{
		FAIL("Alloc wasn't correct");
		++error_counter;
	}

	if (FSACountFree(new_fsa) != wanted_blocks - 4)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 4");
		++error_counter;
	}

	FSAFree(var[1]); var[1] = NULL;

	if (FSACountFree(new_fsa) != wanted_blocks - 3)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 5 after free");
		++error_counter;
	}

	var[1] = FSAAlloc(new_fsa);

	if (FSACountFree(new_fsa) != wanted_blocks - 4)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 4 after again");
		++error_counter;
	}

	var[4] = FSAAlloc(new_fsa);
	var[5] = FSAAlloc(new_fsa);
	var[6] = FSAAlloc(new_fsa);
	var[7] = FSAAlloc(new_fsa);

	if (FSACountFree(new_fsa) != )
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 0 after full it");
		++error_counter;
	}

	FSAFree(var[0]);
	FSAFree(var[2]);
	FSAFree(var[4]);
	FSAFree(var[6]);
	FSAFree(var[1]);

	if (FSACountFree(new_fsa) != 2)
	{
		printf("FSACountFree gave %lu\n",FSACountFree(new_fsa));
		FAIL("CountFree Should be 2 after two frees");
		++error_counter;
	}

	free(memory_pool);

	if (0 == error_counter)
	{
		PASS("FSA works");
	}
}

int main()
{
	TestFlowFSA();
    return 0;
}
