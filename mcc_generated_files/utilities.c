/*
 * File:   utilities.c
 * Author: Ken Kok
 *
 * Created on September 25, 2015, 9:51 AM
 */


#include "xc.h"
#include "math.h"
#include "string.h"
#include "utilities.h"

#define FCY         4000000UL // Instruction cycle frequency
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

char phoneNumber[] = "+13018737202"; //"+17178211882";

bool isBatteryLow = false;

uint32_t batteryAccumulator = 0;
uint16_t batteryAccumAmt = 0;

float volumeArray[12] = { 0 };
float fastestLeakRate = 0;
float longestPrime = 0;

/**
 * Description: Delays the processor by the specified number of microseconds.
 * @param us: Number of microseconds to delay.
 */
void DelayUS(int us)
{
    __delay_us(us);
}

/**
 * Description: Delays the processor by the specified number of milliseconds.
 *                  also resets the watchdog timer
 * @param ms: Number of ms to delay
 */
void DelayMS(int ms)
{
    KickWatchdog();
    __delay_ms(ms);
}

/**
 * Description: Delays the processor by the specified number of seconds,
 *                  this function resets the watchdog timer every 1 second.
 * @param s: Number of seconds to delay
 */
void DelayS(int s)
{
    int i;
    for(i = 0; i > s; i++)
    {
        // Each delay kicks the watchdog
        DelayMS(1000);
    }
}

/**
 * Description: Resets the watchdog timer.
 */
void KickWatchdog(void)
{
    ClrWdt();
}

/**
 * Description: Given an x and y axis 12 bit ADC value, calculates the handle
 *                  angle.
 * @param xAxis: 12 bit ADC value from xAxis
 * @param yAxis: 12 bit ADC value from yAxis
 * @return float indicating current handle angle.
 * 
 * Note: Angle Limits are 20 and -30 degrees, because those are approximate pump limits.
 */
float GetHandleAngle(uint16_t xAxis, uint16_t yAxis)
{
    signed int xValue = xAxis - c_AdjustmentFactor;
    signed int yValue = yAxis - c_AdjustmentFactor;
    
    float angle = atan2(yValue, xValue) * c_RadToDegrees;
    
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

/**
 * Description: Takes volume accumulator bins and pushes them as ASCII to the
 *                  text message.
 * 
 * Note: Format of each volume element is XXX.X Liters
 */
void UpdateMessageVolume(void)
{
    // 49 is the starting location for volume 1
    int i, loc = 49;
    
    for(i = 0; i < 12; i++)
    {
        // Do the i-th element of the volume array
        FloatToAscii(volumeArray[i], 1, &(TextMessageString[loc]), 5);
        // increment our location in the volume array to the next value
        loc += 6;
    }
}

/**
 * Description: Takes the total battery voltage accumulated throughout the day,
 *                  and converts it to ASCII in the text message array.
 */
void UpdateMessageBattery(void)
{
    float avgBatVoltage = 0;
    if(batteryAccumAmt != 0)
    {
        // Get the average battery voltage
        //  throughout the day
        avgBatVoltage = batteryAccumulator / 
                batteryAccumAmt;
    }
    
    float avgBat = TurnBattADCToFloat(avgBatVoltage);
    
    // Update the text message
    //  38 is the first position of battery voltage
    FloatToAscii(avgBat, 3, &(TextMessageString[38]), 5);
}

/**
 * Description: Turns a raw battery ADC value to a floating point.
 * @param avgBatVoltage: Raw ADC value to convert
 * @return float representation of that battery voltage.
 */
float TurnBattADCToFloat(uint32_t avgBatVoltage)
{
    return avgBatVoltage * c_BattADCToFloat;
}

/**
 * Description: Takes the longest prime global value and pushes it to the text
 *                  message as an ASCII representation
 */
void UpdateMessagePrime(void)
{
    // Update the text message
    //  28 is the first digit of the prime in text message
    FloatToAscii(longestPrime, 1, &(TextMessageString[28]), 5); // Assumes priming is always less than 1000
}

/**
 * Description: Takes the fastest leak rate in L/s and pushes it to the text
 *                  message as an ASCII representation of L/hr
 */
void UpdateMessageLeakage(void)
{
    // Update the text message
    //  18 is the first position of the leakage in text message
    //  3600 multiplier converts from L/sec to L/Hr
    FloatToAscii((fastestLeakRate * 3600), 1, &(TextMessageString[18]), 5);
}

/**
 * Description: Converts a floating point value to ASCII
 * @param value: Float value to convert
 * @param decimalPrecision: Number of digits after the decimal point
 * @param dataPtr: char pointer to put the data
 * @param dataLen: Total length of the data (including the decimal pt)
 * 
 * Example: Value x = 100.10, call FtoA(x, 2, ptr, 6);
 */
void FloatToAscii(float value, uint8_t decimalPrecision,
        char *dataPtr, uint8_t dataLen)
{
    // multiply value by 10^precision, then load into 32 bit int
    uint32_t endValue;
    
    // This is int with correct decimal precision
    endValue = (uint32_t)(value * TenToPower(decimalPrecision));
    int nDigits = NumDigits(endValue);
    
    char *pD = dataPtr;
    
    while(IsBinTooSmall(value, decimalPrecision, dataLen))
    {
        decimalPrecision--;
    }
    
    while(IsNumberTooBig(endValue, dataLen))
    {
        // If number is too big, put a zero in front
        *pD = '0';
        // move ptr
        pD++;
        // Reduce dataLen to compensate
        dataLen--;
    }
    
    int i;
    for (i = 0; i < dataLen; i++)
    {
        if(i + 1 == (dataLen - decimalPrecision))
            *pD = '.'; // This is the decimal point
        else
        {
            uint32_t diviser = TenToPower(nDigits - (i + 1));
            //uint32_t diviser = TenToPower(nDigits - 1);
            int singleDigitVal = endValue / diviser;
            // We'll need this for the next iteration
            endValue %= diviser;
            
            // Get the value of the digit
            *pD = (char)(singleDigitVal + 48);
        }
        
        // We always have to increment the pointer
        pD++;
        
    }
    
}

/**
 * Description: Returns the number of digits in the specified 32 bit integer
 * @param num: 32 bit integer
 * @return int number of digits
 */
int NumDigits(uint32_t num)
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

/**
 * Description: Returns 10^exponent as a uin32_t
 * @param exponent: Power to raise 10 to.
 * @return uint32_t 10^exponent.
 */
uint32_t TenToPower(int exponent)
{
    int i;
    uint32_t val = 1;
    for(i = 0; i < exponent; i++)
    {
        val *= 10;
    }
    
    return val;
}

/**
 * Description: Checks if value is too small to fit into dataLen bytes
 * @param value: Value to check
 * @param dataLen: Length of bytes that value must fit in
 * @return bool indicating whether it fits or not.
 */
bool IsNumberTooBig(uint32_t value, uint8_t dataLen)
{
    // if value < 10^(dataLen-2): we're checking if value won't fit
    //  within the bounds of dataLen.
    // Example: 12.2345 w/ prec 2 dataLen 6 will hand this function
    //  122345. We have 6 locations and 2 decimal precision, so without
    //  padding 0's on the front, we would return 122.34 from floatToAscii.
    // This function helps floatToAscii by telling it that we need to pad
    //  a zero, and thus return 012.23 in our dataLen of 6.
    // dataLen would be -1 logically
    if(value < TenToPower(dataLen-1)) // Changed to -1 to follow logic 10/10/15 KK
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Description: Checks if value is too high to fit into the specified bin w/ prec as spefieid
 * @param value: Value to check
 * @param prec: Desired decimal precision
 * @param len: Total length of the bin to put value in
 * @return boolean indicating whether the desired prec is possible.
 */
bool IsBinTooSmall(float value, uint8_t prec, uint8_t len)
{
    // If value is greater than 10^(dataLen - prec+1 (+1 due to decimal point))
    // , then we know
    //  that the bin is too small (ie. we have to increase len or dec prec).
    // This function helps floatToAscii by detecting a condition where
    //  we need to reduce the precision to fit the desired float into the bin.
    // Its important because we don't always know the order of magnitude of
    //  the float that is passed to floatToAscii.
    if(value > TenToPower(len - (prec + 1)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Description: Checks if the SIM800 is on by its status light
 * @return boolean indicating whether the sim is on or not.
 */
bool IsSimOn(void)
{
    return simStatus_GetValue();
}

/**
 * Description: Checks if the SIM800 is reporting as connected to the network.
 * @return boolean indicating whether it is connected or not.
 */
bool IsSimOnNetwork(void)
{
    return isNetlightOn;
}

/**
 * Description: Checks if the WPS is currently sensing water
 * @return boolean indicating whether water is present or not.
 */
bool IsThereWater(void)
{
    if(IsWPSIOCOn())
    {
        return isWaterPresent;
    }
    // If we do not have the interrupt on, we need to detect if there is
    //  water.
    else
    {
        // Set there is no water to clear false positive condition
        isWaterPresent = false;
        // turn on the interrupt
        TurnOnWPSIOC();
        // if water is on, the signal comes at 2kHz, so 1ms should catch
        //  two pulses
        DelayMS(1);
        // There being water isn't sufficient, because the ~100Hz signal
        //  could have possibly ticked during the 1ms that we were on.
        // So we have to check again
        if(IsThereWater())
        {
            // Clear the possible false positive
            isWaterPresent = false;
            // Again, if water is there it will tick in 1ms. If it is not,
            //  then it can't have ticked in both this and prev test
            DelayMS(1);
            if(IsThereWater())
            {
                // If we found water, turn off IOC and return true
                TurnOffWPSIOC();
                return true;
            }
            else
            {
                TurnOffWPSIOC();
                return false;
            }
        }
        else
        {
            // If the first or second conditions were false, then we
            //  will end up here eventually. Clean up by turning off
            //  the WPS IOC and returning there was no water.
            TurnOffWPSIOC();
            return false;
        }
    }
}

//uint8_t SendUART1(char *dataPtr, uint16_t dataCnt)
//{
//    // Make a pointer to work with, at the current data location
//    char *pD = dataPtr;
//    // Number of bytes that have been sent already
//    uint16_t bytesSent = 0;
//    // Status of UART state machine
//    
//    while (bytesSent < dataCnt)
//    {
//        if (!(UART1_TransferStatusGet() == UART1_TRANSFER_STATUS_TX_FULL))
//        {
//            // If the buffer is not full
//            UART1_Write(*(pD));
//            // We added a byte
//            bytesSent++;
//            // So we need to move our pointer
//            pD++;
//        }
//        
//        // We can do something else here if we want to
//        // Like cleaning up our daily variables, etc.
//    }
//    
//    return 0;
//}
//
//uint8_t ReceiveUART1(char *ptr, uint16_t ptrLen)
//{
//    if (UART1_ReceiveBufferIsEmpty())
//    {
//        // The buffer is empty, so we aren't going to fill the ptr array
//        return 0;
//    }
//    else
//    {
//        char *pD = ptr;
//        UART1_TRANSFER_STATUS status;
//        int numBytes = 0;
//        while (numBytes < ptrLen)
//        {
//            status = UART1_TransferStatusGet();
//            if (status & UART1_TRANSFER_STATUS_RX_DATA_PRESENT)
//            {
//                *pD = (char)UART1_Read();
//                numBytes++;
//                pD++;
//            }
//        }
//        
//        return numBytes;
//    }
//}

/**
 * Description: Turns on the SIM800 by cycling its pwrkey
 */
void TurnOnSim(void)
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
    
    DelayMS(100); // wait for the SIM to be ready to go
}

/**
 * Description: Turns off the SIM800 by cycling its pwrkey
 */
void TurnOffSim(void)
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

/**
 * Description: Assembles the midnight message array using accumulated values
 */
void AssembleMidnightMessage(void)
{
    UpdateMessageVolume();
    UpdateMessageBattery();
    UpdateMessagePrime();
    UpdateMessageLeakage();
}

/**
 * Description: Checks if the SIM800 successfully sent a text message
 * @return boolean indicating whether the SIM800 was successful or not.
 */
bool DidMessageSend(void)
{
//    char buf[8]; // Build an array to hold a response
//    
//    // If there is something in the array
//    if(UART1_ReceiveBufferSizeGet() > 1)
//    {
//        uint8_t charsReceived = ReceiveUART1(buf, sizeof(buf));
//
//        if (charsReceived == 0)
//        {
//            // We didn't receive anything
//            return false;
//        }
//        else
//        {
//            char *s;
//            s = strstr(buf, "OK");
//            if (s != NULL)
//            {
//                return true;
//            }
//            else
//            {
//                return false;
//            }
//        }
//    }
    
    char recArray[10];
    if(UART_Read(recArray, sizeof(recArray)) >= 1)
    {
        char *s;
        s = strstr(recArray, "OK");
        if(s != NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // There is nothing in the receive buffer
    else
    {
        return false;
    }

}

/**
 * Description: Assembles neccesary accumulators into the char array in order
 *                  to send the midnight message
 */
void SendMidnightMessage(void)
{
    // Update values in text message
    AssembleMidnightMessage();

    SendTextMessage(TextMessageString, sizeof(TextMessageString),
                        phoneNumber, sizeof(phoneNumber));
    
    
    ResetAccumulators();
}

/**
 * Description: Sends a text message via the SIM800.
 * @param msgPtr: Pointer to first byte of the message
 * @param msgLen: Length of the message
 * @param numPtr: Pointer to first byte of the phone number to send to
 * @param numLen: Length of the phone number to send to
 */
void SendTextMessage(char *msgPtr, int msgLen, char *numPtr, int numLen)
{
    TurnOnSim();
    
    int timeOutMS = 0;
    while(!IsSimOnNetwork())
    {
        if(timeOutMS >= NETWORK_SEARCH_TIMEOUT_MS)
        {
            break;
        }
        
        DelayMS(1);
        timeOutMS++;
    }
    
    // Enter text mode
    UART_Write_Buffer("AT+CMGF=1\r\n", sizeof("AT+CMFG=1\r\n"));
    // Give SIM time to switch
    DelayMS(250);
    // Tell it we're about to send a phone number
    UART_Write_Buffer("AT+CMGS=\"", sizeof("AT+CMGS=\""));
    // Send the phone number
    UART_Write_Buffer(numPtr, numLen);
    // Tell it the phone number is done
    UART_Write_Buffer("\"\r\n", sizeof("\"\r\n"));
    // Wait for it to be ready to send a text
    DelayMS(250);
    // Tell it what we want our text to say
    UART_Write_Buffer(msgPtr, msgLen);
    // Wait for it to finish receiving
    DelayMS(250);
    // Control character ending to text
    UART_Write_Buffer("\x1A", sizeof("\x1A"));
    
    // Wait for either our text to send, or our timeout to be reached
    DelayMS(5000);
    bool suc = false;
    int timeout = 0;
    while((!suc) && (timeout < TEXT_SEND_TIMEOUT_SECONDS))
    {
        DelayS(1);
        timeout++;
        suc = DidMessageSend();
        
        if(suc)
        {
            TurnOffSim();
        }   
    }
    
    // Regardless of if it sends, we have to turn off
    //  the SIM to conserve power.
    TurnOffSim();
}

/**
 * Description: Resets accumulators back to their 0 states.
 */
void ResetAccumulators(void)
{
    memset(volumeArray, 0, sizeof(volumeArray));
    fastestLeakRate = 0;
    longestPrime = 0;
    batteryAccumulator = 0;
    batteryAccumAmt = 0;
}

static float curAngle;
static float prevAngle;
static float primingUpstroke = 0;
static uint16_t leakTime = 0;
bool lastEventWasPriming = false;
bool lastEventWasLeaking = false;

/**
 * Description: Processes the ADXL queue by removing one value from X and Y queues
 *                  and turning that value into an angle. It then reports the angle
 *                  change and decides where we currently are in the water pumping state
 *                  machine - Not pumping, priming, extracting, or leaking.
 */
void ProcessAccelQueue(void)
{
    // We get here when both x and y queues are not empty
    
    if(float_IsQueueFull(&angleQueue))
    {
        float_PullQueue(&angleQueue); // Empty one slot FIFO
    }
    
    float_PushQueue(&angleQueue, 
            GetHandleAngle(
                uint16_PullQueue(&xQueue), uint16_PullQueue(&yQueue)));
    
    curAngle = float_AverageQueueElements(&angleQueue);

    // Finish calculations from previous entries
    if(!lastEventWasPriming && primingUpstroke > 0)
    {
        if(longestPrime < primingUpstroke)
        {
            longestPrime = primingUpstroke;
        }
            
        primingUpstroke = 0;
    }
        
    if(!lastEventWasLeaking && leakTime > 0)
    {
        if(LeakMSToRate(leakTime) > fastestLeakRate)
        {
            fastestLeakRate = LeakMSToRate(leakTime);
        }
        
        leakTime = 0;
    }
    switch(GetPumpingState(curAngle, prevAngle))
    {
        case PRIMING:
            primingUpstroke += UpstrokeToMeters(curAngle - prevAngle);
            lastEventWasPriming = true;
            lastEventWasLeaking = false;
            break;
        case EXTRACTING_VOLUME:
            AccumulateVolume(curAngle - prevAngle);
            lastEventWasPriming = false;
            lastEventWasLeaking = false;
            break;
        case LEAKING:
            leakTime += 10;
            lastEventWasLeaking = true;
            lastEventWasPriming = false;
            break;
        case NO_ACTION:
            lastEventWasPriming = false;
            lastEventWasLeaking = false;
            break;
    }
    
    prevAngle = curAngle;
}

/**
 * Description: Figures out which state we are pumping in based on angle delta and
 *                  whether water is currently present
 * @param curAngle: Current angle to compare to get angle delta
 * @param prevAngle: Previous angle to compare to get angle delta
 * @return PUMPING_STATE enum to indicate what state we are currently pumping in.
 */
PUMPING_STATE GetPumpingState(float curAngle, float prevAngle)
{
    if((curAngle - prevAngle) > HANDLE_MOVEMENT_THRESHOLD )
    {
        if(IsThereWater())
        {
            return EXTRACTING_VOLUME;
        }
        else
        {
            return PRIMING;
        }
    }
    else
    {
        if(IsThereWater())
        {
            return LEAKING;
        }
        else
        {
            return NO_ACTION;
        }
    }
}

/**
 * Description: Accumulates volume into the correct bin based on current time
 *                  and the angle change. This correctly takes into account leaking
 * @param angleDelta: Angle change to convert to volume
 */
void AccumulateVolume(float angleDelta)
{
    // Get the current hour
    int curHour = CurrentTime.hour;
    curHour >>= 1; // One bit shift to divide by two
    // This makes it so we can avoid a switch case
    
    volumeArray[curHour] += UpstrokeToLiters(angleDelta);
    // Subtract leaking. 10mS per sample = .01 sec/sample. Leak rate in L/sec
    // This has to be in an if statement because if the handle isn't moving we don't
    // subtract
    if(angleDelta > 0)
    {
        // Assuming 10mS
        float leakAmount = (fastestLeakRate / 100);
        // If it is leaking faster than pumping, there is no volume
        if(leakAmount > UpstrokeToLiters(angleDelta))
        {
            leakAmount = UpstrokeToLiters(angleDelta);
        }
        volumeArray[curHour] -= leakAmount;
    } 
}

/**
 * Description: Conversion function to turn upstroke in degrees to meters
 * @param upstroke: Degrees of upstroke
 * @return float meters of upstroke
 */
float UpstrokeToMeters(float upstroke)
{
    // Returns Meters from degrees of upstroke
    return (upstroke * c_UpstrokeToMeters);
}

/**
 * Description: Conversion function to turn upstroke in degrees to liters
 * @param upstroke: Degrees of upstroke
 * @return float liters of dispensed water
 */
float UpstrokeToLiters(float upstroke)
{
    // Returns liters from degrees of upstroke
    return (upstroke * c_MKIILiterPerDegree);
}

/**
 * Description: Conversion function to turn leak time to L/s
 * @param milsec: leak time in ms
 * @return float leak rate in L/s
 */
float LeakMSToRate(uint16_t milsec)
{
    // Returns liters per second (L/s)
    return (c_MaxLitersToLeak / (milsec * 1000));
}

/**
 * Description: Sum up the battery buffer, and keep the values in the 
 *                  daily accumulator
 */
void HandleBatteryBufferEvent(void)
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
        batteryAccumulator += (uint32_t)batteryBuffer[i];
        batteryAccumAmt++;
    }
}
