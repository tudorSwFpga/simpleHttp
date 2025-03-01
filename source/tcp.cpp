
#include <stdexcept>
#include <errno.h>
#include <cstring>
#include <chrono>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/tcp.hpp"

/**
 * @brief Establishes a connection to a remote server.
 * 
 * This method creates a socket and attempts to connect to the specified remote IP address and port.
 * If the socket creation fails, it throws a runtime error with the appropriate error message.
 * 
 * @param remPort The remote port to connect to.
 * @param remIp The remote IP address to connect to.
 * @return int Returns 0 on success, or -1 on failure.
 * @throws std::runtime_error If socket creation fails.
 */
int TcpClient::connect(const uint16_t &remPort, const std::string &remIp)
{
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        throw std::runtime_error(std::strerror(errno));
    }

    memset(&addr, 0, sizeof(sockaddr_in)); // init address to 0
    addr.sin_addr.s_addr = inet_addr(remIp.c_str());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(remPort);

    m_sockfd = sockfd;
    spdlog::debug("TcpClient {0} attempting connection to 127.0.0.1: {1} ", m_name, remPort);
    auto ret = ::connect(sockfd, (const struct sockaddr *)&addr, sizeof(sockaddr_in));
    return ret;
}

/**
* @brief Sends a message to the connected remote server.
* 
* This method sends the specified message to the remote server using the established TCP connection.
* It uses the send system call to transmit the data.
* 
* @param msg The message to be sent to the remote server.
* @return int The number of bytes sent on success, or -1 on failure.
*/
int TcpClient::send(const std::string &msg)
{
    spdlog::debug("TcpClient sending data");
    return ::send(m_sockfd, msg.c_str(), sizeof(msg), 0);
}

int TcpClient::recv(std::string &msg)
{
    char buffer[4096];
    int bytesRead = ::recv(m_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        msg = buffer;
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
