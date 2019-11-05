#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define NUM_OF_THREADS 4

int sequence_array[NUM_OF_THREADS] = {0};
pthread_t id[NUM_OF_THREADS] = {0};
static const size_t number = 1234567890;

static void *SumOfDivisors(void *arg)
{   
    size_t i = 0;
    size_t sum = 0;
    size_t part_of_number = number / NUM_OF_THREADS + 1;
    size_t last_index = part_of_number * ((size_t)arg + 1);
    
    for (i = part_of_number * (size_t)arg + 1; 
         i <= last_index; 
         ++i)
    {
        if (number % i == 0)
        {
            sum += i;
        }
    }
    
    return (void *)sum;
}

int main()
{
    size_t i = 0;
    void *sum = 0;
    size_t tot_sum = 0;
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        if(0 != pthread_create(&id[i], NULL, SumOfDivisors, (void *)i))
        {
            printf("%lu\n", i);
            while (0 != pthread_create(&id[i], NULL, SumOfDivisors, (void *)i))
            {}
        }
        
        pthread_join(id[i], &sum);
        tot_sum += (size_t)sum;     
    }
    
    printf("the sum of divisors is %lu\n", tot_sum);
    
    return 0;
}
