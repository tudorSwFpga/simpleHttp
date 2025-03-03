
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <list>
#include <errno.h>
#include <unistd.h>
#include "spdlog/spdlog.h"

#pragma once

class TcpClient {
public:
    TcpClient(const std::string &name) : m_name(name) {
        spdlog::debug(" TCP Client Ctor {}", m_sockfd);
    }
    ~TcpClient() {
        spdlog::debug("TCP Client Dtor ");
    }

    const std::string m_name;
    /**
    * @brief Establishes a connection to a remote server.
    * 
    * This method creates a socket and attempts to connect to the specified host and port.
    * If the socket creation fails, it throws a runtime error with the appropriate error message.
    * 
    * @param remPort The remote port to connect to.
    * @param host The host to connect to.
    * @return int Returns 0 on success, or -1 on failure.
    */
    int connect(const uint16_t &remPort, const std::string &host);
    /**
    * @brief Sends a message to the connected remote server.
    * 
    * This method sends the specified message to the remote server using the established TCP connection.
    * 
    * @param msg The message to be sent to the remote server.
    * @return int The number of bytes sent on success, or -1 on failure.
    */
    int send(const std::string &msg);
    /**
    * @brief Attempts receiving a message from the connected remote server.
    *  
    * @param msg The message received from the remote server.
    * @return int The number of bytes received on success, or -1/0 on failure.
    */
    int recv(std::string &msg);
    int disconnect();

private:
    int m_sockfd;
    uint16_t getMyPort();
    uint16_t getPeerPort();
    
};