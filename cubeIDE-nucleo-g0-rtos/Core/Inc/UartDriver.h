#include "usart.h"
#include "Scope/Communication/ITransceiver.h"
#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

void UartDriver_init(void);
void UartDriver_run(void);

void UART_CharReception_Callback(void);
void UART_TXEmpty_Callback(void);
void UART_CharTransmitComplete_Callback(void);
void UART_Error_Callback(void);
void transmit_data(ITransceiverHandle self);


#endif

