/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef QUEUE_H
#define	QUEUE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc.h"

typedef struct {
    uint16_t *contents;
    int front;
    int back;
    int maxSize;
    int cnt;
} queue;

typedef struct {
    float *contents;
    int front;
    int back;
    int maxSize;
    int cnt;
} floatqueue;

bool InitQueue(queue *queueP, uint8_t queueSize);
bool DestroyQueue(queue *queueP);
bool IsQueueEmpty(queue *queueP);
bool IsQueueFull(queue *queueP);
bool ClearQueue(queue *queueP);
bool PushQueue(queue *queueP, uint16_t element);
uint16_t PullQueue(queue *queueP);

bool InitFloatQueue(floatqueue *queueP, uint8_t queueSize);
bool DestroyFloatQueue(floatqueue *queueP);
bool IsFloatQueueEmpty(floatqueue *queueP);
bool IsFloatQueueFull(floatqueue *queueP);
bool ClearFloatQueue(floatqueue *queueP);
bool PushFloatQueue(floatqueue *queueP, float element);
float PullFloatQueue(floatqueue *queueP);
float AverageFloatQueueElements(floatqueue *queueP);




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
