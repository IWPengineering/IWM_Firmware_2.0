/*
 * File:   UART_Functions.c
 * Author: KSK0419
 *
 * Created on October 12, 2015, 10:39 AM
 */


#include "xc.h"
#include "UART_Functions.h"
#include "queue.h"

queue TX_Queue;
queue RX_Queue;

void UART_Init(void)
{
    // Assemble the queues
    InitQueue(TX_Queue, TX_QUEUE_SIZE);
    InitQueue(RX_Queue, RX_QUEUE_SIZE);
    
    // Set init blocks
    
    /*
     * UART enabled, Continue operation in idle, IrDA disabled,
     * UxRTS in Flow Control, UxCTS and UxRTS unused by UART,
     * Wake on start bit during sleep enabled, loopback disabled
     * autobaud disabled, reverse polarity disabled, high baud enabled,
     * parity and data selection = 8 bit, no parity, one stop bit
     */
    U1MODE  = 0x8088;
    
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
    IEC0bits.U1TXIE = 1; 
    
    // Enable transmit
    U1STAbits.UTXEN = 1;
}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    
}
