//
// Created by anselm on 17.05.21.
//

#include <cstring>
#include "../../inc/Communication/UartCommunicator.h"

extern "C" {
    #include "../../inc/Communication/UART/uart.h"
}

void UartCommunicator::initializeCommunicator(void){
    fs = open_uart(this->uart_path);
    setOptions(fs);
}

void UartCommunicator::transmit(char *send_buffer, size_t length) {
    uart_transmit(fs, send_buffer, length);
}

int UartCommunicator::receive(char *rec_buffer, size_t buf_size) {
    return uart_rxBytes(fs, rec_buffer, buf_size);
}

void UartCommunicator::setUartPath(char* path){
    strcpy(this->uart_path, path);
}