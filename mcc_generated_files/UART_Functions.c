/*
 * File:   UART_Functions.c
 * Author: Ken Kok
 *
 * Created on October 12, 2015, 10:39 AM
 */


#include "xc.h"
#include "UART_Functions.h"

uint8_queue TX_Queue;
uint8_queue RX_Queue;

/**
 * Description: Initializes UART TX & RX queues, then sets UART config
 *                  bits for desired operation, as notated below.
 */
void UART_Init(void)
{
    // Assemble the queues
    uint8_InitQueue(&TX_Queue, TX_QUEUE_SIZE);
    uint8_InitQueue(&RX_Queue, RX_QUEUE_SIZE);
    
    // Set init blocks
    
    /*
     * UART enabled, Continue operation in idle, IrDA disabled,
     * UxRTS in Flow Control, UxCTS and UxRTS unused by UART,
     * Wake on start bit during sleep disabled, loopback disabled
     * autobaud disabled, reverse polarity disabled, high baud enabled,
     * parity and data selection = 8 bit, no parity, one stop bit
     */
    U1MODE  = 0x8008;
    
    /*
     * UART TX interrupt when there is a char open in TX Buffer, IrDA idle = 1
     * Sync break disabled, UART Transmit disabled, 
     * RX Interrupt when any char is put in receive buffer
     * Address mode disabled
     */
    U1STA   = 0x0000;
    
    // Baud rate set @ 38400
    U1BRG = 0x000C;
    
    // Enable interrupts for TX and RX
    IEC0bits.U1RXIE = 1;
    //IEC0bits.U1TXIE = 1; 
    
}

/**
 * Description: TX ISR Handler. Pulls elements from the queue, turns off
 *                  tx interrupts when the queue is empty.
 */
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    if(!uint8_IsQueueEmpty(&TX_Queue) && !U1STAbits.UTXBF)
    {
        // If there is at least one element in the queue, and the TX 
        //  buffer isn't full
        U1TXREG = uint8_PullQueue(&TX_Queue);
        //U1STAbits.UTXEN = 1;
    }
    else if(uint8_IsQueueEmpty(&TX_Queue) && U1STAbits.TRMT)
    {
        // If queue is empty and everything is transmitted, disable UTXEN
        U1STAbits.UTXEN = 0;
        IEC0bits.U1TXIE = 0;
        return;
    }
    // Clear the interrupt flag
    IFS0bits.U1TXIF = false;
}

/**
 * Description: RX ISR Handler. Pushes new data into the queue
 */
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    // Clear the interrupt flag
    IFS0bits.U1RXIF = false;
    
    if(!uint8_IsQueueFull(&RX_Queue) && U1STAbits.URXDA)
    {
        // If there is room in the queue and data on the bus
        uint8_PushQueue(&RX_Queue, U1RXREG);
    }
}

/**
 * Description: Function to write one byte of information to UART1.
 * @param byte: Information to write.
 * @return UART_STATUS enum, indicating if the function was successful.
 */
UART_STATUS UART_Write(char byte)
{
//    U1STAbits.UTXEN = 1;
//    if(uint8_IsQueueFull(&TX_Queue))
//    {
//        return TX_QUEUE_FULL;
//    }
//    else
//    {
//        uint8_PushQueue(&TX_Queue, (uint8_t)byte);
//        // Enabling TX will throw the TX Interrupt
//        
//        
//        if(IEC0bits.U1TXIE == false)
//        {
//            IEC0bits.U1TXIE = true;
//        }
//        
//        return TX_STARTED;
//    }
    
//     Non-Interrupt implementation
    U1STAbits.UTXEN = 1;
    if(!U1STAbits.UTXBF)
    {
        U1TXREG = byte;
        return TX_STARTED;
    }
    else
    {
        return TX_QUEUE_FULL;
    }
}

/**
 * Description: Write an entire buffer to UART. This function is blocking.
 * @param dataPtr: Pointer to data block to write
 * @param dataLen: Length of data block
 * @return UART_STATUS enum, indicating whether the function was successful.
 */
UART_STATUS UART_Write_Buffer(char *dataPtr, uint8_t dataLen)
{
    char *pD = dataPtr;
    int i;
    for(i = 0; i < dataLen; i++)
    {
        UART_STATUS stat = UART_Write((char)*pD);
        if(stat == TX_STARTED)
        {
            // If we started TX, then we are free to move to the next char
            pD++;              
        }
        else
        {
            i--; // Run the loop again
        }
        
    }
    
    return TX_STARTED;
}

/**
 * Description: Read the uart RX buffer into the specified buffer
 * @param dataPtr: Pointer to specified buffer
 * @param dataLen: Maximum length of data
 * @return uint8_t amount of data read from buffer
 */
uint8_t UART_Read(char *dataPtr, uint8_t dataLen)
{
    char *pD = dataPtr;
    
    int i;
    for(i = 0; i < dataLen; i++)
    {
        // If there are no entries, return how far we were
        if(uint8_IsQueueEmpty(&RX_Queue))
        {
            return i;
        }
        
        // Get an entry, put it in the pointer
        *pD = (char)uint8_PullQueue(&RX_Queue);
        // Then increment our pointer
        pD++;
    }
    
    return dataLen;
}
