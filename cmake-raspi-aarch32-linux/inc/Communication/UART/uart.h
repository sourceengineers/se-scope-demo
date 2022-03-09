//
// Created by anselm on 18.05.21.
//

#ifndef SE_SCOPE_SERVER_UART_H
#define SE_SCOPE_SERVER_UART_H

int open_uart(char*);
void setOptions(int);
void uart_transmit(int, char*, int);
int uart_rxBytes(int, char*, int);
void closeUart(int);

#endif //SE_SCOPE_SERVER_UART_H
