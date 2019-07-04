    #include <stdio.h>
    #include <math.h>
    float Pow(int exponent);
    int Flip(int number);
    void Swap(int var1, int var2);
    void TestPow(int exponent);
    
    int main()
    {
    printf("%f", Pow(-2));
    TestPow(-2);
    return 0;
    }
    
    
    float Pow(int exponent)
    {
        
        float base = 10;
        float result = 1;
        
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
    
        
    int Flip(int number)
    {
        
        int result = 0;
        
        while(number)
        {
            result *= 10;    
            result += number % 10;
            number /= 10;  
        }

        return result;
        
    }
        
    
    void Swap(int var1, int var2)
    {
        
        int tmp = var1;
        var1 = var2;
        var2 = tmp;
        return;
           
    }
        
    void TestPow(int exponent)
    {
        
        if((double)pow(10.0, exponent) == (double)Pow(exponent))
        {
            printf("Great Success!!");
        }
        
        else
        {
            printf("Great Fail!!");
        }
        
    }
    
    void TestFlip(int flop, int flip)
    {
        if(flip == Flip(flop))
        {
            printf("Great Success!!");
        }
        
        else
        {
            printf("Great Fail!!");
        }
        
        return;
    }
          
           
