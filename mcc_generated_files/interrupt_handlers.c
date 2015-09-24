/*
 * File:   interrupt_handlers.c
 * Author: KSK0419
 *
 * Created on September 24, 2015, 12:23 PM
 */


#include "xc.h"
#include "interrupt_handlers.h"

uint16_t depthBuffer[DEPTH_BUFFER_SIZE];
uint16_t batteryBuffer[BATTERY_BUFFER_SIZE];
uint16_t yAxisBuffer[Y_AXIS_BUFFER_SIZE];
uint16_t xAxisBuffer[X_AXIS_BUFFER_SIZE];

uint8_t depthBufferDepth = 0;
uint8_t batteryBufferDepth = 0;
uint8_t yAxisBufferDepth = 0;
uint8_t xAxisBufferDepth = 0;

struct tm PreviousTime;
struct tm CurrentTime;

/**
 Event Flags
 **/

bool depthBufferIsFull = false;
bool batteryBufferIsFull = false;
bool yAxisBufferIsFull = false;
bool xAxisBufferIsFull = false;

void Timer1Handler(void)
{
    // This function starts an ADC transaction
    // To get accelerometer x and y values
    
    // It should be called every 10 ms
}

void Timer4Handler(void)
{
    // This function starts an ADC transaction
    // To read the battery level
    
    // It should be called every 1800 s (30 minutes))
}

void Timer5Handler(void)
{
    // This function handlers a timer interrupt
    // It occurs every 1 second, and says that
    // we need to read the RTCC over I2C
}

void ADC0Handler(void)
{
    if (depthBufferDepth == DEPTH_BUFFER_SIZE)
    {
        depthBufferIsFull = true;
    }
    else
    {
        // Save the ADC's information
        depthBuffer[depthBufferDepth] = ADC1_ConversionResultGet();
        // Increment where we are
        depthBufferDepth++;
        
        // Set a flag for the main processor to do what it wants
        // with the ADC samples
        if (depthBufferDepth == DEPTH_BUFFER_SIZE)
            depthBufferIsFull = true;
    }
}

void ADC11Handler(void)
{
    if (yAxisBufferDepth == Y_AXIS_BUFFER_SIZE)
    {
        yAxisBufferIsFull = true;
    }
    else
    {
        // Save the ADC's information
        yAxisBuffer[yAxisBufferDepth] = ADC1_ConversionResultGet();
        // Increment where we are
        yAxisBufferDepth++;
        
        // Set a flag for the main processor to do what it wants
        // with the ADC samples
        if (yAxisBufferDepth == Y_AXIS_BUFFER_SIZE)
            yAxisBufferIsFull = true;
    }
}

void ADC12Handler(void)
{
    if (batteryBufferDepth == BATTERY_BUFFER_SIZE)
    {
        batteryBufferIsFull = true;
    }
    else
    {
        // Save the ADC's information
        batteryBuffer[batteryBufferDepth] = ADC1_ConversionResultGet();
        // Increment where we are
        batteryBufferDepth++;
        
        // Set a flag for the main processor to do what it wants
        // with the ADC samples
        if (batteryBufferDepth == BATTERY_BUFFER_SIZE)
            batteryBufferIsFull = true;
    }
}

void ADC15Handler(void)
{
    if (xAxisBufferDepth == X_AXIS_BUFFER_SIZE)
    {
        xAxisBufferIsFull = true;
    }
    else
    {
        // Save the ADC's information
        xAxisBuffer[xAxisBufferDepth] = ADC1_ConversionResultGet();
        // Increment where we are
        xAxisBufferDepth++;
        
        // Set a flag for the main processor to do what it wants
        // with the ADC samples
        if (xAxisBufferDepth == X_AXIS_BUFFER_SIZE)
            xAxisBufferIsFull = true;
    }
}
