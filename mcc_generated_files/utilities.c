/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on September 25, 2015, 9:51 AM
 */


#include "xc.h"
#include "mcc.h"
#include "interrupt_handlers.h"
#include "utilities.h"

#define FCY         2000000UL // Instruction cycle frequency
#include <libpic30.h>

bool isBatteryLow = false;

uint16_t batteryAccumulator = 0;
uint16_t batteryAccumAmt = 0;

float volumeArray[12] = { 0 };
float longestLeakRate = 0;
float longestPrime = 0;

void DelayUS(int us)
{
    __delay_us(us);
}
void DelayMS(int ms)
{
    __delay_ms(ms);
}

void DelayS(int s)
{
    DelayMS(s * 1000);
}

void floatToAscii(float value, int decimalPrecision, char *dataPtr, uint8_t dataLen)
{
    // multiply value by 10^precision, then load into 32 bit int
    uint32_t endValue = 0;
    
    int i, multiplier = 1;
    for (i = 0; i < decimalPrecision; i++)
    {
        multiplier *= 10;
    }
    
    // This is int with correct decimal precision
    endValue = (uint32_t)(value * multiplier);
    int nDigits = numDigits(endValue);
    
    char *pD = dataPtr;
    
    for (i = 0; i < dataLen; i++)
    {
        if(i == (dataLen - decimalPrecision))
            *pD = '.'; // This is the decimal point
        else
        {
            uint32_t diviser = tenToPower(nDigits);
            int value = endValue / diviser;
            // We'll need this for the next iteration
            endValue %= diviser;
            
            // Get the value of the digit
            *pD = (char)(value + 48);
            // Decrement numDigits when we calc a value
            nDigits--;
        }
        
        // We always have to increment the pointer
        pD++;
        
    }
    
}

// Call this to get the number of digits
int numDigits(uint32_t num)
{
    if (num < 0) return 1;
    if (num < 10) return 1;
    if (num < 100) return 2;
    if (num < 1000) return 3;
    if (num < 10000) return 4;
    if (num < 100000) return 5;
    if (num < 1000000) return 6;
    if (num < 10000000) return 7;
    if (num < 100000000) return 8;
    if (num < 1000000000) return 9;
    
    // If we are above 9 digits (32 bit uints go to 4,294,967,295)
    return 10;
}

// returns 10^exponent
uint32_t tenToPower(int exponent)
{
    int i;
    uint32_t val = 1;
    for(i = 0; i < exponent; i++)
    {
        val *= 10;
    }
    
    return val;
}

bool IsSimOn(void)
{
    return simStatus_GetValue();
}

bool IsSimOnNetwork(void)
{
    return isNetlightOn;
}

bool IsThereWater(void)
{
    return isWaterPresent;
}

uint8_t sendUART1(char *dataPtr, uint16_t dataCnt)
{
    // Make a pointer to work with, at the current data location
    char *pD = dataPtr;
    // Number of bytes that have been sent already
    unsigned int bytesSent = 0;
    // Status of UART state machine
    
    while (bytesSent < dataCnt)
    {
        if (!(UART1_TransferStatusGet() == UART1_TRANSFER_STATUS_TX_FULL))
        {
            // If the buffer is not full
            UART1_Write(*(pD));
            // We added a byte
            bytesSent++;
            // So we need to move our pointer
            pD++;
        }
        
        // We can do something else here if we want to
        // Like cleaning up our daily variables, etc.
    }
    
    return 0;
}

void turnOnSim(void)
{
    simVioPin_SetHigh();
    if (!IsSimOn())
    {
        // If sim isn't on, set pwrkey Low
        simPwrKey_SetLow();
    }
    
    while(!IsSimOn()) 
    {
        // Wait for the sim to come on
    }
    // Set PwrKey back to high
    simPwrKey_SetHigh();
}

void turnOffSim(void)
{
    if(IsSimOn())
    {
        // Assert pwrkey low to toggle
        simPwrKey_SetLow();
    }
    while(IsSimOn()) 
    {
        // Wait for sim to turn off
    } 
    simPwrKey_SetHigh();
}

void assembleMidnightMessage(void)
{
    
}

float curVolume = 0;
void handleAccelBufferEvent(void)
{
    // TONY: Handle a new set of accel data here
    
    // Data is stored in xAxisBuffer and yAxisBuffer respectively,
    //  w/ size of X_AXIS_BUFFER_SIZE and Y_AXIS_BUFFER_SIZE
    
    // Reset pointer as desired for where you want samples to go in the buffer
    //  Ptr is yAxisBufferDepth and xAxisBufferDepth. Should be set to
    //  sizeof(xAxisBuffer && yAxisBuffer) when this event is called.
    
    // You can also check if there is water or not using the
    //  IsThereWater() function (returns bool)
    
    // UNDER ASSUMPTION THAT CUR VOLUME WILL BE FILLED
    //  WITH PROPER VALUE
    
    int curHour = CurrentTime.tm_hour;
    curHour >>= 1; // One bit shift to divide by two
    // This makes it so we can avoid a switch case
    
    volumeArray[curHour] += curVolume;
}

void handleBatteryBufferEvent(void)
{
    // Accumulates battery voltage for an end of day
    //  average
    int i = 0;
    for(i = 0; i < BATTERY_BUFFER_SIZE; i++)
    {
        if(batteryBuffer[i] <= BATTERY_LOW_THRESHOLD)
        {
            isBatteryLow = true;
        }
        batteryAccumulator += batteryBuffer[i];
        batteryAccumAmt++;
    }
}
