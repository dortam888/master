#include <stdio.h>
#include <stdlib.h>
#include "vsa.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void TestVSAFlow1()
{
	void *memory_pool = NULL;
	size_t memory_size = 155;
	vsa_t *new_vsa = NULL;
	size_t error_counter = 0;
	void *freeme[3] = {NULL};
	void *should_be_null = NULL;

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);

	if (VSABiggestChunkAvailable(new_vsa) != 136)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 128");
		++error_counter;
	}

    freeme[0] = VSAAlloc(new_vsa, 15);

	if (VSABiggestChunkAvailable(new_vsa) != 112)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 96");
		++error_counter;
	}
	
	freeme[1] = VSAAlloc(new_vsa, 9);
	
	if (VSABiggestChunkAvailable(new_vsa) != 88)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 64");
		++error_counter;
	}
	
	freeme[2] = VSAAlloc(new_vsa, 88);
	
	if (VSABiggestChunkAvailable(new_vsa) != 0)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 0");
		++error_counter;
	}
	
	should_be_null = VSAAlloc(new_vsa, 2);
	
	if (NULL != should_be_null)
	{
		printf("should_be_null is %p\n", should_be_null);
		FAIL("shouln't be allocated");
		++error_counter;
	}

	VSAFree(freeme[1]);

	if (VSABiggestChunkAvailable(new_vsa) != 16)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 16");
		++error_counter;
	}

	VSAFree(freeme[2]);

	if (VSABiggestChunkAvailable(new_vsa) != 112)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 80");
		++error_counter;
	}
	
	VSAFree(NULL);

    free(memory_pool);

	if (0 == error_counter)
	{
		PASS("TestVSAFlow1 Pass. Tested: regular use. full memory_pool. ALLOC when Full. Free NULL.");
	}
}

static void TestVSAFlow2()
{
	void *memory_pool = NULL;
	size_t memory_size = 152;
	vsa_t *new_vsa = NULL;
	size_t error_counter = 0;
	size_t i = 0;
	void *freeme[20] = {NULL};

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);
	
	while (VSABiggestChunkAvailable(new_vsa) != 0)
	{
	    freeme[i] = VSAAlloc(new_vsa, 14);
	    ++i;
	}

	if(NULL != freeme[6])
	{
	    printf("pointer is %p", freeme[6]);
	    FAIL("shouldn't be allocated");
	    ++error_counter;
	}
	
	VSAFree(freeme[1]);
	VSAFree(freeme[2]);

    freeme[19] = VSAAlloc(new_vsa, 40);

	if(NULL == freeme[19])
	{
	    printf("pointer is %p", freeme[19]);
	    FAIL("should be allocated");
	    ++error_counter;
	}

    free(memory_pool);

	if (0 == error_counter)
	{
		PASS("TestVSAFlow2 Pass. Tested: Alloc should do fragmentation");
	}
}

static void TestVSAFlow3()
{
	void *memory_pool = NULL;
	size_t memory_size = 152;
	vsa_t *new_vsa = NULL;
	size_t error_counter = 0;
	size_t i = 0;
	char *freeme[20] = {NULL};

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);
	
	while (VSABiggestChunkAvailable(new_vsa) != 0)
	{
	    freeme[i] = VSAAlloc(new_vsa, 14);
	    ++i;
	}
	
	if(NULL != freeme[6])
	{
	    printf("pointer is %p", freeme[4]);
	    FAIL("shouldn't be allocated");
	    ++error_counter;
	}

    freeme[19] = VSAAlloc(new_vsa, 32);
    
	if(NULL != freeme[19])
	{
	    printf("pointer is %p", freeme[19]);
	    FAIL("should be allocated");
	    ++error_counter;
	}

    free(memory_pool);
	if (0 == error_counter)
	{
		PASS("TestVSAFlow3 Pass. Tested: Alloc shouldn't do fragmentation");
	}
}

int main()
{
	TestVSAFlow1();
	TestVSAFlow2();
	TestVSAFlow3();
    return 0;
}
