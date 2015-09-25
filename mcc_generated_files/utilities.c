/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on September 25, 2015, 9:51 AM
 */


#include "xc.h"
#include "mcc.h"
#include "interrupt_handlers.h"

void DelayMS(int ms)
{
    
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
