#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#define NIBBLE1111 15

enum {False, True};

double Pow2(unsigned int x, int y);
static void Pow2Test();

unsigned int IsPower2(unsigned int x);
unsigned int IsPower2Bit(unsigned int x);
static void IsPower2Test_A(unsigned int (*function)(unsigned int));
static void IsPower2Test();

unsigned int Add1(unsigned int x);
static void Add1BitTest();

void PrintNumsWithBits(unsigned int *x, unsigned int length, unsigned char num_of_bits_req);
static void TestPrintNumsWithBits();

unsigned int ByteMirror(unsigned char number);
static void ByteMirrorTest();

unsigned int CheckBitOn(unsigned int x, unsigned int pos1, unsigned int pos2);
static void TestCheckBitOn();

unsigned int CheckBitOrBitOn(unsigned int x, unsigned int pos1, unsigned int pos2);
static void TestCheckBitOrBitOn();

unsigned int SwapTwoBits(unsigned int x, unsigned int pos1, unsigned int pos2);
static void TestSwapTwoBits();

unsigned int ClosestNumberDivided16(unsigned int number);
static void TestClosestNumberDivided16();

void XorSwap(unsigned int *num1, unsigned int *num2);
static void SwapTest(unsigned int *var1, unsigned int *var2);
static void XorSwapTest();

unsigned char CountSetBits(unsigned int num);
unsigned char CountSetBitsLUT(unsigned int num);
static void TestCountBits();
static void TestCount(unsigned char (function)(unsigned int));

void PrintFloat(float num);

int main()
{

	Pow2Test();
	
	IsPower2Test();
	
	Add1BitTest();
	
	ByteMirrorTest();

	TestPrintNumsWithBits();

	TestCheckBitOn();

	TestCheckBitOrBitOn();

	TestSwapTwoBits();

	TestClosestNumberDivided16();

	XorSwapTest();

	TestCountBits();
	
	return 0;
}


double Pow2(unsigned int multiplier, int exponent)
{
	return (exponent > 0)? multiplier << exponent : multiplier >> (-exponent);
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
	
	return (i == x) ? True : False;
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


void PrintNumsWithBits(unsigned int *x, unsigned int length, unsigned char num_of_bits_req)
{
	unsigned int i = 0;
	unsigned int num_of_bits = 0;
	
	printf("numbers with %u bits: \n", num_of_bits_req);

	for (; i < length; ++i)
	{
		num_of_bits = CountSetBitsLUT(*x);
		
		if (num_of_bits == num_of_bits_req)
		{
			printf("%u\n", *x);
		}
		
		++x;
	}		
}


static void TestPrintNumsWithBits()
{
	unsigned int array_of_ints[] = {7, 8, 11, 12, 100, 101};
	PrintNumsWithBits(array_of_ints, 6, 3);
}


unsigned int ByteMirror(unsigned char number)
{
	unsigned char mirror_number = 0;

	while(0 < number)
	{
		mirror_number |= number & 1;
		mirror_number <<= 1;
		number >>= 1;
	}

	return mirror_number >> 1;
}


unsigned int ByteMirrorLUT(unsigned char number)
{
	unsigned char bit_lut[16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
	unsigned char mask = NIBBLE1111;
	unsigned char firstmirror = number & mask;
	unsigned char mirrored = (number >> 4) & mask;
	
	mirrored = bit_lut[mirrored]
	mirrored <<= 4;
	mirrored |= bit_lut[firstmirror];
		
	return mirrored;
}


static void ByteMirrorTest()
{
	assert(ByteMirror(9) == 9);
	assert(ByteMirror(8) == 1);
	assert(ByteMirror(10) == 5);
}


unsigned int CheckBitOn(unsigned int x, unsigned int bit_pos1, unsigned int bit_pos2)
{
	return ((x >> bit_pos1) & (x >> bit_pos2));
}

static void TestCheckBitOn()
{
	assert(!CheckBitOn(8, 2, 6));
	assert(CheckBitOn(9, 0, 3));
	assert(CheckBitOn(10, 1, 3));
	assert(CheckBitOn(33, 0, 5));
	assert(!CheckBitOn(0, 0, 0));
} 


unsigned int CheckBitOrBitOn(unsigned int x, unsigned int bit_pos1, unsigned int bit_pos2)
{
	return (((x >> bit_pos1) | (x >> bit_pos2)) & 1);
}


static void TestCheckBitOrBitOn()
{
	assert(!CheckBitOrBitOn(8, 2, 6));
	assert(CheckBitOrBitOn(9, 1, 3));
	assert(CheckBitOrBitOn(10, 4, 3));
	assert(CheckBitOrBitOn(33, 7, 5));
	assert(!CheckBitOrBitOn(0, 0, 0));
} 


unsigned int SwapTwoBits(unsigned int x, unsigned int bit_pos1, unsigned int bit_pos2)
{
	unsigned int bit1 = (x >> bit_pos1) & 1;
	unsigned int bit2 = (x >> bit_pos2) & 1;
	
	unsigned int xor = bit1 ^ bit2;
	xor = (xor << bit_pos1) | (xor << bit_pos2);
	return x^xor;
}


static void TestSwapTwoBits()
{
	assert(SwapTwoBits(8, 3, 1) == 2);
	assert(SwapTwoBits(8, 3, 2) == 4);
	assert(SwapTwoBits(8, 5, 3) == 32);
	assert(SwapTwoBits(32, 3, 5) == 8);
	assert(SwapTwoBits(77, 3, 3) == 77);
} 


unsigned int ClosestNumberDivided16(unsigned int num)
{
	num >>= 4;
	num <<= 4;

	return num;
}


static void TestClosestNumberDivided16()
{
	assert(ClosestNumberDivided16(33) == 32);
	assert(ClosestNumberDivided16(17) == 16);
	assert(ClosestNumberDivided16(22) == 16);
	assert(ClosestNumberDivided16(16) == 16);
	assert(ClosestNumberDivided16(15) == 0);
	assert(ClosestNumberDivided16(41400) == 41392);
	assert(ClosestNumberDivided16(97) == 96);
	assert(ClosestNumberDivided16(95) == 80);
}


void XorSwap(unsigned int *num1, unsigned int *num2)
{
	if (num1 == num2)
	{
		return;
	}

	*num1 ^= *num2;
	*num2 ^= *num1;
	*num1 ^= *num2;	
}


static void SwapTest(unsigned int *var1, unsigned int *var2)
{
    unsigned int tmp1 = *var1;
    unsigned int tmp2 = *var2;
    
    XorSwap(var1, var2);
    
    assert(tmp1 == *var2 && tmp2 == *var1);
}


static void XorSwapTest()
{
    unsigned int tester1 = 2, tester2 = 3;
    SwapTest(&tester1, &tester2);
    SwapTest(&tester1, &tester1);    
}


unsigned char CountSetBits(unsigned int num)
{
	unsigned char bits_on = 0;

	while (0 != num)
	{
		bits_on += (num & 1);
		
		num >>= 1;
	}

	return bits_on;		
}


unsigned char CountSetBitsLUT(unsigned int num)
{
	unsigned char bit_lut[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};	
	unsigned char mask = NIBBLE1111;
	unsigned bits_on = 0;

	bits_on += bit_lut[num & mask];
	num >>= 4;
	bits_on += bit_lut[num & mask];
	num >>= 4;
	bits_on += bit_lut[num & mask];
	num >>= 4;
	bits_on += bit_lut[num & mask];	
	num >>= 4;
	bits_on += bit_lut[num & mask];	
	num >>= 4;
	bits_on += bit_lut[num & mask];	
	num >>= 4;
	bits_on += bit_lut[num & mask];	
	num >>= 4;
	bits_on += bit_lut[num & mask];	

	return bits_on;
}


static void TestCount(unsigned char (function)(unsigned int))
{
	assert(function(0) == 0);
	assert(function(1) == 1);
	assert(function(2048) == 1);
	assert(function(15) == 4);
	assert(function(31) == 5);
	assert(function(30) == 4);
}


static void TestCountBits()
{
	TestCount(CountSetBitsLUT);
	TestCount(CountSetBits);
}

/* Not Tested
void PrintFloat(float num)
{
	unsigned numint = (unsigned)num;
	
	while (0 != numint)
	{
		printf("%u", (numint & 1));
		numint >>= 1;
	}
}
*/
