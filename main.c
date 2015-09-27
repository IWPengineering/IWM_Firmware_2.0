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
        
        KickWatchdog(); // Reset the watchdog timer
        
        if(isMidnightPassed)
        {
            // We need to send our midnight message
            turnOnSim();
            
            while(!IsSimOnNetwork())
            {
                // The sim is not online yet
                DelayMS(1); // Wait to check, and resets WDT
            }
            
            // Update values in text message
            updateMessageVolume();
            updateMessageBattery();
            updateMessagePrime();
            updateMessageLeakage();
            
            // Enter text mode
            sendUART1("AT+CMGF=1/r/n", 13);
            DelayMS(100); // Delay to give SIM time to switch
            sendUART1("AT+CMGS=\"", 9); // Start sending a text
            sendUART1(phoneNumber, sizeof(phoneNumber)); // Send phone number
            sendUART1("\"\r\n", 5); // end of phone number
            DelayMS(100);
            sendUART1(TextMessageString, sizeof(TextMessageString)); // Add message
            // We probably have to send an extra control char here
            
            // TODO: Teach it to listen for the SIM's response on RX, and 
            //  respond appropriately.
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