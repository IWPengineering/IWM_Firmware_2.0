// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_FUNCTIONS_H
#define	UART_FUNCTIONS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc.h"
#include "queue.h"

#define TX_QUEUE_SIZE       32
#define RX_QUEUE_SIZE       32

typedef enum {
            NO_TX_RX = 0x00,
            TX_STARTED = 0x01,
            TX_QUEUE_FULL = 0x02,
            TX_FAILED = 0x04,
            RX_START = 0x10,
            RX_QUEUE_EMPTY = 0x20,
            RX_FAILED = 0x40
} UART_STATUS;

extern uint8_queue TX_Queue;
extern uint8_queue RX_Queue;

void UART_Init(void);
UART_STATUS UART_Write(char byte);
UART_STATUS UART_Write_Buffer(char *dataPtr,
                            uint8_t dataLen);
uint8_t UART_Read(char *dataPtr, uint8_t dataLen);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

