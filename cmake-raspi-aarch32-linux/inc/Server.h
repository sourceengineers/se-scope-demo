#ifndef SE_SCOPE_LINUX_SERVER_H
#define SE_SCOPE_LINUX_SERVER_H

extern "C" {
    #include <Scope/Builders/ScopeFramedStack.h>
};
#include "UartDriver.h"

class Server {

public:
    Server(const std::string& serialPath);

    [[noreturn]] void start();

    //helper functions for the threads

private:
    UartDriver uartDriver;
    uint32_t timestamp;
    ScopeFramedStackHandle scopeStack;
    int32_t flipflop;
    float adc_value;
    std::string serial;
    IByteStreamHandle logByteStream;
};


#endif //SE_SCOPE_LINUX_SERVER_H
