#include <stdio.h>
#include <assert.h>

double Pow2(unsigned int x, int y);
static void Pow2Test();
unsigned int IsPower2(unsigned int x);
unsigned int IsPower2Bit(unsigned int x);
static void IsPower2Test_A(unsigned int (*function)(unsigned int));
static void IsPower2Test();
unsigned int Add1(unsigned int x);
static void Add1BitTest();
void Exactly3Bits(unsigned int *x, unsigned int length);
unsigned int ByteMirror(unsigned int number);
static void ByteMirrorTest();
unsigned int ClosestNumber(unsigned int number);
unsigned int CountSetBits(unsigned int num);
unsigned int CountSetBits_NoLoop(unsigned int num);
void PrintFloat(float num);

int main()
{
	Pow2Test();
	
	IsPower2Test();
	
	Add1BitTest();
	
	ByteMirrorTest();
	
	return 0;
}


double Pow2(unsigned int x, int y)
{
	return (y > 0)? x << y : x >> (-y);
}


static void Pow2Test()
{
	assert(Pow2(0,88) == 0);
	assert(Pow2(999,0) == 999);
	assert(Pow2(5,1) == 10);
	assert(Pow2(4,3) == 32);
	assert(Pow2(4,-1) == 2);
	assert(Pow2(32,-5) == 1);
}

unsigned int IsPower2(unsigned int x)
{
	unsigned int i = 1;
	
	while(i < x)
	{
		i <<= 1;
	}
	
	return (i == x) ? 1 : 0;
}


unsigned int IsPower2Bit(unsigned int x)
{
	return x && (!(x & (x-1)));
}


static void IsPower2Test_A(unsigned int (*function)(unsigned int))
{
	assert(!function(0));
	assert(!function(77));
	assert(!function(78));
	assert(function(2));
	assert(function(2048));
}


static void IsPower2Test()
{
	IsPower2Test_A(IsPower2);
	IsPower2Test_A(IsPower2Bit);
}

unsigned int Add1(unsigned int x)
{
	return -(~x);
}


static void Add1BitTest()
{
	assert(Add1(99) == 100);
	assert(Add1(1) == 2);
	assert(Add1(0) == 1);
}


void Exactly3Bits(unsigned int *x, unsigned int length)
{
	unsigned int i = 0;
	unsigned int count = 0;
	
	for (; i < length; ++i)
	{
		count = CountSetBits(*x);
		
		if (count == 3)
		{
			printf("%u\n", *x);
		}
		
		++x;
	}		
}


unsigned int ByteMirror(unsigned int number)
{
	unsigned int mirror_number = 0;

	while(0 < number)
	{
		mirror_number |= number & 1;
		mirror_number <<= 1;
		number >>= 1;
	}

	return mirror_number >> 1;
}

static void ByteMirrorTest()
{
	assert(ByteMirror(9) == 9);
	assert(ByteMirror(8) == 1);
	assert(ByteMirror(10) == 5);
}


unsigned int Function1(unsigned int x, unsigned int pos1, unsigned int pos2)
{
	return ((x >> pos1) & (x >> pos2) & 1);
}


unsigned int Function2(unsigned int x, unsigned int pos1, unsigned int pos2)
{
	return (((x >> pos1) | (x >> pos2)) & 1);
}


unsigned int Function3(unsigned int x, unsigned int pos1, unsigned int pos2)
{
	unsigned int bit1 = x >> pos1;
	unsigned int bit2 = x >> pos2;
	
	unsigned int xor = bit1 ^ bit2;
	xor = (xor << pos1) | (xor << pos2);
	return x^xor;
}


unsigned int ClosestNumberDivided16(unsigned int num)
{
	if (num < 16)
	{
		printf("there is no smaller number that divides by 16\n");
		return 0;
	}

	num >>= 4;
	num <<= 4;

	return num;
}


void XorSwap(unsigned int *num1, unsigned int *num2)
{
	if (*num1 == *num2)
	{
		return;
	}

	*num1 ^= *num2;
	*num2 ^= *num1;
	*num1 ^= *num2;	
}


unsigned int CountSetBits(unsigned int num)
{
	unsigned int count = 0;

	while (0 != num)
	{
		count += (num & 1);
		
		num >>= 1;
	}

	return count;		
}


void PrintFloat(float num)
{
	unsigned numint = (unsigned)num;
	
	while (0 != numint)
	{
		printf("%u", (numint & 1));
		numint >>= 1;
	}
}


