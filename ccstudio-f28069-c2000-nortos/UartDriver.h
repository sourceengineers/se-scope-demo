/*
 * UartDriver.h
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

#include "Scope/Communication/ITransceiver.h"
#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

typedef enum {
    ONE = 0,
    TWO = 1
} Stopbits;

typedef enum {
    ODD = 0,
    EVEN = 1
} ParityOddEven;

typedef enum {
    ENABLED = 1,
    DISABLED = 0
} ParityEnable;

typedef enum {
    SEVEN = 6,
    EIGHT = 7,
} CharCount;

typedef struct {
    uint32_t baudrate;
    CharCount charCount;
    ParityEnable parityEnable;
    ParityOddEven parityOddEven;
    Stopbits stopbits;
} UartConfig;


void UartDriver_setup();
void UartDriver_init(UartConfig config, size_t inputBufferSize, size_t outputBufferSize);
int UartDriver_read(uint16_t* buffer, size_t length);
int UartDriver_write(uint16_t* buffer, size_t length);



#endif
