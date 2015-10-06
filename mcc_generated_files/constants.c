/*
 * File:   constants.c
 * Author: KSK0419
 *
 * Created on October 6, 2015, 12:37 PM
 */


#include "xc.h"
#include "constants.h"

const float MKIILiterPerDegree = .002949606; // .169 L/Rad converted to L/Deg
// .169 L/Rad Specified in 1.0 Firmware "IWPUtilities.c"
const float UpstrokeToMeters = 0.01287;
const float MaxLitersToLeak = 0.01781283;

const float RadToDegrees = 57.2957914; // 180 / pi()
const int AdjustmentFactor = 2047; // 1/2 of 12 bit ADC