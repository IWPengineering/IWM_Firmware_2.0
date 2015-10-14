
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/utilities.h"
#include "mcc_generated_files/queue.h"
#include "mcc_generated_files/interrupt_handlers.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/tmr5.h"
time_s StartTime = { // All values in BCD
    0x30, // seconds
    0x25, // minutes
    0x21, // hours
    0x04, // wkDay
    0x14, // mnDay
    0x10, // month
    0x15  // year
};
/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();

    InitQueues(); // Start ADC queues
    InitIOCInterrupt(); // Initialize IOC Interrupts

    I2C_Init(); // Call custom I2C Init function to start the bus

    SetRTCCTime(&StartTime); // Set the current time on the MCP7940
    CurrentTime = StartTime; // Set this so our first run through doesn't trigger
                             //  a message send
    
    UART_Init();
    
    TMR1_Start();
    TMR2_Start();
    TMR3_Start();
    TMR4_Start();
    TMR5_Start();
    
    while (1) 
    {
        
        KickWatchdog(); // Reset the watchdog timer
        
        if(isMidnightPassed)
        {
            SendMidnightMessage();
        }
        
        if(!uint16_IsQueueEmpty(&xQueue) && !uint16_IsQueueEmpty(&yQueue))
        {
            ProcessAccelQueue();
        }
        
        if(depthBufferIsFull)
        {
            
        }
        
        if(batteryBufferIsFull)
        {
            HandleBatteryBufferEvent();
        }
        
        DelayMS(1000);
        //SendUART1("Hello! I'm on! :)", sizeof("Hello! I'm on! :)") - 1);
        UART_Write_Buffer(&TextMessageString[0], sizeof(TextMessageString));
        UART_Write_Buffer("\r\n", sizeof("\r\n"));
    }

    return -1;
}
/**
 End of File
 */