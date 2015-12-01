/*
 * File:   eeprom.c
 * Author: KSK0419
 *
 * Created on December 1, 2015, 10:38 AM
 */


#include "xc.h"
#include "eeprom.h"

/**
 * Description: Reads a word of data from the EEProm
 * @param address: Address to read from (must be even)
 * @param dataPtr: Pointer to put data into
 * @return EEPROM_STATUS indicating if the function was successful
 */
EEPROM_STATUS readEEprom(address_t address, int *dataPtr)
{
    if(address > ADDRESS_MAX_SIZE)
    {
        return EEPROM_ADDR_OOR;
    }
    
    if(address % 2)
    {
        return EEPROM_ADDR_ODD;
    }
    
    EEPROM_STATUS stat = EEPROM_SUCCESS;
    
    unsigned int offset;
    
    TBLPAG = __builtin_tblpage(&eeData);
    offset = __builtin_tbloffset(&eeData) +
                (address & 0x1FF);
    
    *dataPtr = __builtin_tblrdl(offset);
    
    return stat;
}

/**
 * Description: Write a word to the specified address
 * @param address: Address to write to. Must be even
 * @param data: Data word to be written
 * @return EEPROM_STATUS indicating if the function was successful
 */
EEPROM_STATUS writeEEprom(address_t address, int data)
{
    if(address > ADDRESS_MAX_SIZE)
    {
        return EEPROM_ADDR_OOR;
    }
    
    if(address % 2)
    {
        return EEPROM_ADDR_ODD;
    }
    
    EEPROM_STATUS stat = EEPROM_SUCCESS;
    
    unsigned int offset;
    
    // Set up the write command
    NVMCON = 0x4004;
    
    // Find the right table page and offset
    TBLPAG = __builtin_tblpage(&eeData);
    offset = __builtin_tbloffset(&eeData) +
                (address & 0x01FF);
    
    // Set up a write to the correct page
    __builtin_tblwtl(offset, data);
    
    // Start the write with interrupts disabled, wait to finish
    asm volatile ("disi #5");
    
    __builtin_write_NVM();
    
    while(NVMCONbits.WR == 1);
    
    return stat;
}

/**
 * Description: Erase 1, 4, 8, or all words of EEPROM
 * @param address: Address to erase from. Must be even
 * @param cmd: Specify how many words should be erased
 * @return EEPROM_STATUS indicating whether the function was successful
 */
EEPROM_STATUS eraseEEprom(address_t address, EEPROM_ERASE_COMMAND cmd)
{
    if((cmd != ERASE_ALL) && (address > ADDRESS_MAX_SIZE))
    {
        return EEPROM_ADDR_OOR;
    }
    
    if((cmd != ERASE_ALL) && (address % 2))
    {
        return EEPROM_ADDR_ODD;
    }
    
    EEPROM_STATUS stat = EEPROM_SUCCESS;
    
    switch(cmd)
    {
        case ERASE_ALL:
        {
            // Set up NVMCON to bulk erase the data in EEPROM
            NVMCON = 0x4050;
            
            // Disable interrupts for 5 instructions
            asm volatile ("disi #5");
            
            // Issue unlock sequence and start erase cycle
            __builtin_write_NVM();
            
            break;
        }
        
        case ERASE_EIGHT:
        {
            eraseUsingNvmcon(0x4046, address);
            
            break;
        }
        
        case ERASE_FOUR:
        {
            eraseUsingNvmcon(0x4045, address);
            
            break;
        }
        
        case ERASE_ONE:
        {
            eraseUsingNvmcon(0x4044, address);
            
            break;
        }
        
        default:
        {
            // We should never make it here
            break;
        }
    }
    
    return stat;
}

/**
 * Description: Functionalize common erase functions - get the right table offset
 *                  and performing the erase based on NVMCON parameter
 * @param NVMCONvalue: Parameter to put into the NVMCON register pre-erase
 * @param address: Address to erase from. Must be even.
 */
static void eraseUsingNvmcon(int NVMCONvalue, address_t address)
{
    unsigned int offset;

    // Set up the EEProm command
    NVMCON = NVMCONvalue;

    // Get the right table page and offset location
    TBLPAG = __builtin_tblpage(&eeData);
    offset = __builtin_tbloffset(&eeData) + 
                (address & 0x01FF);
    __builtin_tblwtl(offset, offset);

    // Set up the write command, wait for it to finish
    asm volatile ("disi #5");
    __builtin_write_NVM();
}
