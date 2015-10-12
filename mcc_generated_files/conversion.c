/*
 * File:   conversion.c
 * Author: KSK0419
 *
 * Created on October 12, 2015, 1:43 PM
 */


#include "xc.h"
#include <stdint.h>

uint8_t DecToBcd(uint8_t val)
{
	return ((val / 10 * 16) + (val % 10));
}

uint8_t BcdToDec(uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}
