//
// Created by anselm on 17.05.21.
//

#ifndef SE_SCOPE_LINUX_UARTCOMMUNICATOR_H
#define SE_SCOPE_LINUX_UARTCOMMUNICATOR_H


#include "Communicator.h"

class UartCommunicator : public Communicator{

public:
    void initializeCommunicator(void);
    void transmit(char* send_buffer, size_t length);
    int receive(char* rec_buffer, size_t buf_size);
    void setUartPath(char *path);

private:
    int fs;
    char uart_path[256];

};

#endif //SE_SCOPE_LINUX_UARTCOMMUNICATOR_H
