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
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct uint16_queue {
    uint16_t *contents;
    int front;
    int back;
    int maxSize;
    int cnt;
} uint16_queue;

typedef struct uint8_queue {
    uint8_t *contents;
    int front;
    int back;
    int maxSize;
    int cnt;
} uint8_queue;

typedef struct float_queue {
    float *contents;
    int front;
    int back;
    int maxSize;
    int cnt;
} float_queue;

bool uint16_InitQueue(uint16_queue *queueP, uint8_t queueSize);
bool uint16_DestroyQueue(uint16_queue *queueP);
bool uint16_IsQueueEmpty(uint16_queue *queueP);
bool uint16_IsQueueFull(uint16_queue *queueP);
bool uint16_ClearQueue(uint16_queue *queueP);
bool uint16_PushQueue(uint16_queue *queueP, uint16_t element);
uint16_t uint16_PullQueue(uint16_queue *queueP);

bool uint8_InitQueue(uint8_queue *queueP, uint8_t queueSize);
bool uint8_DestroyQueue(uint8_queue *queueP);
bool uint8_IsQueueEmpty(uint8_queue *queueP);
bool uint8_IsQueueFull(uint8_queue *queueP);
bool uint8_ClearQueue(uint8_queue *queueP);
bool uint8_PushQueue(uint8_queue *queueP, uint8_t element);
uint8_t uint8_PullQueue(uint8_queue *queueP);

bool float_InitQueue(float_queue *queueP, uint8_t queueSize);
bool float_DestroyQueue(float_queue *queueP);
bool float_IsQueueEmpty(float_queue *queueP);
bool float_IsQueueFull(float_queue *queueP);
bool float_ClearQueue(float_queue *queueP);
bool float_PushQueue(float_queue *queueP, float element);
float float_PullQueue(float_queue *queueP);
float AverageFloatQueueElements(float_queue *queueP);




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

