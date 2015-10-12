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
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <math.h>
#include <string.h>
#include "mcc.h"
#include "interrupt_handlers.h"
#include "constants.h"

typedef enum {
            PRIMING,
            EXTRACTING_VOLUME,
            LEAKING,
            NO_ACTION
} pumping_state;

/*
 Public Variables
 */
extern char TextMessageString[MESSAGE_LENGTH];
extern char phoneNumber[12];
extern bool isBatteryLow;
extern const float c_MKIILiterPerDegree;
extern const float c_UpstrokeToMeters;
extern const float c_MaxLitersToLeak;

// Accumulates battery voltage for an end of day average
extern uint32_t batteryAccumulator;
extern uint16_t batteryAccumAmt;

// Volume accumulator array
extern float volumeArray[12];
// Longest leak rate recorded for the day
extern float fastestLeakRate;
// Longest prime time recorded for the day
extern float longestPrime;

/*
 Public Functions
 */
void DelayUS(int us);
void DelayMS(int ms);
void DelayS(int sec);
void KickWatchdog(void);

float GetHandleAngle(uint16_t xAxis, uint16_t yAxis);

void UpdateMessageVolume(void);
void UpdateMessageBattery(void);
float TurnBattADCToFloat(uint32_t avgBatVoltage);
void UpdateMessagePrime(void);
void UpdateMessageLeakage(void);
// len of data must INCLUDE decimal point
//  Cannot handle higher than 6 decimal precision due to implementation
void FloatToAscii(float value, uint8_t decimalPrecision, 
        char *dataPtr, uint8_t dataLen);
int NumDigits(uint32_t num);
uint32_t TenToPower(int exponent);
bool IsNumberTooBig(uint32_t value, uint8_t dataLen);
bool IsBinTooSmall(float value, uint8_t prec, uint8_t len);
bool IsSimOn(void);
bool IsSimOnNetwork(void);
bool IsThereWater(void);

uint8_t SendUART1(char *dataPtr, uint16_t dataCnt);
uint8_t ReceiveUART1(char *ptr, uint16_t ptrLen);
void TurnOnSim(void);
void TurnOffSim(void);

void AssembleMidnightMessage(void);
bool DidMessageSend(void);
void SendMidnightMessage(void);
void ResetAccumulators(void);

void ProcessAccelQueue(void);
pumping_state GetPumpingState(float curAngle, float prevAngle);
void AccumulateVolume(float angleDelta);
float UpstrokeToMeters(float upstroke);
float UpstrokeToLiters(float upstroke);
float LeakMSToRate(uint16_t milsec);

void HandleBatteryBufferEvent(void);
uint8_t DecToBcd(uint8_t val);
/*
 Private Functions
 */

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

