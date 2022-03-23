#ifndef SE_SCOPE_LINUX_SERVER_H
#define SE_SCOPE_LINUX_SERVER_H

extern "C" {
    #include <Scope/Builders/ScopeFramedStack.h>
};
#include <TcpServer.h>

class Server {

public:
    Server();

    [[noreturn]] void start();

    //helper functions for the threads

private:
    TcpServer tcpServer;
    uint32_t timestamp;
    ScopeFramedStackHandle scopeStack;
    int32_t flipflop{};
    float sine{};
    IByteStreamHandle logByteStream;
};


#endif //SE_SCOPE_LINUX_SERVER_H
