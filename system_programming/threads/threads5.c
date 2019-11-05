#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



static const size_t number = 1234567890;

static size_t SumOfDivisors(void)
{   
    size_t i = 0;
    size_t sum = 0;
      
    #pragma omp parallel num_threads(8)
    {   
        #pragma omp for     
        for (i = 1; i <= number; ++i)
        {
            if (number % i == 0)
            {
                sum += i;
            }
        }
    }
    
    return sum;
}

int main()
{    
    printf("the sum of divisors is %lu\n", SumOfDivisors());
    
    return 0;
}
