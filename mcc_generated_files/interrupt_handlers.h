// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPT_HANDLERS_H
#define	INTERRUPT_HANDLERS_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "constants.h"
#include "I2C_Functions.h"
#include "queue.h"
#include "adc1.h"
#include "mcc.h"

extern uint16_t depthBuffer[DEPTH_BUFFER_SIZE];
extern uint16_t batteryBuffer[BATTERY_BUFFER_SIZE];

extern uint8_t depthBufferDepth;
extern uint8_t batteryBufferDepth;

extern time_s PreviousTime;
extern time_s CurrentTime;

extern uint16_queue xQueue;
extern uint16_queue yQueue;
extern float_queue angleQueue;

/**
 Event Flags
 **/

extern bool depthBufferIsFull;
extern bool batteryBufferIsFull;
extern bool isMidnightPassed;

extern bool isNetlightOn;
extern bool isWaterPresent;

void InitIOCInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);
void IOCHandler(void);
void TurnOffWPSIOC(void);
void TurnOnWPSIOC(void);
bool IsWPSIOCOn(void);

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

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

