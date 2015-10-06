/*
 * File:   interrupt_handlers.c
 * Author: KSK0419
 *
 * Created on September 24, 2015, 12:23 PM
 */


#include "xc.h"
#include "interrupt_handlers.h"
#include "rtcc_handler.h"
#include "queue.h"
#include "constants.h"

uint16_t depthBuffer[DEPTH_BUFFER_SIZE];
uint16_t batteryBuffer[BATTERY_BUFFER_SIZE];

uint8_t depthBufferDepth = 0;
uint8_t batteryBufferDepth = 0;

struct tm PreviousTime;
struct tm CurrentTime;

queue xQueue;
queue yQueue;
floatqueue angleQueue;

/**
 Event Flags
 **/

bool depthBufferIsFull = false;
bool batteryBufferIsFull = false;
bool isMidnightPassed = false;
bool isNetlightOn = false;
bool isWaterPresent = false;

static bool prevWPSValue = false;
static bool prevSimNetlightValue = false;
static bool prevSimStatusValue = false;

void InitIOCInterrupt(void)
{
    // Set Change Notification priority to 7 (lowest))
    IPC4bits.CNIP = 0b111;
    
    // Enable change notification interrupt
    IEC1bits.CNIE = true;
    
    // Enable specific pins
    CNEN1bits.CN9IE = true; // SimStatus Change
    CNEN1bits.CN12IE = true; // SimNetlight Change
    CNEN2bits.CN27IE = true; // WPS Change
}
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    // Clear the interrupt flag
    IFS1bits.CNIF = false;
    // Handle the interrupt
    IOCHandler();
}
void IOCHandler(void)
{
    if (wpsMeasure_GetValue() != prevWPSValue)
    {
        // We must have measured a WPS event
        
        // Get rid of this, because we only want to measure positive change
        //prevWPSValue = !prevWPSValue;
        UpdateWaterStatus();
    }
    
    if (simNetlight_GetValue() != prevSimNetlightValue)
    {
        // We must have measured a Netlight event
        
        // Get rid of this, because we only want to measure positive change
        //prevSimNetlightValue = !prevSimNetlightValue;
        UpdateNetStatus();
    }
    
    if (simStatus_GetValue() != prevSimStatusValue)
    {
        // Sim Status changed
        prevSimStatusValue = !prevSimStatusValue;
    }
}

void TurnOffWPSIOC(void)
{
    CNEN2bits.CN27IE = false;
}

void TurnOnWPSIOC(void)
{
    CNEN2bits.CN27IE = true;
}

bool IsWPSIOCOn(void)
{
    return (bool)CNEN2bits.CN27IE;
}

void InitQueues(void)
{
    InitQueue(&xQueue, X_AXIS_BUFFER_SIZE);
    InitQueue(&yQueue, Y_AXIS_BUFFER_SIZE);
    InitFloatQueue(&angleQueue, ANGLES_TO_AVERAGE);
}

void UpdateWaterStatus(void)
{
    TMR2_Stop();
    // Always compare to 0
    uint16_t periodTicks = TMR2_Counter16BitGet();
    
    if (periodTicks >= WATER_PERIOD_LOW_BOUND && 
            periodTicks <= WATER_PERIOD_HIGH_BOUND)
    {
        isWaterPresent = true;
    }
    else
    {
        isWaterPresent = false;
    }
    
    // Set the timer back to zero
    TMR2_Counter16BitSet(0);
    
    // Start it again for the next event
    TMR2_Start();
}

void UpdateNetStatus(void)
{
    TMR3_Stop();
    
    uint16_t periodTicks = TMR3_Counter16BitGet();
    
    if (periodTicks >= NETLIGHT_PERIOD_LOW_BOUND &&
            periodTicks <= NETLIGHT_PERIOD_HIGH_BOUND)
    {
        isNetlightOn = true;
    }
    else
    {
        isNetlightOn = false;
    }
    
    TMR3_Counter16BitSet(0);
    
    TMR3_Start();
}

/*
 NOTE: This code is blocking.
 I don't like that, but I was unable to find a reliable way
 to make sure that the X and Y axis get sampled at near the same
 time every 10ms. I therefore deprecated ADC11 and ADC15 Handlers,
 because they are just handling the X and Y axis interrupts
 
 These are also no longer called in adc1.c during the ISR.
 */
void Timer1Handler(void)
{
    // This function starts an ADC transaction
    // To get accelerometer x and y values
    
    // It should be called every 10 ms
    

    // Get our ADC channels
    ADC1_CHANNEL xChan = ADC1_XAXIS_ACCELEROMETER;
    ADC1_CHANNEL yChan = ADC1_YAXIS_ACCELEROMETER;
    
    // Change our reference to VDD
    ADC1_ReferenceSelect(ADC1_REFERENCE_AVDD);
    
    // Select our ADC channel as X
    ADC1_ChannelSelect(xChan);
    // Start taking a measurement
    ADC1_Start();
    // Wait for measurement to complete
    while (!ADC1_IsConversionComplete())
    { }
    // Stop ADC when measurement is complete
    ADC1_Stop();
    // Push result to xQueue
    PushQueue(&xQueue, ADC1_ConversionResultGet());
    // Select our ADC Channel as Y
    ADC1_ChannelSelect(yChan);
    // Start taking a measurement
    ADC1_Start();
    // Wait for measurement to complete
    while (!ADC1_IsConversionComplete())
    { }
    // Stop ADC when measurement is complete
    ADC1_Stop();
    // Push result to yQueue
    PushQueue(&yQueue, ADC1_ConversionResultGet());
    
    // Switch the reference back to the band gap
    ADC1_ReferenceSelect(ADC1_REFERENCE_2VBG); 
}

void Timer4Handler(void)
{
    // This function starts an ADC transaction
    // To read the battery level
    
    // It should be called every 1800 s (30 minutes))

    // Pick the battery channel
    ADC1_ChannelSelect(ADC1_BATTERY_SENSOR);
    // Select 2xVBG as reference voltage
    ADC1_ReferenceSelect(ADC1_REFERENCE_2VBG);
    
    // Start sampling, then go away, the ADC interrupt will
    //  do all of the buffering etc.
    ADC1_Start();
}

void Timer5Handler(void)
{
    // This function handlers a timer interrupt
    // It occurs every 1 second, and says that
    // we need to read the RTCC over I2C
    
    PreviousTime = CurrentTime;
    CurrentTime = GetTime();
    
    // If the day isn't the same
    if (PreviousTime.tm_mday != CurrentTime.tm_mday)
    {
        // Then trigger a midnight event
        isMidnightPassed = true;
    }
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
