/* 
 * File:   eeprom.h
 * Author: Ken Kok
 * Comments: eeprom driver header file
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  

/**
 Types
 */
typedef enum {
    EEPROM_SUCCESS = 0x00,
    EEPROM_ADDR_OOR = 0x01,
    EEPROM_ADDR_ODD = 0x02
} EEPROM_STATUS;

typedef enum {
    ERASE_ONE,
    ERASE_FOUR,
    ERASE_EIGHT,
    ERASE_ALL
} EEPROM_ERASE_COMMAND;

typedef unsigned int address_t;

/**
 Private Constants
 */
#define ADDRESS_MAX_SIZE            255

/**
 Private Functions
 */
static void eraseUsingNvmcon(int NVMCONvalue, address_t address);

/**
 Public Variables
 */
int __attribute__((space(eedata))) eeData = 0x1234;

/**
 Public Functions
 */
EEPROM_STATUS readEEprom(address_t address, int *dataPtr);
EEPROM_STATUS writeEEprom(address_t address, int data);
EEPROM_STATUS eraseEEprom(address_t address, EEPROM_ERASE_COMMAND cmd);


#endif	/* XC_HEADER_TEMPLATE_H */

