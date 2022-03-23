/*!****************************************************************************************************************************************
 * @file         TcpServer.h
 *
 * @authors      Anselm Fuhrer  anselm.fuhrer@sourceengineers.com
 *
 * @brief        Communicator for the Communication over TCP
 *
 *****************************************************************************************************************************************/

#ifndef SE_SCOPE_SERVER_TCPSERVER_H
#define SE_SCOPE_SERVER_TCPSERVER_H

#include <netdb.h>
#include <mutex>
#include <vector>
#include <sys/select.h>

class TcpServer {
public:

    /**
     * @brief initializes
     * @param portNumber
     */
    TcpServer(int portNumber);

    /**
    * @brief  initializes the TCP communication
    *
    * sets the Socket File Descriptor, opens an internet
    * socket and configures it
    *
    * @returns 1 on sucess, 0 on failure
    */
    int initialize();



    /**
    * @brief reads data from the connection file-descriptor
    *
    * uses select() to create a timeout to avoid blocking the communication
    * when nothing can be read
    *
    * @return how many bytes were received
    */
    std::vector<uint8_t> receive();

    /**
    * @brief transmits data via TCP
    * @returns number of bytes written
    */
    int transmit(std::vector<uint8_t> data);

    int resetConnection();

    bool isConnectionEstablished();

private:
    int startListening(void);

    int mSocketFd;                               /**< File Descriptor for the socket */
    int mConnectionFd;                           /**< File Descriptor for the connection */
    struct sockaddr_in mServSockAddr;            /**< Structure describing the Internet socket address*/
    int mPort;
    struct timeval mTimeout;
    bool mIsConnectionEstablished;
    int mInputDataLength;
    std::mutex mCommunicationMutex;    /**< mutex for the communication */
};


#endif //SE_SCOPE_SERVER_TCPSERVER_H
