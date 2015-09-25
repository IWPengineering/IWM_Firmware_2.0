/*
 * File:   rtcc_handler.c
 * Author: KSK0419
 *
 * Created on September 24, 2015, 3:41 PM
 */


#include "rtcc_handler.h"
#include "mcc.h"

#define RTCC_RETRY_MAX      100
#define RTCC_ADDRESS        0x00
#define RTCC_TIMEOUT        50

uint8_t RTCC_Read(
                    uint16_t address,
                    uint8_t *pData,
                    uint16_t nCount)
{
    I2C1_MESSAGE_STATUS status;
    uint8_t writeBuffer[3];
    uint16_t retryTimeOut, slaveTimeOut;
    uint16_t counter;
    uint8_t *pD;
    
    pD = pData;
    
    for (counter = 0; counter < nCount; counter++)
    {
        writeBuffer[0] = (address >> 8);
        writeBuffer[1] = (uint8_t)(address);
        
        retryTimeOut = 0;
        slaveTimeOut = 0;
        
        while (status != I2C1_MESSAGE_FAIL)
        {
            I2C1_MasterWrite(writeBuffer, 2, RTCC_ADDRESS, &status);
            
            while (status == I2C1_MESSAGE_PENDING)
            {
                // Delay
                
                if (slaveTimeOut == RTCC_TIMEOUT)
                    return 0;
                else
                    slaveTimeOut++;
            }
            
            if (status == I2C1_MESSAGE_COMPLETE)
                break;
            
            if (retryTimeOut == RTCC_RETRY_MAX)
                break;
            else
                retryTimeOut++;
        }
        
        if (status == I2C1_MESSAGE_COMPLETE)
        {
            retryTimeOut = 0;
            slaveTimeOut = 0;
            
            while (status != I2C1_MESSAGE_FAIL)
            {
                I2C1_MasterRead(pD, 1, RTCC_ADDRESS, &status);
                
                while (status == I2C1_MESSAGE_PENDING)
                {
                    // Delay
                    
                    if (slaveTimeOut == RTCC_TIMEOUT)
                        return 0;
                    else
                        slaveTimeOut++;
                }
                
                if (status == I2C1_MESSAGE_COMPLETE)
                    break;
                if (retryTimeOut == RTCC_RETRY_MAX)
                    break;
                else
                    retryTimeOut++;
            }
        }
        
        if (status == I2C1_MESSAGE_FAIL)
        {
            return 0;
            break;
        }
        
        pD++;
        address++;
    }
    
    return 1;
}

struct tm GetTime(void)
{
    struct tm cTime;
    /* Array to stick RTCC values in
      7 is the RTCC array depth, where
     * 0: Second (00-59)
     * 1: Minute (00-59)
     * 2: Hour (1-12 + AM/PM)
     * 3: Day (1-7)
     * 4: Date (1-31)
     * 5: Month (01-12)
     * 6: Year (0-99)
    */
    uint8_t rtcc_ret_array[7];
    uint16_t rtcc_cnt = 7;
    uint16_t start_addr = 0x00;
    
    int status = RTCC_Read(start_addr, rtcc_ret_array, rtcc_cnt);
    
    if (status == 1)
    {
        // We found success :)
        
        // We have to translate the values from their BCD
        //  to decimal values that are expected on our end
        
    }
    else
    {
        // We didn't find success :(
    }
    
    /*
    cTime.tm_sec = GetSecond();
    cTime.tm_min = GetMinute();
    cTime.tm_hour = GetHour();
    cTime.tm_mday = GetDay();
    cTime.tm_mon = GetMonth();
    cTime.tm_year = GetYear();
    */
    return cTime;
    
}
