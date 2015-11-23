/*
 * File:   interrupt_handlers.c
 * Author: Ken Kok
 *
 * Created on September 24, 2015, 12:23 PM
 */


#include "xc.h"
#include "interrupt_handlers.h"

uint16_t depthBuffer[DEPTH_BUFFER_SIZE];
uint16_t batteryBuffer[BATTERY_BUFFER_SIZE];

uint8_t depthBufferDepth = 0;
uint8_t batteryBufferDepth = 0;

time_s PreviousTime;
time_s CurrentTime;

uint16_queue xQueue;
uint16_queue yQueue;
float_queue angleQueue;

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

/**
 * Description: Initializes the CN interrupts for SIM_STATUS, SIM_NETLIGHT,
 *                  and WPS inputs - to keep track of frequencies.
 */
void InitIOCInterrupt(void)
{
    // Set Change Notification priority to 6 (lowest))
    IPC4bits.CNIP = 4;
    
    // Enable change notification interrupt
    IEC1bits.CNIE = true;
    
    // Enable specific pins
    CNEN1bits.CN9IE = true; // SimStatus Change
    CNEN1bits.CN12IE = true; // SimNetlight Change
    CNEN2bits.CN27IE = true; // WPS Change
}

/**
 * Description: Interrupt handler for all change notification interrupts.
 *                  This handler calls another handler to figure out which
 *                  pin triggered the CN interrupt. Additionally, it clears
 *                  the interrupts flag.
 */
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    // Clear the interrupt flag
    IFS1bits.CNIF = false;
    // Handle the interrupt
    IOCHandler();
}

/**
 * Description: CN Handler. Figures out which of the pins triggered the CN ISR.
 *                  Currently, this handler only catches positive edges for WPS
 *                  and for the netlight, but catches both for status.
 */
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

/**
 * Description: Function to turn off CN interrupts for the WPS. This is implemented
 *                  because the WPS sensor has a relatively high frequency, and thus
 *                  can cause problems in other processing functions if it is left
 *                  on continuously.
 */
void TurnOffWPSIOC(void)
{
    CNEN2bits.CN27IE = false;
}

/**
 * Description: Function to turn on CN interrupts for the WPS. This is implemented
 *                  to turn the WPS interrupts on when we need to know if there is
 *                  water once again.
 */
void TurnOnWPSIOC(void)
{
    CNEN2bits.CN27IE = true;
}

/**
 * Description: Checks if the WPS CN interrupt is currently active.
 * @return boolean indicating whether the WPS IOC is currently active.
 */
bool IsWPSIOCOn(void)
{
    return (bool)CNEN2bits.CN27IE;
}

/**
 * Description: Initializes the 3 queues used to track handle movements and water.
 *                  - xQueue, yQueue, and angleQueue.
 */
void InitQueues(void)
{
    uint16_InitQueue(&xQueue, X_AXIS_BUFFER_SIZE);
    uint16_InitQueue(&yQueue, Y_AXIS_BUFFER_SIZE);
    float_InitQueue(&angleQueue, ANGLES_TO_AVERAGE);
}

/**
 * Description: Called as part of IOC ISR if the WPS sensor is triggered.
 *                  Uses timer 2 to figure out how long since this function
 *                  was last called, and uses that value to decide if the wps
 *                  is currently reading an on or off value.
 */
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

/**
 * Description: Called in the netlight ISR. Uses timer 3 to calculate how long
 *                  it has been since this function was called, and uses that
 *                  period to figure out if the netlight is currently telling
 *                  us that we have network or do not.
 */
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

/**
 * Timer1Handler Desc: Timer1 period of 10ms. When this interrupt occurs,
 *                      we read an X and Y accelerometer sample, and put them
 *                      in their respective buffers.
 */
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
    uint16_PushQueue(&xQueue, ADC1_ConversionResultGet());
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
    uint16_PushQueue(&yQueue, ADC1_ConversionResultGet());
    
    // Switch the reference back to the band gap
    ADC1_ReferenceSelect(ADC1_REFERENCE_2VBG); 
}

/**
 * Description: This function is called on the period overflow of timer4, which
 *                  should occur once every 1800 s (30 minutes). This function
 *                  starts a battery ADC read, but it is completed in the ADC ISR.
 */
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

/**
 * Description: This function is called on the period overflow of timer5, which
 *                  should occur once every 1s. This function gets the current time
 *                  from the RTCC over I2C, and checks if we are in a new day.
 *                  If we are, then midnight has passed.
 */
void Timer5Handler(void)
{
    // This function handlers a timer interrupt
    // It occurs every 1 second, and says that
    // we need to read the RTCC over I2C
    
    PreviousTime = CurrentTime;
    CurrentTime = I2C_GetTime();
    
    // If the day isn't the same
    if (PreviousTime.mnDay != CurrentTime.mnDay)
    {
        // Then trigger a midnight event
        isMidnightPassed = true;
    }
}

/**
 * Description: This is the interrupt handler for ADC0.
 *                  If we are here, it means that we read the depth buffer.
 *                  We store the ADC value in a buffer, and report if the
 *                  buffer is full.
 */
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

/**
 * Description: This is the interrupt handler for ADC12, which occurs when the
 *                  battery voltage is read. We store this value in the appropriate
 *                  buffer, and set a flag if the buffer is full.
 */
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
