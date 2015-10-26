/*
 * File:   I2C_Functions.c
 * Author: Ken Kok
 *
 * Created on October 11, 2015, 7:43 PM
 */


#include "xc.h"
#include "I2C_Functions.h"
#include "utilities.h"
#include "conversion.h"

#define FCY         2000000UL // Instruction cycle frequency
#include <libpic30.h>

#define I2C_TIMEOUT_VALUE           1300

void I2C_Init(void)
{
    I2C1CON  = 0x0200;
    I2C1BRG  = 0x0012;
    
    I2C1CONbits.I2CEN = 1;
}

time_s I2C_GetTime(void)
{
    time_s t;
    
    I2C_STATUS stat;
    
    while(stat != I2C_SUCCESS)
    {
        stat = I2C_NO_TRY; // Reset stat
        // Then or = stat, if any of them are not success, we'll try again
        stat |= StartI2C();
        stat |= WriteI2C(0xDE); // Addr + Write
        stat |= WriteI2C(0x00); // Addr for seconds
        stat |= RestartI2C();
        stat |= IdleI2C();
        stat |= WriteI2C(0xDF); // Addr + Read
        stat |= ReadI2C(&t.second, false);
        stat |= ReadI2C(&t.minute, false);
        stat |= ReadI2C(&t.hour, false);
        stat |= ReadI2C(&t.wkDay, false);
        stat |= ReadI2C(&t.mnDay, false);
        stat |= ReadI2C(&t.month, false);
        stat |= ReadI2C(&t.year, true);
        stat |= StopI2C();
    }
     
    t.second  &= 0x7F; // Remove Osc
    t.minute  &= 0x7F; // Remove unused
    t.hour    &= 0x3F; // Remove 12/24 bit
    t.wkDay   &= 0x07; // Remove oscRun, pwrFail, VBATEN
    t.mnDay   &= 0x3F; // Remove unused
    t.month   &= 0x1F; // Remove Lpyr
    
    // Convert all values to decimal
    t.second  = BcdToDec(t.second);
    t.minute  = BcdToDec(t.minute);
    t.hour    = BcdToDec(t.hour);
    t.wkDay   = BcdToDec(t.wkDay);
    t.mnDay   = BcdToDec(t.mnDay);
    t.month   = BcdToDec(t.month);
    t.year    = BcdToDec(t.year);
    
    return t;
}

void ToggleSCL(void)
{
    PORTBbits.RB8 = 1;
    DelayUS(10);
    PORTBbits.RB8 = 0;
    DelayUS(10);
    PORTBbits.RB8 = 1;
}

void SoftwareReset(void)
{
    // Procedure:
    /*
     1. Check SDA
        a. If SDA = 1, generate STOP Condition --> Return
        b. If SDA = 0, Generate Clock Pulse on SCL (1-0-1) --> Go To 1
     */
    I2C1CONbits.I2CEN = 0; // Disable the I2C module
    I2C_Init();
    
    TRISBbits.TRISB9 = 0; // Set SDA to an input
    TRISBbits.TRISB8 = 1; // Set SCL to an output
    
    int i = 0;
    while(PORTBbits.RB9 == 0 && i <= 9)
    {
        ToggleSCL();
        
        // Gives another way to break out of this loop,
        //  without locking up the whole program
        //  This should only have to happen 9 times
        if(i >= 20)
        {
            break;
        }
        
        i++;
    }
    
    TRISBbits.TRISB9 = 1; // Set SDA to an output
    TRISBbits.TRISB8 = 0; // Set SCL to an input
    
    // We got here because SDA is 1 now - we need a 
    //  restart - stop condition to reset
    RestartI2C();
    StopI2C();
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

I2C_STATUS WriteI2C(unsigned char data)
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
    
    // Always idle the bus after a send command
    I2C_STATUS stat = IdleI2C();
    
    // Return whatever success status we just got back
    return stat;
}

I2C_STATUS ReadI2C(uint8_t *dataPtr, bool isEoT)
{
    uint8_t *pD = dataPtr; // Give this function the ptr
    
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
    
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    
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
    
    *pD = (uint8_t)I2C1RCV;
    
    return I2C_SUCCESS;
}

I2C_STATUS TurnOffRTCCOscillator(void)
{
    I2C_STATUS stat = I2C_NO_TRY;
    
    while(stat != I2C_SUCCESS)
    {
        stat = I2C_NO_TRY; // Reset status
        
        stat |= StartI2C();
        stat |= WriteI2C(0xDE); // Device Addr + Write
        stat |= WriteI2C(0x00); // Sec addr
        stat |= WriteI2C(0x00); // Turn off osc, set sec to 0
        stat |= StopI2C();
    }
    
    return I2C_SUCCESS;
}

I2C_STATUS SetRTCCTime(time_s *curTime)
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
    wkDay |= 0x07; // Set bat backup to enabled
    
    if(curTime->year % 4 != 0)
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
    
    I2C_STATUS write_stat = I2C_NO_TRY;
    
    while(write_stat != I2C_SUCCESS)
    {
        write_stat = I2C_NO_TRY; // Reset
        
        // I2C_STATUS is set up so that 0x01 = success, 0x02 = Soft reset,
        //  0x04 = Collision detect, etc. So if there is any error, the |=
        //  operator will cause the result to not be 0x01, but instead 0x03,
        //  etc. This can be used to identify specific glitches, but here I
        //  am using it as a catch all.
        write_stat |= StartI2C();
        write_stat |= WriteI2C(0xDE); // Address + Write
        write_stat |= WriteI2C(0x01); // Address for minutes
        write_stat |= WriteI2C(min);
        write_stat |= WriteI2C(hr);
        write_stat |= WriteI2C(wkDay);
        write_stat |= WriteI2C(date);
        write_stat |= WriteI2C(month);
        write_stat |= WriteI2C(year);
        write_stat |= StopI2C();

        // Write the second last and start the oscillator
        write_stat |= StartI2C();
        write_stat |= WriteI2C(0xDE); // Address + Write
        write_stat |= WriteI2C(0x00); // Address for seconds
        write_stat |= WriteI2C(sec);
        write_stat |= StopI2C();
    }
    
    return I2C_SUCCESS;
}

