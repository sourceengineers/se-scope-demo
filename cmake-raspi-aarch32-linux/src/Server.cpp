//
// Created by anselm on 17.05.21.
//

#include <iostream>
#include <unistd.h>

extern "C"{
    #include <Scope/Builders/ScopeFramedStack.h>
    #include <Scope/Communication/ITransceiver.h>
    #include <Scope/Control/AnnounceStorage.h>
    #include <se-lib-c/container/ByteRingBuffer.h>
    #include <se-lib-c/osal/IMutex.h>
    #include <se-lib-c/stream/BufferedByteStream.h>
    #include <se-lib-c/stream/ThreadSafeByteStream.h>
    #include <LinuxMutex.h>
}
#include "../inc/Server.h"

Server::Server()
{
    this->mCommunicator.setUartPath("/dev/serial0");
    mCommunicator.initializeCommunicator();
    this->mInputDataLength = 0;
}

void Server::start(void){
    this->mTimestamp = 0;

    int16_t flipflop = 1 ;
    int16_t const_1 = 1;
    float adc_value;

    pthread_mutex_init(&this->mUartMutex, NULL);

    LinuxMutexHandle configMutex = LinuxMutex_create();
    LinuxMutexHandle dataMutex = LinuxMutex_create();
    LinuxMutexHandle logMutex = LinuxMutex_create();

    ScopeFramedStackMutex mutexes = {
            .configMutex = LinuxMutex_getIMutex(configMutex),
            .dataMutex = LinuxMutex_getIMutex(dataMutex),
            .logBufferMutex = LinuxMutex_getIMutex(logMutex)
    };

    //Scope setup
    ScopeFramedStackConfig config = {
            .sizeOfChannels = 50,
            .amountOfChannels = 3,
            .callback = NULL,   // might also use a callback, but could not get it to work
            .timestamp = &mTimestamp,
            .addressesInAddressAnnouncer = 3,
            .timebase = 0.001f,
    };

    //  LOGGER
    IByteStreamHandle  _logByteStream;
    size_t _logBufferSize = 1024;
    size_t _logMessageSize = 300;

    BufferedByteStreamHandle stream = BufferedByteStream_create(512);
    ThreadSafeByteStreamHandle bufferedStream = ThreadSafeByteStream_create(LinuxMutex_getIMutex(logMutex), BufferedByteStream_getIByteStream(stream));

    Message_Priorities priorities = {
            .data = HIGH,
            .log = MEDIUM,
            .stream = LOW,
    };

    _logByteStream = ThreadSafeByteStream_getIByteStream(bufferedStream);
    ScopeFramedStackLogOptions scopeLogOptions = {
            .logByteStream = _logByteStream
    };

    this->mScopeStack = ScopeFramedStack_createThreadSafe(config, mutexes, scopeLogOptions, priorities);
    this->mTransceiver = ScopeFramedStack_getTranscevier(this->mScopeStack);

    //Announce the signals
    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(mScopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "flipflop", &flipflop, SE_INT16);
    AnnounceStorage_addAnnounceAddress(addressStorage, "const_1", &const_1, SE_INT16);
    AnnounceStorage_addAnnounceAddress(addressStorage, "ADC_0", &adc_value, SE_FLOAT);

    //start the threads
    pthread_t threadHandleInput;
    pthread_create(&threadHandleInput, NULL, &Server::runListenerHelper, this);
    pthread_t threadHandleStack;
    pthread_create(&threadHandleStack, NULL, &Server::runStackHelper, this);
    pthread_t threadHandleScope;
    pthread_create(&threadHandleScope, NULL, &Server::runScopeHelper, this);
    pthread_t threadHandleTransmit;
    pthread_create(&threadHandleTransmit, NULL, &Server::runTransmitHelper, this);
    pthread_t threadHandleLog;
    pthread_create(&threadHandleLog, NULL, &Server::runLogHelper, this);

    while(true){
        mTimestamp = mTimestamp + 1;
        flipflop = flipflop * -1;
        usleep(INTERVAL_TIMESTAMP);
    }
//    pthread_cancel(threadHandleScope);
}


void* Server::runScope(void){
    while(true){
        ScopeFramedStack_runThreadScope(this->mScopeStack);
        usleep(INTERVAL_SCOPE);
    }
}

/**
 * @brief receive data from the communicator
 */
void* Server::runListener(void){
    while(true){
        if(pthread_mutex_lock(&this->mUartMutex) == 0) {
            this->mInputDataLength = this->mCommunicator.receive(this->mInputData, INPUT_BUFFER_SIZE);
            if(this->mInputDataLength > 0){
                std::cout << " Received " << this->mInputDataLength << " Bytes" << std::endl;
            }
            pthread_mutex_unlock(&this->mUartMutex);
            usleep(INTERVAL_LISTENER);
        }
    }
}

/**
 * @brief runs the scope stack
 *
 * if there is data in the input data buffer, it gives it to the transceiver and
 * runs the Scope Stack
 */
void* Server::runStack(void) {
    while(true){
        if(pthread_mutex_trylock(&this->mUartMutex) == 0){
            if(this->mInputDataLength > 0){
                this->mTransceiver->put(mTransceiver, (uint8_t*) this->mInputData, this->mInputDataLength);
                this->mInputDataLength = 0;
            }
            ScopeFramedStack_runThreadStack(this->mScopeStack);
            pthread_mutex_unlock(&this->mUartMutex);
            usleep(INTERVAL_STACK);
        }
    }
}

/**
 * @brief sends the data from the transceiver via communicator
 *
 */
void* Server::runTransmit() {
    while(true){
        if(pthread_mutex_trylock(&this->mUartMutex) == 0) {
            const size_t length = this->mTransceiver->outputSize(this->mTransceiver);
            uint8_t data[length];
            if (length > 0) {
                this->mTransceiver->get(this->mTransceiver, data, length);
                this->mCommunicator.transmit((char *) data, length);
            }
            pthread_mutex_unlock(&this->mUartMutex);
            usleep(INTERVAL_TRANSMIT);
        }
    }
}

/**
 * @brief Logs a message every second
 */
void* Server::runLog() {
    int log_counter = 0;
    char log_msg [100];
    while(true){
        sprintf(log_msg, "Log Message num %d\n\r", log_counter);
        log_counter++;
        usleep(INTERVAL_LOG);
    }
}

//  ****************************************************************
//  Helper Functions for the threads
//  ****************************************************************
void *Server::runStackHelper(void* context){
    return ((Server *)context)->runStack();
}

void *Server::runScopeHelper(void* context){
    return ((Server *)context)->runScope();
}

void  *Server::runListenerHelper(void* context){
    return ((Server *)context)->runListener();
}

void *Server::runTransmitHelper(void *context) {
    return ((Server *)context)->runTransmit();
}

//static
void *Server::runLogHelper(void *context) {
    return ((Server *)context)->runLog();
}

int main(){
    Server server;
    std::cout << "Starting the server " << std::endl;
    server.start();
}
