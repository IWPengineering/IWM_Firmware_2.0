/*
 * File:   I2C_Functions.c
 * Author: kkok9
 *
 * Created on October 11, 2015, 7:43 PM
 */


#include "xc.h"
#include "I2C_Functions.h"
#include "utilities.h"

#define I2C_TIMEOUT_VALUE           1300

void I2C_Init(void)
{
    I2C1CON = 0x8200;
    I2C1BRG = 0x0013;
}

time_t I2C_GetTime(void)
{
    
}

void SoftwareReset(void)
{
    
}

I2C_STATUS IdleI2C(void)
{
    int i = 0;
    while(I2C1STATbits.TRSTAT) // Wait for the bus to idle
    {
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS StartI2C(void)
{
    I2C1CONbits.SEN = 1; // Generate a start condition
    
    int i = 0;
    while(I2C1CONbits.SEN)
    {
        // While I2C is still started
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS StopI2C(void)
{
    I2C1CONbits.PEN = 1; // Generate a stop condition
    
    int i = 0;
    while(I2C1CONbits.PEN)
    {
        // Wait for I2C to not be stopped anymore
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS RestartI2C(void)
{
    I2C1CONbits.RSEN = 1; // Generate a reset cond.
    
    int i = 0;
    while(I2C1CONbits.RSEN)
    {
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS NackI2C(void)
{
    I2C1CONbits.ACKDT = 1; // Send NACK during Acknowledge phase
    I2C1CONbits.ACKEN = 1; // Init an Acknowledge sequence on SDA bus
    
    int i = 0;
    while(I2C1CONbits.ACKEN)
    {
        // While we are in an acknowledge phase
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS AckI2C(void)
{
    I2C1CONbits.ACKDT = 0; // Send ACK during Acknowledge phase
    I2C1CONbits.ACKEN = 1; // Init an Acknowledge sequence on SDA bus
    
    int i = 0;
    while(I2C1CONbits.ACKEN)
    {
        // While we are in an acknowledge phase
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS WriteI2C(char data)
{
    int i = 0;
    while(I2C1STATbits.TRSTAT) // Wait for bus to idle
    {
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    I2C1TRN = data; // Load buffer w/ data
    
    i = 0;
    while(I2C1STATbits.TBF) // Wait for data transmission
    {
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    I2C_STATUS stat = IdleI2C();
    while(stat != I2C_SUCCESS)
    {
        stat = IdleI2C();
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS ReadI2C(char *dataPtr)
{
    char *pD = dataPtr; // Give this function the ptr
    
    I2C1CONbits.ACKDT = 1; // Prep a NACK
    I2C1CONbits.RCEN = 1; // Give clk control to slave
    
    int i = 0;
    while(!I2C1STATbits.RBF)
    {
        // While the receive register is not full
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    I2C1CONbits.ACKEN = 1; // Prep to Ack/Nack received data
    
    i = 0;
    while(I2C1CONbits.ACKEN)
    {
        if(i == I2C_TIMEOUT_VALUE)
        {
            SoftwareReset();
            return I2C_SOFTWARE_RESET;
        }
        
        i++;
    }
    
    *pD = I2C1RCV;
    
    return I2C_SUCCESS;
}

I2C_STATUS TurnOffRTCCOscillator(void)
{
    
}

I2C_STATUS SetRTCCTime(time_t *curTime)
{
    uint8_t sec = DecToBcd(curTime->second);
    uint8_t min = DecToBcd(curTime->minute);
    uint8_t hr = DecToBcd(curTime->hour);
    uint8_t wkDay = DecToBcd(curTime->wkDay);
    uint8_t date = DecToBcd(curTime->mnDay);
    uint8_t month = DecToBcd(curTime->month);
    uint8_t year = DecToBcd(curTime->year);
    sec |= 0x80; // Add turn on Osc bit
    hr &= 0xBF; // Turn in to 24 hour time
    wkDay |= 0x08; // Set bat backup to enabled
    
    if(year % 4 == 0)
    {
        month &= 0xDF; // It is apparently not a leap year
    }
    else
    {
        month |= 20; // It is a leap year
    }
    
    I2C_Init();
    I2C_STATUS stat = I2C_NO_TRY;
    while(stat != I2C_SUCCESS)
    {
        stat = TurnOffRTCCOscillator();
    }
    
    StartI2C();
    WriteI2C(0xDE); // Address + Write
    WriteI2C(0x01); // Address for minutes
    WriteI2C(min);
    WriteI2C(hr);
    WriteI2C(wkDay);
    WriteI2C(date);
    WriteI2C(month);
    WriteI2C(year);
    StopI2C();
    
    StartI2C();
    WriteI2C(0xDE); // Address + Write
    WriteI2C(0x00);
    WriteI2C(sec);
    StopI2C();
}

