
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/utilities.h"
#include "mcc_generated_files/queue.h"
#include "mcc_generated_files/interrupt_handlers.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/tmr5.h"

/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();

    InitQueues(); // Start ADC queues
    InitIOCInterrupt(); // Initialize IOC Interrupts

    I2C_Init(); // Call custom I2C Init function to start the bus
    
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
        UART_Write_Buffer(TextMessageString, sizeof(TextMessageString));
    }

    return -1;
}
/**
 End of File
 */