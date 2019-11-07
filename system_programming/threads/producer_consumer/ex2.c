#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>

#include "doubly_linked_list.h"

static const size_t NUMBER_OF_PRODUCERS = 5;
static const size_t NUMBER_OF_CONSUMERS = 5;

static pthread_t id[10] = {0};
pthread_mutex_t lock;

static void *Producer(void *);
static void *Consumer(void *);
static void Produce(dlist_t *dlist);
static void Consume(dlist_t *dlist);

int main()
{
    size_t i = 0;
    dlist_t *dlist = DlistCreate();
    
    if (0 != pthread_mutex_init(&lock, NULL))
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    for (i = 0; i < NUMBER_OF_PRODUCERS; ++i)
    {
        pthread_create(&id[i], NULL, Producer, dlist);
        
        pthread_detach(id[i]);
    }
    
    for (i = NUMBER_OF_PRODUCERS; 
         i < NUMBER_OF_PRODUCERS + NUMBER_OF_CONSUMERS;
         ++i)
    {
        pthread_create(&id[i], NULL, Consumer, dlist);
    
        pthread_join(id[i], NULL); 
    }   

    
    return 0;
}

static void *Producer(void *dlist)
{       
    while(1)
    {
        Produce((dlist_t *)dlist);
    }
    
    return NULL;   
}

static void Produce(dlist_t *dlist)
{
    static size_t counter;
    
    pthread_mutex_lock(&lock);
    
    DlistPushBack(dlist, (void *)counter); 
    
    pthread_mutex_unlock(&lock);   
    
    ++counter;
}

static void *Consumer(void *dlist)
{    
    while(1)
    {
        Consume((dlist_t *)dlist);
    }
    
    return NULL;  
}

static void Consume(dlist_t *dlist)
{
    void *data_to_print = NULL;
    
    pthread_mutex_lock(&lock);
    
    while (DlistIsEmpty(dlist))
    {
       pthread_mutex_unlock(&lock);
       pthread_yield();
       pthread_mutex_lock(&lock); 
    }
    
    data_to_print = DlistPopFront(dlist);
    pthread_mutex_unlock(&lock);
     
    
    printf("%lu\n", (size_t)data_to_print);    
}

