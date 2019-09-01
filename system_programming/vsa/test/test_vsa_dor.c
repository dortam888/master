#include <stdio.h>
#include <stdlib.h>
#include "vsa.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void TestVSA()
{
	void *memory_pool = NULL;
	size_t memory_size = 400;
	vsa_t *new_vsa = NULL;
	size_t error_counter = 0;

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);

	if (VSABiggestChunkAvailable(new_vsa) != (memory_size - 24))
	{
		printf("biggest chunk got is %lu", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 376");
		++error_counter;
	}
	
	if (0 == error_counter)
	{
		PASS("TestVSA");
	}
}

int main()
{
	TestVSA();
    return 0;
}
