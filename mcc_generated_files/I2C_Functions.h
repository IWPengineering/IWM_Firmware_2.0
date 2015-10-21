// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_FUNCTIONS_H
#define	I2C_FUNCTIONS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "constants.h"

typedef struct time_s time_s;

struct time_s {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t wkDay;
    uint8_t mnDay;
    uint8_t month;
    uint8_t year;
};

typedef enum {
            I2C_SOFTWARE_RESET = 0x02,
            I2C_COLLISION_DETECT = 0x04,
            I2C_SUCCESS = 0x01,
            I2C_NO_TRY = 0x00
} I2C_STATUS;

void I2C_Init(void);
time_s I2C_GetTime(void);
void SoftwareReset(void);
I2C_STATUS IdleI2C(void);
I2C_STATUS StartI2C(void);
I2C_STATUS StopI2C(void);
I2C_STATUS RestartI2C(void);
I2C_STATUS NackI2C(void);
I2C_STATUS AckI2C(void);
I2C_STATUS WriteI2C(unsigned char data);
I2C_STATUS ReadI2C(uint8_t *dataPtr, bool isEoT);
I2C_STATUS TurnOffRTCCOscillator(void);
I2C_STATUS SetRTCCTime(time_s *curTime);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

