#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/interrupt_handlers.h"
#include "mcc_generated_files/rtcc_handler.h"
#include "mcc_generated_files/utilities.h"

/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    InitIOCInterrupt(); // Initialize IOC Interrupts
    InitRTCC(); // Initialize I2C RTCC
    
    TMR1_Start();
    TMR2_Start();
    TMR3_Start();
    TMR4_Start();
    TMR5_Start();
    
    InitBufferStacks();
    
    while (1) {
        // Add your application code
        
        KickWatchdog(); // Reset the watchdog timer
        
        if(isMidnightPassed)
        {
            sendMidnightMessage();
        }
        
        if(xAxisBufferIsFull && yAxisBufferIsFull)
        {
            handleAccelBufferEvent();
        }
        
        if(depthBufferIsFull)
        {
            
        }
        
        if(batteryBufferIsFull)
        {
            handleBatteryBufferEvent();
        }
    }

    return -1;
}
/**
 End of File
 */