#include <stdio.h>

double pow2(unsigned int x, int y);
unsigned int IsPower2(unsigned int x);
unsigned int IsPower2Bit(unsigned int x);
unsigned int Add1Bit(unsigned int x);

int main()
{
	printf("%d\n", Add1Bit(80));
	return 0;
}


double pow2(unsigned int x, int y)
{
	return y > 0? x << y : x >> (-y);
}


unsigned int IsPower2(unsigned int x)
{
	unsigned int i = 0;
	
	while(i << 1 < x)
	{
		++i;
	}
	
	return (i << 1 == x) ? 1 : 0;
}


unsigned int IsPower2Bit(unsigned int x)
{
	return x && (!x & (x-1));
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
			if (*x & bit == bit)
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


void ByteMirror(unsigned int x)
{
	
}
