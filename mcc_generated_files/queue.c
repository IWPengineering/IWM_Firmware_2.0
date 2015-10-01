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
