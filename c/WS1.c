    #include <stdio.h>
    #include <math.h>
    int Pow(int exponent);
    int Flip(int number);
    void Swap(int var1, int var2)
    
    int main()
    {
    printf("%d", Flip(0320));
    return 0;
    }
    
    
    int Pow(int exponent)
    {
        
        int base = 10;
        float result = 1;
        
        if(exponent < 0)
        {
            base = 1 / base;
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
        
        if((double)pow(10, exponent) == Pow(exponent))
        {
            printf("Great Success")
        }
        
        else
        {
            printf("Great Fail!!")
        }
        
    }
            
        
        
           
