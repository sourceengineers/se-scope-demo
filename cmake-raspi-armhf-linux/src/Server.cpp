//
// Created by anselm on 17.05.21.
//

#include <iostream>
#include <unistd.h>
#include <Server.h>
#include <thread>
#include <LinuxMutex.h>
#include <math.h>

extern "C"{
    #include <Scope/Builders/ScopeFramedStack.h>
    #include <Scope/Communication/ITransceiver.h>
    #include <Scope/Control/AnnounceStorage.h>
    #include <se-lib-c/container/ByteRingBuffer.h>
    #include <se-lib-c/osal/IMutex.h>
    #include <se-lib-c/stream/BufferedByteStream.h>
    #include <se-lib-c/stream/ThreadSafeByteStream.h>
}

constexpr size_t LOG_BUFFER_SIZE = 256;
constexpr size_t INTERVAL_STACK = 1000; // 1 ms
constexpr size_t INTERVAL_TIMESTAMP = 1000;

Server::Server(): timestamp(0), tcpServer(8080), seesaw(0), sine(0)
{
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
            .callback = NULL,
            .timestamp = &timestamp,
            .addressesInAddressAnnouncer = 3,
            .timebase = 0.001f,
    };

    BufferedByteStreamHandle stream = BufferedByteStream_create(LOG_BUFFER_SIZE);
    ThreadSafeByteStreamHandle bufferedStream = ThreadSafeByteStream_create(
            LinuxMutex_getIMutex(logMutex),
            BufferedByteStream_getIByteStream(stream));

    Message_Priorities priorities = {
            .data = HIGH,
            .log = MEDIUM,
            .stream = LOW,
    };

    logByteStream = ThreadSafeByteStream_getIByteStream(bufferedStream);
    ScopeFramedStackLogOptions scopeLogOptions = {
            .logByteStream = logByteStream
    };

    scopeStack = ScopeFramedStack_createThreadSafe(config, mutexes, scopeLogOptions, priorities);

    //Announce the signals
    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(scopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "seesaw", &seesaw, SE_INT16);
    AnnounceStorage_addAnnounceAddress(addressStorage, "sine", &sine, SE_FLOAT);
}

[[noreturn]] void Server::start(){

    int status = tcpServer.initialize();
    if (status < 0) {
        std::cerr << "Failed to open path to serial port" << std::endl;
        exit(status);
    }

    std::thread stackThread([this] {
        ITransceiver* transceiver = ScopeFramedStack_getTranscevier(scopeStack);
        while(true){
            auto bytes = tcpServer.receive();
            transceiver->put(transceiver->handle, bytes.data(), bytes.size());

            ScopeFramedStack_runThreadStack(scopeStack);

            size_t outputLength = transceiver->outputSize(transceiver->handle);
            if (outputLength > 0) {
                std::vector<uint8_t> output;
                uint8_t b;
                for (size_t i = 0; i < outputLength; ++i) {
                    transceiver->get(transceiver->handle, &b, 1);
                    output.push_back(b);
                }

                tcpServer.transmit(output);
            }

            usleep(INTERVAL_STACK);
        }
    });

    uint32_t logDelay = 0;
    while(true){
        timestamp = timestamp + 1;
        seesaw = (seesaw + 1)  % 500;
        sine = sinf((float) timestamp);
        logDelay += 1;

        // One second
        char log[LOG_BUFFER_SIZE];
        if (logDelay > 1000) {
            logDelay = 0;
            int len = snprintf(log, LOG_BUFFER_SIZE, "Timestamp: %u\n", timestamp);
            if (len > 0) {
                logByteStream->write(logByteStream->handle, (const uint8_t*) log, len);
            }
        }

        ScopeFramedStack_runThreadScope(scopeStack);
        usleep(INTERVAL_TIMESTAMP);
    }
}
