/*
 * File:   queue.c
 * Author: Ken Kok
 *
 * Created on October 1, 2015, 11:41 AM
 */


#include "xc.h"
#include "queue.h"

/**
 * Description: Inits a unsigned 16bit int queue
 * @param queueP: Pointer to where you want the queue to go
 * @param queueSize: maxLengh of queue desired.
 *                   max = UINT16_QUEUE_SIZE
 * @return boolean indicating whether the queue was init'ed successfully.
 */
bool uint16_InitQueue(uint16_queue *queueP, uint8_t queueSize)
{
    //uint16_t newContents[queueSize];
    
    //queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

/**
 * Description: Checks if the specified queue is empty
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is empty or not.
 */
bool uint16_IsQueueEmpty(uint16_queue *queueP)
{
    return (queueP->cnt == 0);
}

/**
 * Description: Checks if the specified queue is full.
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is full or not.
 */
bool uint16_IsQueueFull(uint16_queue *queueP)
{
    return (queueP->cnt == queueP->maxSize);
}

/**
 * Description: Resets the specified queue - clearing all values
 * @param queueP: Specified queue to reset
 * @return boolean indicating whether the queue was emptied or not.
 */
bool uint16_ClearQueue(uint16_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

/**
 * Description: Push one element to the specified FIFO queue.
 * @param queueP: Specified queue to push to
 * @param element: Element to push into the queue
 * @return boolean indicating whether the push was successful
 */
bool uint16_PushQueue(uint16_queue *queueP, uint16_t element)
{
    if (uint16_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        if(queueP->back == queueP->maxSize)
        {
            queueP->back = 0;
        }
        queueP->contents[(queueP->back % queueP->maxSize)] = element;
        queueP->cnt++;
        return true;
    }
}

/**
 * Description: Pull one element from the specified FIFO queue.
 * @param queueP: Specified queue to pull from
 * @return uint16_t value that was pulled from the queue.
 */
uint16_t uint16_PullQueue(uint16_queue *queueP)
{
    if (uint16_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        queueP->front++;
        if(queueP->front == queueP->maxSize)
        {
            queueP->front = 0;
        }
        queueP->cnt--;
        return queueP->contents[(queueP->front % queueP->maxSize)];
    }
}

/**
 * Description: Inits a unsigned 8bit int queue
 * @param queueP: Pointer to where you want the queue to go
 * @param queueSize: maxLengh of queue desired.
 *                   max = UINT8_QUEUE_SIZE
 * @return boolean indicating whether the queue was init'ed successfully.
 */
bool uint8_InitQueue(uint8_queue *queueP, uint8_t queueSize)
{
    //uint8_t newContents[queueSize];
    
    //queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

/**
 * Description: Checks if the specified queue is empty
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is empty or not.
 */
bool uint8_IsQueueEmpty(uint8_queue *queueP)
{
    return (queueP->cnt == 0);
}

/**
 * Description: Checks if the specified queue is full.
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is full or not.
 */
bool uint8_IsQueueFull(uint8_queue *queueP)
{
    return (queueP->cnt == queueP->maxSize);
}

/**
 * Description: Resets the specified queue - clearing all values
 * @param queueP: Specified queue to reset
 * @return boolean indicating whether the queue was emptied or not.
 */
bool uint8_ClearQueue(uint8_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

/**
 * Description: Push one element to the specified FIFO queue.
 * @param queueP: Specified queue to push to
 * @param element: Element to push into the queue
 * @return boolean indicating whether the push was successful
 */
bool uint8_PushQueue(uint8_queue *queueP, uint8_t element)
{
    if (uint8_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        if(queueP->back == queueP->maxSize)
        {
            queueP->back = 0;
        }
        queueP->contents[(queueP->back % queueP->maxSize)] = element;
        queueP->cnt++;
        return true;
    }
}

/**
 * Description: Pull one element from the specified FIFO queue.
 * @param queueP: Specified queue to pull from
 * @return uint8_t value that was pulled from the queue.
 */
uint8_t uint8_PullQueue(uint8_queue *queueP)
{
    if (uint8_IsQueueEmpty(queueP))
    {
        return NULL;
    }
    else
    {
        queueP->front++;
        if(queueP->front == queueP->maxSize)
        {
            queueP->front = 0;
        }
        queueP->cnt--;
        return queueP->contents[(queueP->front % queueP->maxSize)];
    }
}

/**
 * Description: Inits a float queue
 * @param queueP: Pointer to where you want the queue to go
 * @param queueSize: maxLengh of queue desired.
 *                   max = FLOAT_QUEUE_SIZE
 * @return boolean indicating whether the queue was init'ed successfully.
 */
bool float_InitQueue(float_queue *queueP, uint8_t queueSize)
{
    //float newContents[queueSize];
    
    //queueP->contents = newContents;
    queueP->maxSize = queueSize;
    queueP->cnt = 0;
    queueP->front = -1;
    queueP->back = -1;
    
    return true;
}

/**
 * Description: Checks if the specified queue is empty
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is empty or not.
 */
bool float_IsQueueEmpty(float_queue *queueP)
{
    return (queueP->cnt == 0);
}

/**
 * Description: Checks if the specified queue is full.
 * @param queueP: Specified queue to check
 * @return boolean indicating whether the queue is full or not.
 */
bool float_IsQueueFull(float_queue *queueP)
{
    return (queueP->cnt == queueP->maxSize);
}

/**
 * Description: Resets the specified queue - clearing all values
 * @param queueP: Specified queue to reset
 * @return boolean indicating whether the queue was emptied or not.
 */
bool float_ClearQueue(float_queue *queueP)
{
    queueP->front = -1;
    queueP->back = -1;
    queueP->cnt = 0;
    
    return true;
}

/**
 * Description: Push one element to the specified FIFO queue.
 * @param queueP: Specified queue to push to
 * @param element: Element to push into the queue
 * @return boolean indicating whether the push was successful
 */
bool float_PushQueue(float_queue *queueP, float element)
{
    if (float_IsQueueFull(queueP))
    {
        return false; // We can't push to the queue, its full
    }
    else
    {
        queueP->back++;
        if(queueP->back == queueP->maxSize)
        {
            queueP->back = 0;
        }
        queueP->contents[(queueP->back % queueP->maxSize)] = element;
        queueP->cnt++;
        return true;
    }
}

/**
 * Description: Pull one element from the specified FIFO queue.
 * @param queueP: Specified queue to pull from
 * @return float value that was pulled from the queue.
 */
float float_PullQueue(float_queue *queueP)
{
    if (float_IsQueueEmpty(queueP))
    {
        return 0;
    }
    else
    {
        queueP->front++;
        if(queueP->front == queueP->maxSize)
        {
            queueP->front = 0;
        }
        queueP->cnt--;
        return queueP->contents[(queueP->front % queueP->maxSize)];
    }
}

/**
 * Description: Averages all of the elements of the specified float_queue
 * @param queueP: Specified float queue
 * @return float average value of the elements.
 */
float float_AverageQueueElements(float_queue *queueP)
{
    float_queue *fq = queueP;
    if (float_IsQueueEmpty(fq))
    {
        return 0;
    }
    else
    {
        int i;
        int cnt = fq->cnt;
        float acc = 0;
        // Go through the queue, pull each value to accumulate,
        //  then push the same value back on to the stack
        for(i = 0; i < cnt; i++)
        {
            float added = float_PullQueue(fq);
            acc += added;
            float_PushQueue(fq, added);
        }
        // Now get an average
        acc /= cnt;
        
        return acc;
    }
}
