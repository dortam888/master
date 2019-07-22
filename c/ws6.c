#include <stdio.h>

double pow2(unsigned int x, int y);
unsigned int IsPower2(unsigned int x);
unsigned int IsPower2Bit(unsigned int x);
unsigned int Add1Bit(unsigned int x);
void Exactly3Bits(unsigned int *x, unsigned int length);
unsigned int ClosestNumber(unsigned int number);
unsigned int CountSetBits(unsigned int num);
unsigned int CountSetBits_NoLoop(unsigned int num);
void PrintFloat(float num);

int main()
{
	printf("%u\n", IsPower2(2048));
	return 0;
}


double pow2(unsigned int x, int y)
{
	return y > 0? x << y : x >> (-y);
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


unsigned int Add1Bit(unsigned int x)
{
	return -(~x);
}


void Exactly3Bits(unsigned int *x, unsigned int length)
{
	unsigned int i = 0;
	unsigned int bit = 1;
	unsigned int count = 0;
	
	for (; i < length; ++i)
	{
		while (*x > bit)
		{
			if ((*x & bit) == bit)
			{
				count++;
			}
			
			bit = bit << 1;		
		}
		
		if (3 == count)
		{
			printf("%d\n", *x);
		}
	
		x++;
	}		
}


unsigned int ByteMirror(unsigned int number)
{
	unsigned int mirror_number = 0;

	while(0 != number)
	{
		mirror_number += number & 1;
		mirror_number <<= 1;
		number >>= 1;
	}

	return mirror_number;
}


unsigned int Function1(unsigned char x)
{
	return ((x >> 2) & (x >> 6) & 1);
}


unsigned int Function2(unsigned int x)
{
	return ((x >> 2) | (x >> 6) & 1);
}


unsigned int Function3(unsigned int x)
{
	
}


unsigned int ClosestNumber(unsigned int num)
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

	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;	
}


unsigned int CountSetBits(unsigned int num)
{
	unsigned int count = 0;

	while (0 != num)
	{
		if (1 == (num & 1))
		{
			++count;
		}
		
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


