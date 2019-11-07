#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "circular_queue.h"

struct c_queue
{
    char *start;
    char *begin;
    char *current;
    size_t capacity;
    size_t element_size;
};

c_queue_t *CQueueCreate(size_t capacity, size_t element_size)
{
    c_queue_t *new_queue = (c_queue_t *)malloc(sizeof(*new_queue));
    
    new_queue->start = (char *)malloc(capacity * element_size);
    new_queue->begin = new_queue->begin;
    new_queue->current = new_queue->begin;
    new_queue->capacity = capacity;
    new_queue->element_size = element_size;
    
    return new_queue;
}

void CQueueDestroy(c_queue_t *c_queue)
{
    free(c_queue->begin); c_queue->begin = NULL;
    c_queue->current = NULL;
    
    free(c_queue);
}

int CQueueIsEmpty(const c_queue_t *c_queue)
{
    return c_queue->begin == c_queue->current;
}

int CQueueEnqueue(c_queue_t *c_queue, void *data)
{
    memcpy(c_queue->current, data, c_queue->element_size);
    
    c_queue->current = c_queue->start + (c_queue->current - c_queue->start) % 
    (c_queue->capacity * c_queue->element_size) + c_queue-> element_size;
    
    return 0;           
}

void *CQueueDequeue(c_queue_t *c_queue)
{
    assert(!CQueueIsEmpty(c_queue));
    
    c_queue->begin = c_queue->start + (c_queue->begin - c_queue->start) % 
    (c_queue->capacity * c_queue->element_size) + c_queue-> element_size;
    
    return (void *)c_queue->begin;
}
