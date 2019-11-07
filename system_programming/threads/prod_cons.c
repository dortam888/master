#include <stdio.h>
#include <pthread.h>

static void *Producer(void *);
static void *Consumer(void *);
static void Produce(void);
static void Consume(void);

#define Array_Size 10
#define UNUSED(x) ((void)(x))

int volatile is_producing = 0;
int volatile is_consuming = 1;
int arr[Array_Size] = {0};

int main()
{
    pthread_t id[2] = {0};
    
    if (0 != pthread_create(&id[0], NULL, Consumer, NULL))
    {
        perror("pthread_create");
    }
    
    pthread_detach(id[0]);
    
    if (0 != pthread_create(&id[1], NULL, Producer, NULL))
    {
        perror("pthread_create");
    }
    
    pthread_join(id[1], NULL);
    
    printf("Done\n");
    
    return 0;
}

static void *Consumer(void *arg)
{
    while(1)
    {
        while (__sync_lock_test_and_set(&is_consuming, 1))
        {}
        
        Consume();
        __sync_lock_release(&is_producing);
    }
    
    UNUSED(arg);
    
    return NULL;
}

static void *Producer(void *arg)
{
    while(1)
    {
        while(__sync_lock_test_and_set(&is_producing, 1))
        {}
        
        Produce();
        __sync_lock_release(&is_consuming);
    }
    
    UNUSED(arg);
    
    return NULL;
}

static void Produce()
{
    size_t i = 0;
    
    for (i = 0; i < Array_Size; ++i)
    {
        __sync_add_and_fetch(&arr[i], i); 
    }               
}

static void Consume()
{
    size_t i = 0;
    
    for (i = 0; i < Array_Size; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

