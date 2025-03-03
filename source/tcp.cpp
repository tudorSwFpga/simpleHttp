
#include <stdexcept>
#include <errno.h>
#include <cstring>
#include <chrono>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/tcp.hpp"
#include <netdb.h>


int TcpClient::connect(const uint16_t &remPort, const std::string &host)
{
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        spdlog::error("Failed to open socket");
        return -1;
    }

    struct hostent* server = gethostbyname(host.c_str());
    if (!server) {
        spdlog::error("Failed to resolve host: {0}", host);
        close(sockfd);
        return -1;
    }
    //set the server address and port
    memset(&addr, 0, sizeof(sockaddr_in)); 
    std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(remPort);

    m_sockfd = sockfd;
    spdlog::debug("TcpClient {0} attempting connection {1}: {2} ", m_name, host, remPort);
    auto ret = ::connect(sockfd, (const struct sockaddr *)&addr, sizeof(sockaddr_in));
    return ret;
}

int TcpClient::send(const std::string &msg)
{
    size_t totalSent = 0;
    while (totalSent < msg.size()) {
        ssize_t sent = ::send(m_sockfd, msg.c_str() + totalSent, msg.size() - totalSent, 0);
        if (sent < 0) {
            spdlog::error("Send failed: {0}", strerror(errno));
            close(m_sockfd);
            return -1;
        }
        totalSent += sent;
    }
    return totalSent;
}

int TcpClient::recv(std::string &msg)
{
    char buffer[4096];
    int bytesRead = 0;
    //set the timeout for the recv call
    fd_set read_fds;
    struct timeval timeout;
    timeout.tv_sec = 5;  
    timeout.tv_usec = 0;
    FD_ZERO(&read_fds);
    FD_SET(m_sockfd, &read_fds);
    //receive the data
    bytesRead = ::recv(m_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        msg += buffer;
        spdlog::error("TcpClient::recv() {0} bytes", bytesRead);
    } else {
        spdlog::error("recv() failed: {0}", strerror(errno));
    }
    return bytesRead;
}

int TcpClient::disconnect()
{
    spdlog::debug("TcpClient disconnecting");
    return close(m_sockfd);
}

/**
 * @brief Retrieves the port number of the local endpoint of the TCP connection.
 *
 * This function uses the getsockname system call to obtain the local address
 * and port number associated with the socket. It then extracts and returns
 * the port number.
 *
 * @return uint16_t The port number of the local endpoint.
 */
uint16_t TcpClient::getMyPort()
{
    struct sockaddr_in peerAddr;
    socklen_t socketSize = sizeof(peerAddr);
    ::getsockname(m_sockfd, (struct sockaddr *)&peerAddr, &socketSize);
    return peerAddr.sin_port;
}
