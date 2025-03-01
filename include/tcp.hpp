
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

    int connect(const uint16_t &remPort, const std::string &remIp);
    int send(const std::string &msg);
    int recv(std::string &msg);
    int disconnect();

private:
    int m_sockfd;
    uint16_t getMyPort();
    uint16_t getPeerPort();
    
};