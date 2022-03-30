/*!****************************************************************************************************************************************
 * @file         TcpServer.cpp
 *
 * @authors      Anselm Fuhrer  anselm.fuhrer@sourceengineers.com
 *
 * @brief        Communicator for the TCP Communication
 *****************************************************************************************************************************************/

#include <TcpServer.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <bitset>

constexpr size_t BUFFER_SIZE = 5*1024;

TcpServer::TcpServer(int portNumber) {
    assert(portNumber<= 65535);
    mPort = portNumber;

    //set the timeout for listening
    mTimeout.tv_sec = 0;
    mTimeout.tv_usec = 0;
    mIsConnectionEstablished = false;
}


int TcpServer::initialize(){

//    Create Socket
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (mSocketFd < 0) {
        perror("Failed to open socket");
        return mSocketFd;
    }

    //initialize mServSockAddr to 0
    bzero(&mServSockAddr, sizeof(mServSockAddr));
    int opt = 1;
    if (setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return -1;
    }

    mServSockAddr.sin_family = AF_INET;
    mServSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mServSockAddr.sin_port = htons(mPort);

    startListening();   //blocks until a connection is opened

    return 0;
}


int TcpServer::startListening(){
    int success = 1;

    socklen_t len =  sizeof(mServSockAddr);
    if (bind(mSocketFd, (struct sockaddr *)&mServSockAddr,len) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }
    if ((listen(mSocketFd, 1)) != 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    int addrlen = sizeof(mServSockAddr);
    std::cout << "Waiting for a connection" << std::endl;
    mConnectionFd = accept(mSocketFd, (struct sockaddr *)&mServSockAddr, (socklen_t*)&addrlen);
    if (mConnectionFd < 0)
    {
        std::cerr << "Connection accept" << std::endl;
        success = 0;
    }
    else{
        std::cout << "Connection opened" << std::endl;
        mIsConnectionEstablished = true;
    }
    return success;
}

std::vector<uint8_t> TcpServer::receive() {

    if(!mIsConnectionEstablished){
        return {};
    }

    fd_set input;
    FD_ZERO(&input);
    FD_SET(mConnectionFd, &input);

    std::vector<uint8_t> data;

    int bytesReady = select(mConnectionFd + 1, &input, NULL, NULL, &mTimeout);
    if(bytesReady <= 0) {
        return {};
    }

    int bytesRead = -1;
    if(mCommunicationMutex.try_lock()) {
        uint8_t rec_buffer[BUFFER_SIZE];
        bytesRead = read(mConnectionFd, rec_buffer, bytesReady);

        if (bytesRead > 0) {
            data.insert(data.begin(), rec_buffer, rec_buffer + bytesRead);
            mCommunicationMutex.unlock();
        }
    }

    if(bytesRead == -1){
        std::cout << "Received FIN" << std::endl;
        mIsConnectionEstablished = false;
        std::cout << "Connection closed" << std::endl;
    }

    if(bytesReady == 1 && bytesRead == 0){
        mIsConnectionEstablished = false;
        std::cout << "Connection closed" << std::endl;
    }

    return data;
}


int TcpServer::transmit(std::vector<uint8_t> data) {
    if(!mIsConnectionEstablished){
        std::cerr << "Failed to send";
        return -1;
    }
    int bytesSent = -1;
    if(mCommunicationMutex.try_lock()){
        bytesSent = write(mConnectionFd, data.data(), data.size());
        if(bytesSent > 0){
            mIsConnectionEstablished = true;
        }
        mCommunicationMutex.unlock();
    }
    return bytesSent;
}

int TcpServer::resetConnection() {
    std::cout << "Resetting connection";
    initialize();
    return 0;
}

bool TcpServer::isConnectionEstablished() {
    return mIsConnectionEstablished;
}
