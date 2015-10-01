/*
 * File:   stack.c
 * Author: KSK0419
 *
 * Created on October 1, 2015, 10:19 AM
 */


#include "xc.h"
#include "stdlib.h"
#include "mcc.h"
#include "stack.h"

bool stackInit(stack_t *stackP, int maxSize)
{
    stackElement_t *newContents;
    
    // Allocate a new array to hold the contents
    newContents = (stackElement_t *)malloc(
            sizeof(stackElement_t) * maxSize);
    
    if (newContents == NULL)
    {
        return false; // We failed to init the stack because there
                      //  isn't enough memory available
    }
    
    stackP->contents = newContents;
    stackP->maxSize = maxSize;
    stackP->top = -1; // Empty
    
    return true;
}

bool stackDestroy(stack_t *stackP)
{
    free(stackP->contents);
    
    stackP->contents = NULL;
    stackP->maxSize = 0;
    stackP->top = -1;
    
    return true;
}

bool stackIsEmpty(stack_t *stackP)
{
    return (bool)stackP->top < 0;
}

bool stackIsFull(stack_t *stackP)
{
    return (bool)stackP->top >= stackP->maxSize - 1;
}

bool stackPush(stack_t *stackP, stackElement_t element)
{
    if (stackIsFull(stackP))
    {
        return false; // We can't push, stack is full
    }
    
    // Put element in array, update top
    stackP->contents[++stackP->top] = element;
    
    return true;
}

stackElement_t stackPop(stack_t *stackP)
{
    if (stackIsEmpty(stackP))
    {
        return NULL;
    }
    
    // Return element from array, update top
    return stackP->contents[stackP->top--];
}
