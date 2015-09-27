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
    

    while (1) {
        // Add your application code
        
        if(isMidnightPassed)
        {
            // We need to send our midnight message
            turnOnSim();
            
            while(!IsSimOnNetwork())
            {
                // The sim is not online yet
            }
            
            // Enter text mode
            sendUART1("AT+CMGF=1/r/n", 13);
            DelayMS(100); // Delay to give SIM time to switch
            sendUART1("AT+CMGS=\"", 9);
            sendUART1(phoneNumber, sizeof(phoneNumber));
            sendUART1("\"\r\n", 5);
            DelayMS(100);
            sendUART1(TextMessageString, sizeof(TextMessageString));
            
            DelayMS(5000);
            turnOffSim();
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