/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on September 25, 2015, 9:51 AM
 */


#include "xc.h"
#include "mcc.h"
#include "interrupt_handlers.h"

#define FCY         2000000UL // Instruction cycle frequency
#include <libpic30.h>

void DelayUS(int us)
{
    __delay_us(us);
}
void DelayMS(int ms)
{
    __delay_ms(ms);
}

void DelayS(int s)
{
    DelayMS(s * 1000);
}
bool IsSimOn(void)
{
    return simStatus_GetValue();
}

bool IsSimOnNetwork(void)
{
    return isNetlightOn;
}

bool IsThereWater(void)
{
    return isWaterPresent;
}
