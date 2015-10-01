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
#ifndef INTERRUPT_HANDLERS_H
#define	INTERRUPT_HANDLERS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "mcc.h"
#include "queue.h"

#define DEPTH_BUFFER_SIZE           8
#define BATTERY_BUFFER_SIZE         8
#define Y_AXIS_BUFFER_SIZE          8
#define X_AXIS_BUFFER_SIZE          8
#define ANGLES_TO_AVERAGE           10

#define WaterPeriodLowBound         250 // ~1kHz
#define WaterPeriodHighBound        385 // ~650Hz
#define NetlightPeriodLowBound      19500 // ~2.5 seconds
#define NetlightPeriodHighBound     27350 // ~3.5 seconds

extern uint16_t depthBuffer[DEPTH_BUFFER_SIZE];
extern uint16_t batteryBuffer[BATTERY_BUFFER_SIZE];

extern uint8_t depthBufferDepth;
extern uint8_t batteryBufferDepth;

extern struct tm PreviousTime;
extern struct tm CurrentTime;

extern queue xQueue;
extern queue yQueue;
extern floatqueue angleQueue;

/**
 Event Flags
 **/

extern bool depthBufferIsFull;
extern bool batteryBufferIsFull;
extern bool yAxisBufferIsFull;
extern bool xAxisBufferIsFull;
extern bool isMidnightPassed;

extern bool isNetlightOn;
extern bool isWaterPresent;

void InitIOCInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);
void IOCHandler(void);
void TurnOffWPSIOC(void);
void TurnOnWPSIOC(void);

void InitQueues(void);

void UpdateWaterStatus(void);
void UpdateNetStatus(void);

void Timer1Handler(void);
void Timer4Handler(void);
void Timer5Handler(void);
void ADC0Handler(void);
void ADC11Handler(void);
void ADC12Handler(void);
void ADC15Handler(void);

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

