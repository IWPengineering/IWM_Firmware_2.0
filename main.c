#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/utilities.h"
#include "mcc_generated_files/queue.h"
#include "mcc_generated_files/interrupt_handlers.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/tmr5.h"
time_s StartTime = { // All values in BCD
    30, // seconds
    58, // minutes
    23, // hours
    04, // wkDay
    14, // mnDay
    10, // month
    15  // year
};
/*
                         Main application
 */
int main(void) {
    InitQueues(); // Start ADC queues
    // initialize the device
    SYSTEM_Initialize();

    
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
    
    SendTextMessage("I'm alive!", sizeof("I'm alive!"), 
            phoneNumber, sizeof(phoneNumber));

    
    while (1) 
    {

        KickWatchdog(); // Reset the watchdog timer
        
        if(false)//if(isMidnightPassed)
        {
            SendMidnightMessage();
            isMidnightPassed = false;
        }
        //This is a comment
        if((!uint16_IsQueueEmpty(&xQueue)) && (!uint16_IsQueueEmpty(&yQueue)))
        {
            ProcessAccelQueue();
        }
        
        if(depthBufferIsFull)
        {
            depthBufferIsFull = false;
        }
        
        if(batteryBufferIsFull)
        {
            HandleBatteryBufferEvent();
            batteryBufferIsFull = false;
        }
    }

    return -1;
}

void (*getErrLoc(void))(void); // Get Address Error Location
void (*errLoc)(void);          // Function Pointer

void __attribute__((interrupt, no_auto_psv)) _AddressError(void)
{
    errLoc = getErrLoc();
    
    INTCON1bits.ADDRERR = 0;
    //errLoc();
}

void __attribute__((interrupt, no_auto_psv)) _StackError(void)
{
    while(1);
}

void __attribute__((interrupt, no_auto_psv)) _MathError(void)
{
    while(1);
}

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail(void)
{
    while(1);
}

void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    while(1);
}
/**
 End of File
 */