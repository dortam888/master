#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "circular_queue.h"

struct c_queue
{
    char *begin;
    char *current;
    char *end;
    size_t element_size;
}

c_queue_t *CQueueCreate(size_t capacity, size_t element_size)
{
    c_queue_t *new_queue = (c_queue_t *)malloc(*new_queue);
    
    new_queue->begin = (char *)malloc(capacity * element_size);
    new_queue->current = new_queue->begin;
    new_queue->end = new_queue->begin;
    
    return new_queue;
}

void CQueueDestroy(c_queue_t *c_queue)
{
    free(new_queue->begin); new_queue->begin = NULL;
    new_queue->current = NULL;
    
    free(c_queue);
}

int CQueueIsEmpty(const c_queue_t *c_queue)
{
    return c_queue->begin == c_queue->current;
}

int CQueueEnqueue(c_queue_t *c_queue, void *data)
{
    memcpy(c_queue->current, data, element_size);
    
    c_queue->current = c_queue->current + element_size;           
}
