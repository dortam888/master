#include <stdio.h>
#include <math.h>
#include <assert.h>
float Pow(int exponent);
long long Flip(int number);
void Swap(int *var1, int *var2);
void TestPow(int exponent);
void TestFlip(int flop, int flip);
void PowTest();
void FlipTest();
void SwapTest();
void HelloWorldHex();


int main()
{

HelloWorldHex();

PowTest();

Test

return 0;
}


float Pow(int exponent)
{     
    float base = 10.0;
    float result = 1.0;
    
    if(exponent < 0)
    {
        base = 1.0 / base;
        exponent = -exponent;
    }
        
    for(int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    
    return result;
}

    
long long Flip(int number)
{        
    int result = 0;
    
    while (number != 0)
    {
        result *= 10;    
        result += number % 10;
        number /= 10;  
    }

    return result;        
}
    

void Swap(int *var1, int *var2)
{       
    int tmp = *var1;
    *var1 = *var2;
    *var2 = tmp;     
}
    
void TestPow(int exponent)
{               
    assert((float)pow(10.0, exponent) <= Pow(exponent) * 1.01 || //
           (float)pow(10.0, exponent) >= Pow(exponent) * 0.99);
}


void PowTest()
{
    int tester[5] = {-2, -1, 0, 1, 2};
    
    for (int i = 0; i < 5; i++)
    {
        TestPow(*tester);
        *tester += 4;
    }
    
}

           
void TestFlip(int flop, int flip)
{
    assert(flip == Flip(flop));
}

void TestFlip(int flop, int flip)
{
    assert(flip == Flip(flop));
}




void HelloWorldHex()
{
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c", 0x27, 0x48, 0x65, 0x6C, 0x6C, 0x6F, //
                                          0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, //
                                          0x27);
}
      
       
