/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on September 25, 2015, 9:51 AM
 */


#include "xc.h"
#include "math.h"
#include "string.h"
#include "mcc.h"
#include "interrupt_handlers.h"
#include "utilities.h"

#define FCY         2000000UL // Instruction cycle frequency
#include <libpic30.h>

char TextMessageString[MESSAGE_LENGTH] = {
    '(', '"', 't', '"', ':', '"', 'd', '"', ',', '"', 'd', '"', ':', '(', '"',
    'l', '"', ':',
    '0', // Leakage
    '0',
    '0',
    '.',
    '0',
    ',', '"', 'p', '"', ':',
    '0', // Longest Prime
    '0',
    '.',
    '0',
    '0',
    ',', '"', 'b', '"', ':', 
    '0', // Battery Voltage
    '.',
    '0',
    '0',
    '0',
    ',', '"', 'v', '"', ':',
    0x3C, // '<' sign 
    '0', // Volume 1
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 2
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 3
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 4
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 5
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 6
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 7
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 8
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 9
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 10
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 11
    '0',
    '0',
    '.',
    '0',
    ',',
    '0', // Volume 12
    '0',
    '0',
    '.',
    '0',
    '>', ')', ')'
};

char phoneNumber[12] = "+17178211882";

bool isBatteryLow = false;

uint16_t batteryAccumulator = 0;
uint16_t batteryAccumAmt = 0;

float volumeArray[12] = { 0 };
float longestLeakRate = 0;
float longestPrime = 0;

float curHandleAngle = 0;
float prevHandleHangle = 0;

const static float RadToDegrees = 57.2957914; // 180 / pi()
const static int AdjustmentFactor = 2047; // 1/2 of 12 bit ADC

void DelayUS(int us)
{
    __delay_us(us);
}
void DelayMS(int ms)
{
    KickWatchdog();
    __delay_ms(ms);
}

void DelayS(int s)
{
    int i;
    for(i = 0; i > s; i++)
    {
        // Each delay kicks the watchdog
        DelayMS(1000);
    }
}

void KickWatchdog(void)
{
    ClrWdt();
}

float getHandleAngle(uint16_t xAxis, uint16_t yAxis)
{
    signed int xValue = xAxis - AdjustmentFactor;
    signed int yValue = yAxis - AdjustmentFactor;
    
    float angle = atan2(yValue, xValue) * RadToDegrees;
    
    if (angle > 20)
    {
        angle = 20;
    }
    else if (angle < -30)
    {
        angle = -30;
    }
    
    return angle;
}

void updateMessageVolume(void)
{
    // 49 is the starting location for volume 1
    int i, loc = 49;
    
    for(i = 0; i < 12; i++)
    {
        // Do the i-th element of the volume array
        floatToAscii(volumeArray[i], 1, TextMessageString+loc, 5);
        // increment our location in the volume array to the next value
        loc += 6;
    }
}

void updateMessageBattery(void)
{
    float avgBatVoltage = 0;
    if(batteryAccumAmt != 0)
    {
        // Get the average battery voltage
        //  throughout the day
        avgBatVoltage = batteryAccumulator / 
                batteryAccumAmt;
    }
    
    // Update the text message
    //  38 is the first position of battery voltage
    floatToAscii(avgBatVoltage, 3, TextMessageString+38, 5);
}

void updateMessagePrime(void)
{
    // Update the text message
    //  28 is the first digit of the prime in text message
    floatToAscii(longestPrime, 2, TextMessageString+28, 5);
}

void updateMessageLeakage(void)
{
    // Update the text message
    //  18 is the first position of the leakage in text message
    floatToAscii(longestLeakRate, 1, TextMessageString+18, 5);
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

uint8_t receiveUART1(char *ptr, uint16_t ptrLen)
{
    if (UART1_ReceiveBufferIsEmpty())
    {
        // The buffer is empty, so we aren't going to fill the ptr array
        return 0;
    }
    else
    {
        char *pD = ptr;
        UART1_TRANSFER_STATUS status;
        int numBytes = 0;
        while (numBytes < ptrLen)
        {
            status = UART1_TransferStatusGet();
            if (status & UART1_TRANSFER_STATUS_RX_DATA_PRESENT)
            {
                *pD = (char)UART1_Read();
                numBytes++;
                pD++;
            }
        }
        
        return numBytes;
    }
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
    updateMessageVolume();
    updateMessageBattery();
    updateMessagePrime();
    updateMessageLeakage();
}

bool didMessageSend(void)
{
    char buf[8]; // Build an array to hold a response
    
    // If there is something in the array
    if(UART1_ReceiveBufferSizeGet() > 1)
    {
        uint8_t charsReceived = receiveUART1(buf, sizeof(buf));

        if (charsReceived == 0)
        {
            // We didn't receive anything
            return false;
        }
        else
        {
            char *s;
            s = strstr(buf, "OK");
            if (s != NULL)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    // There is nothing in the receive buffer
    else
    {
        return false;
    }

}

void sendMidnightMessage(void)
{
    // We need to send our midnight message
    turnOnSim();
    int timeOutMS = 0;
    while(!IsSimOnNetwork())
    {
        if(timeOutMS >= NETWORK_SEARCH_TIMEOUT)
            break; // We can't find network. IDK how to recover from this
        // The sim is not online yet
        DelayMS(1); // Wait to check, and resets WDT
        timeOutMS++;
    }

    // Update values in text message
    assembleMidnightMessage();

    // Enter text mode
    sendUART1("AT+CMGF=1/r/n", 13);
    DelayMS(100); // Delay to give SIM time to switch
    sendUART1("AT+CMGS=\"", 9); // Start sending a text
    sendUART1(phoneNumber, sizeof(phoneNumber)); // Send phone number
    sendUART1("\"\r\n", 5); // end of phone number
    DelayMS(100);
    sendUART1(TextMessageString, sizeof(TextMessageString)); // Add message
    // TODO: We probably have to send an extra control char here

    // TODO: Teach it to listen for the SIM's response on RX, and 
    //  respond appropriately.
    DelayS(5);
    turnOffSim();
}

float curVolume = 0;
void handleAccelBufferEvent(void)
{
    // TONY: Handle a new set of accel data here
    
    // Data is stored in xAxisBuffer and yAxisBuffer respectively,
    //  w/ size of X_AXIS_BUFFER_SIZE and Y_AXIS_BUFFER_SIZE
    int i;
    for(i = 0; i < xAxisBufferDepth; i++)
    {
        // Get the current angle
        float angle = getHandleAngle(xAxisBuffer[i], yAxisBuffer[i]);
        // Average angle in to curAngle
        curHandleAngle += angle;
        curHandleAngle >>= 1; // divide by 2 to get average
    }
    
    // Reset the ADXL buffers
    xAxisBufferDepth = 0;
    yAxisBufferDepth = 0;
    
    // Check if our angle changed
    if (abs(curHandleAngle - prevHandleAngle) >= 
            HANDLE_MOVEMENT_THRESHOLD)
    {
        // The handle is moving
        if(IsThereWater())
        {
            // There is water & the handle is moving
            // So we are pumping volume
        }
        else
        {
            // The handle is moving and no water,
            // so we are priming
        }
    }
    else
    {
        // The handle is not moving
        if(IsThereWater())
        {
            // If there is water and the handle is not moving
            // then we are leaking
        }
        else
        {
            // Apparently nothing is happening.
        }
    }
    
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
        else
        {
            isBatteryLow = false;
        }
        batteryAccumulator += batteryBuffer[i];
        batteryAccumAmt++;
    }
}
