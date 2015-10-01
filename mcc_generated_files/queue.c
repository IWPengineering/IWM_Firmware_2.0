/*
 * File:   queue.c
 * Author: KSK0419
 *
 * Created on October 1, 2015, 11:41 AM
 */


#include "xc.h"
#include "mcc.h"
#include "queue.h"

bool InitQueue(queue *queueP, uint8_t queueSize)
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

bool DestroyQueue(queue *queueP)
{
    free(queueP->contents);
    
    queueP->contents = NULL;
    queueP->maxSize = 0;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

bool IsQueueEmpty(queue *queueP)
{
    return (bool) (queueP->cnt == 0);
}

bool IsQueueFull(queue *queueP)
{
    return (bool) (queueP->cnt == queueP->maxSize);
}

bool ClearQueue(queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

bool PushQueue(queue *queueP, uint16_t element)
{
    if (IsQueueFull(queueP))
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

uint16_t PullQueue(queue *queueP)
{
    if (IsQueueEmpty(queueP))
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

bool InitFloatQueue(floatqueue *queueP, uint8_t queueSize)
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
bool DestroyFloatQueue(floatqueue *queueP)
{
    free(queueP->contents);
    
    queueP->contents = NULL;
    queueP->maxSize = 0;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}
bool IsFloatQueueEmpty(floatqueue *queueP)
{
    return (bool) (queueP->cnt == 0);
}
bool IsFloatQueueFull(floatqueue *queueP)
{
    return (bool) (queueP->cnt == queueP->maxSize);
}
bool ClearFloatQueue(floatqueue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}
bool PushFloatQueue(floatqueue *queueP, float element)
{
    if (IsFloatQueueFull(queueP))
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
float PullFloatQueue(floatqueue *queueP)
{
    if (IsFloatQueueEmpty(queueP))
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
float AverageFloatQueueElements(floatqueue *queueP)
{
    if (IsFloatQueueEmpty(queueP))
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
            float added = PullFloatQueue(queueP);
            acc += added;
            PushFloatQueue(queueP, added);
        }
        // Now get an average
        acc /= cnt;
        
        return acc;
    }
}
