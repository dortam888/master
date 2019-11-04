#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define NUM_OF_THREADS 100000

int sequence_array[NUM_OF_THREADS] = {0};
pthread_t id[NUM_OF_THREADS] = {0};

static void *SumOfDivisors(void *arg)
{   
    
    return NULL;
}

int main()
{
    size_t i = 0;
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        if(0 != pthread_create(&id[i], NULL, CopyToArray, (void *)i))
        {
            printf("%lu\n", i);
            while (0 != pthread_create(&id[i], NULL, CopyToArray, &i))
            {}
        }
        
        pthread_detach(id[i]);     
    }
    
    sleep(10);
    
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        if (sequence_array[i] != i)
        {
            printf("%lu\n", i);
            exit(1);
        }
    }
    
    return 0;
}
