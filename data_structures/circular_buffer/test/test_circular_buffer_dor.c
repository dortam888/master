#include "circular_buffer.h"
#include <stdio.h>

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n in line %d", #function, __LINE__)

static void TestEmptyBuffer()
{
	size_t capacity = 10;
	c_buff_t *buffer = CBuffCreate(capacity);
	char buf[10];
	int status = 0;
	int flag = 0;

	if (!CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != capacity)
	{
		++flag;
		FAIL(FreeSpace);
	}
	
	status = CBuffRead(buffer, buf, 5);
	
	if (0 != status)
	{
		++flag;
		FAIL(ReadEmpty);
	}
	
	CBuffDestroy(buffer);
	
	if (flag == 0)
	{
		PASS(EmptyBuffer);
	}
}


static void TestWriteBuffer()
{
	size_t capacity = 5;
	c_buff_t *buffer = CBuffCreate(capacity);
	char data[10] = "dor write";
	int status = 0;
	int flag = 0;

	status = CBuffWrite(buffer, data, 1);
	
	if (CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != capacity - 1)
	{
		++flag;
		FAIL(FreeSpace);
	}

	if (status != 1)
	{
		++flag;
		FAIL(WriteStatus);
	}
	
	status = CBuffWrite(buffer, data + 1, 5);
	
	if (CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != 0)
	{
		++flag;
		FAIL(FreeSpace);
	}

	if (status != 4)
	{
		++flag;
		FAIL(WriteStatusOverFlow);
	}
	
	CBuffDestroy(buffer);
	
	if (flag == 0)
	{
		PASS(WriteBuffer);
	}
}

static void TestReadBuffer()
{
	size_t capacity = 10;
	size_t i = 0;
	c_buff_t *buffer = CBuffCreate(capacity);
	char data[10] = "dor write";
	char buff[10] = {0};
	int status = 0;
	int flag = 0;

	status = CBuffWrite(buffer, data, 15);
	
	if (CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != 0)
	{
		++flag;
		FAIL(FreeSpace);
	}

	if (status != (int)capacity)
	{
		++flag;
		FAIL(WriteStatus);
	}
	
	status = CBuffRead(buffer, buff, 1);
	
	if (CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != 1)
	{
		++flag;
		FAIL(FreeSpace);
	}

	if (status != 1)
	{
		++flag;
		FAIL(ReadStatus);
	}

	if (*buff != *data)
	{
		++flag;
		FAIL(Read);
	}
	
	status = CBuffRead(buffer, buff + 1, 20);
	
	if (!CBuffIsEmpty(buffer))
	{
		++flag;
		FAIL(IsEmpty);
	}
	
	if (CBuffCapacity(buffer) != capacity)
	{
		++flag;
		FAIL(Capacity);
	}
	
	if (CBuffFreeSpace(buffer) != capacity)
	{
		++flag;
		FAIL(FreeSpace);
	}

	if (status != (int)capacity - 1)
	{
		++flag;
		FAIL(ReadStatus);
	}

	for (i = 0; i < capacity; ++i)
	{
		if (buff[i] != data[i])
		{
			++flag;
			FAIL(Read);
		}
	}

	CBuffDestroy(buffer);
	
	if (flag == 0)
	{
		PASS(ReadBuffer);
	}
}


int main()
{
	TestEmptyBuffer();
	TestWriteBuffer();
	TestReadBuffer();
	return 0;
}
