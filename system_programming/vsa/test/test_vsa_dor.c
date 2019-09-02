#include <stdio.h>
#include <stdlib.h>
#include "vsa.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void TestVSAFlow1()
{
	void *memory_pool = NULL;
	size_t memory_size = 168;
	vsa_t *new_vsa = NULL;
	size_t error_counter = 0;
	void *freeme[3] = {NULL};
	/*void *should_be_null = NULL;*/

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);

	if (VSABiggestChunkAvailable(new_vsa) != (memory_size - 40))
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 128");
		++error_counter;
	}

    freeme[0] = VSAAlloc(new_vsa, 15);

	if (VSABiggestChunkAvailable(new_vsa) != (memory_size - 72))
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 96");
		++error_counter;
	}
	
	freeme[1] = VSAAlloc(new_vsa, 9);
	
	if (VSABiggestChunkAvailable(new_vsa) != (memory_size - 72 - 32))
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 64");
		++error_counter;
	}
	
	freeme[2] = VSAAlloc(new_vsa, 46);
	
	if (VSABiggestChunkAvailable(new_vsa) != 0)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 0");
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

	if (VSABiggestChunkAvailable(new_vsa) != 48)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 16");
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
	void *freeme[5] = {NULL};

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);
	
	for (i = 0; i < 5; ++i)
	{
	    freeme[i] = VSAAlloc(new_vsa, 14);
	}
	
	if(NULL != freeme[4])
	{
	    printf("pointer is %p", freeme[4]);
	    FAIL("shouldn't be allocated");
	    ++error_counter;
	}
	
	VSAFree(freeme[1]);
	VSAFree(freeme[2]);

    freeme[4] = VSAAlloc(new_vsa, 32);

	if(NULL == freeme[4])
	{
	    printf("pointer is %p", freeme[4]);
	    FAIL("should be allocated");
	    ++error_counter;
	}

	if (VSABiggestChunkAvailable(new_vsa) != 0)
	{
		printf("biggest chunk got is %lu\n", VSABiggestChunkAvailable(new_vsa));
		FAIL("Biggest chunk should be 0 difrag not work");
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
	void *freeme[5] = {NULL};

	memory_pool = malloc(memory_size);
	new_vsa = VSAInit(memory_pool, memory_size);
	
	for (i = 0; i < 5; ++i)
	{
	    freeme[i] = VSAAlloc(new_vsa, 14);
	}
	
	if(NULL != freeme[4])
	{
	    printf("pointer is %p", freeme[4]);
	    FAIL("shouldn't be allocated");
	    ++error_counter;
	}
	
	VSAFree(freeme[1]);
	VSAFree(freeme[3]);

    freeme[4] = VSAAlloc(new_vsa, 32);

	if(NULL != freeme[4])
	{
	    printf("pointer is %p", freeme[4]);
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
	/*TestVSAFlow3();*/
    return 0;
}
