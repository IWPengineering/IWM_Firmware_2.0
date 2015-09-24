/**
  TMR4 Generated Driver API Header File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.h

  @Summary
    This is the generated header file for the TMR4 driver using MPLAB� Code Configurator

  @Description
    This header file provides APIs for driver for TMR4. 
    Generation Information : 
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
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

#ifndef _TMR4_H
#define _TMR4_H

/**
  Section: Included Files
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

#define TMR4_INTERRUPT_TICKER_FACTOR    1

    /**
      Section: Interface Routines
     */

    /**
      @Summary
        Initializes hardware and data for the given instance of the TMR module

      @Description
        This routine initializes hardware for the instance of the TMR module,
        using the hardware initialization given data.  It also initializes all
        necessary internal data.

      @Param
        None.

      @Returns
        None
 
      @Example 
        <code>
        bool statusTimer1;
        uint16_t period;
        uint16_t value;

        period = 0x20;

        TMR4_Initialize();

        TMR4_Period16BitSet(period);

        if((value = TMR4_Period16BitGet())== period)
        {
            TMR4_Start();
        }

        while(1)
        {
            TMR4_Tasks();
            if( (statusTimer1 = TMR4_GetElapsedThenClear()) == true)
            {
                TMR4_Stop();
            }
        }
        </code>
     */
    void TMR4_Initialize(void);
    /**
        void DRV_TMR4_Initialize(void)
     */
    void DRV_TMR4_Initialize(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Initialize instead. ")));


    /**
      @Summary
        Updates 16-bit timer value

      @Description
        This routine updates 16-bit timer value

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    void TMR4_Period16BitSet(uint16_t value);
    /**
        void DRV_TMR4_Period16BitSet(uint16_t value)
     */
    void DRV_TMR4_Period16BitSet(uint16_t value) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Period16BitSet instead. ")));
    /**

      @Summary
        Provides the timer 16-bit period value

      @Description
        This routine provides the timer 16-bit period value

      @Param
        None.

      @Returns
        Timer 16-bit period value
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    uint16_t TMR4_Period16BitGet(void);
    /**
        uint16_t DRV_TMR4_Period16BitGet(void)
     */
    uint16_t DRV_TMR4_Period16BitGet(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Period16BitGet instead. ")));
    /**
      @Summary
        Updates the timer's 16-bit value

      @Description
        This routine updates the timer's 16-bit value

      @Param
        None.

      @Returns
        None

      @Example 
        <code>
        uint16_t value=0xF0F0;

        TMR4_Counter16BitSet(value));

        while(1)
        {
            TMR4_Tasks();
            if( (value == TMR4_Counter16BitGet()))
            {
                TMR4_Stop();
            }
        }
        </code>
     */

    void TMR4_Counter16BitSet(uint16_t value);
    /**
        void DRV_TMR4_Counter16BitSet(uint16_t value)
     */
    void DRV_TMR4_Counter16BitSet(uint16_t value) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Counter16BitSet instead. ")));
    /**
      @Summary
        Provides 16-bit current counter value

      @Description
        This routine provides 16-bit current counter value

      @Param
        None.

      @Returns
        16-bit current counter value
 
      @Example 
        Refer to the example of TMR4_Counter16BitSet();
     */

    uint16_t TMR4_Counter16BitGet(void);
    /**
        uint16_t DRV_TMR4_Counter16BitGet(void)
     */
    uint16_t DRV_TMR4_Counter16BitGet(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Counter16BitGet instead. ")));

    /**
      @Summary
        Callback for timer interrupt.

      @Description
        This routine is callback for timer interrupt

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    void TMR4_CallBack(void);

    /**
      @Summary
        Starts the TMR

      @Description
        This routine starts the TMR

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    void TMR4_Start(void);
    /**
        void DRV_TMR4_Start(void)
     */
    void DRV_TMR4_Start(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Start instead. ")));
    /**
      @Summary
        Stops the TMR

      @Description
        This routine stops the TMR

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    void TMR4_Stop(void);
    /**
        void DRV_TMR4_Stop(void)
     */
    void DRV_TMR4_Stop(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_Stop instead. ")));
    /**
      @Summary
        Returns the elapsed status of the timer and clears if flag is set.

      @Description
        This routine returns the elapsed status of the timer and clears 
        flag if its set.

      @Param
        None.

      @Returns
        True - Timer has elapsed.
        False - Timer has not elapsed.
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    bool TMR4_GetElapsedThenClear(void);
    /**
        bool DRV_TMR4_GetElapsedThenClear(void)
     */
    bool DRV_TMR4_GetElapsedThenClear(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_GetElapsedThenClear instead. ")));
    /**
      @Summary
        Returns the software counter value.

      @Description
        This routine returns the software counter value.

      @Param
        None.

      @Returns
        Software counter value.
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    uint8_t TMR4_SoftwareCounterGet(void);
    /**
        uint8_t DRV_TMR4_SoftwareCounterGet(void)
     */
    uint8_t DRV_TMR4_SoftwareCounterGet(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_SoftwareCounterGet instead. ")));
    /**
      @Summary
        Clears the software counter value.

      @Description
        This routine clears the software counter value.

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR4_Initialize();
     */

    void TMR4_SoftwareCounterClear(void);
    /**
        void DRV_TMR4_SoftwareCounterClear(void)
     */
    void DRV_TMR4_SoftwareCounterClear(void) __attribute__((deprecated("\nThis will be removed in future MCC releases. \nUse TMR4_SoftwareCounterClear instead. ")));
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif //_TMR4_H

/**
 End of File
 */
