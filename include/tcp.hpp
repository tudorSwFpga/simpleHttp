
#include <arpa/inet.h>
#include <errno.h>
#include <list>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#pragma once

class TcpClient {
public:
  TcpClient(const std::string &name);
  // Deleted copy constructor and copy assignment
  // we cannot copy the object because it uses a socket(file descriptor)
  TcpClient(const TcpClient &) = delete;
  TcpClient &operator=(const TcpClient &) = delete;
  // move constructor
  TcpClient(TcpClient &&other) noexcept;
  ~TcpClient();

  const std::string m_name;
  /**
   * @brief Establishes a connection to a remote server.
   *
   * This method creates a socket and attempts to connect to the specified host
   * and port. If the socket creation fails, it throws a runtime error with the
   * appropriate error message.
   *
   * @param remPort The remote port to connect to.
   * @param host The host to connect to.
   * @return int Returns 0 on success, or -1 on failure.
   */
  int connect(const uint16_t &remPort, const std::string &host);
  /**
   * @brief Sends a message to the connected remote server.
   *
   * This method sends the specified message to the remote server using the
   * established TCP connection.
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
  bool isConnected() const;
  int disconnect();

private:
  int sockfd_;
  int socketReuse_;
  uint16_t getMyPort();
  uint16_t getPeerPort();
};