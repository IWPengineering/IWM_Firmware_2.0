/*
 * File:   queue.c
 * Author: KSK0419
 *
 * Created on October 1, 2015, 11:41 AM
 */


#include "xc.h"
#include "queue.h"

bool uint16_InitQueue(uint16_queue *queueP, uint8_t queueSize)
{
    uint16_t *newContents;
    
    newContents = (uint16_t *)malloc(sizeof(uint16_t) * queueSize);
    
    if (newContents == NULL)
    {
        return false;
    }
    
    queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

bool uint16_DestroyQueue(uint16_queue *queueP)
{
    free(queueP->contents);
    
    queueP->contents = NULL;
    queueP->maxSize = 0;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

bool uint16_IsQueueEmpty(uint16_queue *queueP)
{
    return (bool) (queueP->cnt == 0);
}

bool uint16_IsQueueFull(uint16_queue *queueP)
{
    return (bool) (queueP->cnt == queueP->maxSize);
}

bool uint16_ClearQueue(uint16_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

bool uint16_PushQueue(uint16_queue *queueP, uint16_t element)
{
    if (uint16_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        queueP->contents[queueP->back % queueP->maxSize] = element;
        queueP->cnt++;
        return true;
    }
}

uint16_t uint16_PullQueue(uint16_queue *queueP)
{
    if (uint16_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        queueP->front++;
        queueP->cnt--;
        return queueP->contents[queueP->front % queueP->maxSize];
    }
}

//char *next_alloc;
//bool *malloc(int size)
//{
//    char *this_alloc;
//    this_alloc = next_alloc;
//    if((END_OF_ALLOC_SPACE - this_alloc) < size)
//    {
//        return NULL;
//    }
//    next_alloc += size;
//    return this_alloc;
//}

bool uint8_InitQueue(uint8_queue *queueP, uint8_t queueSize)
{
//    uint8_t *newContents;
//    
//    newContents = (uint8_t *)malloc(sizeof(uint8_t) * queueSize);
//    
//    if (newContents == NULL)
//    {
//        return false;
//    }
    uint8_t newContents[queueSize];
    
    queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

bool uint8_DestroyQueue(uint8_queue *queueP)
{
    free(queueP->contents);
    
    queueP->contents = NULL;
    queueP->maxSize = 0;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

bool uint8_IsQueueEmpty(uint8_queue *queueP)
{
    return (bool) (queueP->cnt == 0);
}

bool uint8_IsQueueFull(uint8_queue *queueP)
{
    return (bool) (queueP->cnt == queueP->maxSize);
}

bool uint8_ClearQueue(uint8_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

bool uint8_PushQueue(uint8_queue *queueP, uint8_t element)
{
    if (uint8_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        queueP->contents[queueP->back % queueP->maxSize] = element;
        queueP->cnt++;
        return true;
    }
}

uint8_t uint8_PullQueue(uint8_queue *queueP)
{
    if (uint8_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        queueP->front++;
        queueP->cnt--;
        return queueP->contents[queueP->front % queueP->maxSize];
    }
}

bool float_InitQueue(float_queue *queueP, uint8_t queueSize)
{
    float *newContents;
    
    newContents = (float *)malloc(sizeof(float) * queueSize);
    
    if (newContents == NULL)
    {
        return false;
    }
    
    queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}
bool float_DestroyQueue(float_queue *queueP)
{
    free(queueP->contents);
    
    queueP->contents = NULL;
    queueP->maxSize = 0;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}
bool float_IsQueueEmpty(float_queue *queueP)
{
    return (bool) (queueP->cnt == 0);
}
bool float_IsQueueFull(float_queue *queueP)
{
    return (bool) (queueP->cnt == queueP->maxSize);
}
bool float_ClearQueue(float_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}
bool float_PushQueue(float_queue *queueP, float element)
{
    if (float_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        queueP->contents[queueP->back % queueP->maxSize] = element;
        queueP->cnt++;
        return true;
    }
}
float float_PullQueue(float_queue *queueP)
{
    if (float_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        queueP->front++;
        queueP->cnt--;
        return queueP->contents[queueP->front % queueP->maxSize];
    }
}
float AverageFloatQueueElements(float_queue *queueP)
{
    if (float_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        int i;
        int cnt = queueP->cnt;
        float acc;
        // Go through the queue, pull each value to accumulate,
        //  then push the same value back on to the stack
        for(i = 0; i < cnt; i++)
        {
            float added = float_PullQueue(queueP);
            acc += added;
            float_PushQueue(queueP, added);
        }
        // Now get an average
        acc /= cnt;
        
        return acc;
    }
}
