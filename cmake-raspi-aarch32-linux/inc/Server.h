
#ifndef SE_SCOPE_LINUX_SERVER_H
#define SE_SCOPE_LINUX_SERVER_H


#include "Communication/Communicator.h"
#include "Communication/UartCommunicator.h"

class Server {

#define INPUT_BUFFER_SIZE 300

#define INTERVAL_TIMESTAMP 1000
#define INTERVAL_SCOPE 5
#define INTERVAL_LISTENER 100
#define INTERVAL_STACK 50
#define INTERVAL_TRANSMIT 100
#define INTERVAL_LOG ( 1000*1000*1 )

public:
    Server();

    void start(void);

    //threads
    void *runStack(void);
    void *runScope(void);
    void *runListener(void);
    void *runTransmit(void);
    void *runLog();

    //helper functions for the threads
    static void *runStackHelper(void *context);
    static void *runScopeHelper(void *context);
    static void *runListenerHelper(void *context);
    static void *runTransmitHelper(void *context);
    static void *runLogHelper(void *context);

private:
    char mInputData[INPUT_BUFFER_SIZE];
    int mInputDataLength;
    pthread_mutex_t mUartMutex;
    UartCommunicator mCommunicator;
    uint32_t mTimestamp;
    ScopeFramedStackHandle mScopeStack;
    ITransceiverHandle mTransceiver;
};


#endif //SE_SCOPE_LINUX_SERVER_H
