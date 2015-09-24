
/**
  TMR4 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated source file for the TMR4 driver using MPLAB® Code Configurator

  @Description
    This source file provides APIs for driver for TMR4. 
    Generation Information : 
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC24F32KA302
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.24
        MPLAB 	          :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

/**
  Section: Included Files
 */

#include <xc.h>
#include "tmr4.h"
#include "interrupt_handlers.h"

/**
  Section: Data Type Definitions
 */

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintainence of the hardware instance.

  @Description
    This defines the object required for the maintainence of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
 */

typedef struct _TMR_OBJ_STRUCT {
    /* Timer Elapsed */
    bool timerElapsed;
    /*Software Counter value*/
    uint8_t count;

} TMR_OBJ;

static TMR_OBJ tmr4_obj;

/**
  Section: Driver Interface
 */


void TMR4_Initialize(void) {
    //TSIDL disabled; TGATE disabled; TCS T4CK; TCKPS 1:64; T32 disabled; TON enabled; 
    T4CON = 0x8022;
    //TMR4 0; 
    TMR4 = 0x0000;
    //Period Value = 1800.000 s; PR4 28125; 
    PR4 = 0x6DDD;

    IFS1bits.T4IF = false;
    IEC1bits.T4IE = true;

    tmr4_obj.timerElapsed = false;

}

/**
    void DRV_TMR4_Initialize (void)
 */
void DRV_TMR4_Initialize(void) {
    TMR4_Initialize();
}

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt() {
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    TMR4_CallBack();

    //***User Area End

    tmr4_obj.count++;
    tmr4_obj.timerElapsed = true;
    IFS1bits.T4IF = false;
}

void TMR4_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

/**
    void DRV_TMR4_Period16BitSet (uint16_t value)
 */
void DRV_TMR4_Period16BitSet(uint16_t value) {
    TMR4_Period16BitSet(value);
}

uint16_t TMR4_Period16BitGet(void) {
    return ( PR4);
}

/**
    uint16_t DRV_TMR4_Period16BitGet (void)
 */
uint16_t DRV_TMR4_Period16BitGet(void) {
    return (TMR4_Period16BitGet());
}

void TMR4_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

/**
    void DRV_TMR4_Counter16BitSet (uint16_t value)
 */
void DRV_TMR4_Counter16BitSet(uint16_t value) {
    TMR4_Counter16BitSet(value);
}

uint16_t TMR4_Counter16BitGet(void) {
    return ( TMR4);
}

/**
    uint16_t DRV_TMR4_Counter16BitGet (void)
 */
uint16_t DRV_TMR4_Counter16BitGet(void) {
    return (TMR4_Counter16BitGet());
}

void TMR4_CallBack(void) {
    // Add your custom callback code here
    Timer4Handler();
}

void TMR4_Start(void) {
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC1bits.T4IE = true;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

/**
    void DRV_TMR4_Start (void)
 */
void DRV_TMR4_Start(void) {
    TMR4_Start();
}

void TMR4_Stop(void) {
    /* Stop the Timer */
    T4CONbits.TON = false;

    /*Disable the interrupt*/
    IEC1bits.T4IE = false;
}

/**
    void DRV_TMR4_Stop (void)
 */
void DRV_TMR4_Stop(void) {
    TMR4_Stop();
}

bool TMR4_GetElapsedThenClear(void) {
    bool status;

    status = tmr4_obj.timerElapsed;

    if (status == true) {
        tmr4_obj.timerElapsed = false;
    }
    return status;
}

/**
    bool DRV_TMR4_GetElapsedThenClear (void)
 */
bool DRV_TMR4_GetElapsedThenClear(void) {
    return (TMR4_GetElapsedThenClear());
}

uint8_t TMR4_SoftwareCounterGet(void) {
    return tmr4_obj.count;
}

/**
    uint8_t DRV_TMR4_SoftwareCounterGet (void)
 */
uint8_t DRV_TMR4_SoftwareCounterGet(void) {
    return (TMR4_SoftwareCounterGet());
}

void TMR4_SoftwareCounterClear(void) {
    tmr4_obj.count = 0;
}

/**
    void DRV_TMR4_SoftwareCounterClear (void)
 */
void DRV_TMR4_SoftwareCounterClear(void) {
    TMR4_SoftwareCounterClear();
}

/**
 End of File
 */
