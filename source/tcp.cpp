
#include "../include/tcp.hpp"
#include <chrono>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>

TcpClient::TcpClient(const std::string &name) : m_name(name) {
  spdlog::debug(" TCP Client Ctor {}", m_name);
  sockfd_ = -1;
}

TcpClient::TcpClient(TcpClient &&other) noexcept
    : sockfd_(std::move(other.sockfd_)), m_name(std::move(other.m_name)) {
  spdlog::debug(" TCP Client Move Ctor");
  other.sockfd_ = -1; // Invalidate the old object
}

TcpClient::~TcpClient() { spdlog::debug("TCP Client Dtor "); }

// Check if connected
bool TcpClient::isConnected() const {
  int error = 0;
  socklen_t len = sizeof(error);
  return getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &error, &len) == 0 &&
         error == 0;
}

int TcpClient::connect(const uint16_t &remPort, const std::string &host) {
  // first check if the socket is already connected following a move constructor
  if (isConnected()) {
    spdlog::debug("TcpClient {0} already connected", m_name);
    return 0;
  }
  // get the host address
  struct addrinfo hints{}, *res;
  hints.ai_family = AF_INET;
  if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) {
    spdlog::error("Failed to resolve host address\n");
    return -1;
  }
  // create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    spdlog::error("Failed to open socket");
    return -1;
  }
  sockfd_ = sockfd;
  // connect to the remote host
  sockaddr_in addr{};
  if (res->ai_family == AF_INET) {
    memset(&addr, 0, sizeof(sockaddr_in));
    std::memcpy(&addr.sin_addr.s_addr,
                &reinterpret_cast<sockaddr_in *>(res->ai_addr)->sin_addr,
                sizeof(in_addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(remPort);
  }
  freeaddrinfo(res);
  spdlog::debug("TcpClient {0} attempting connection {1}: {2} ", m_name, host,
                remPort);
  auto ret =
      ::connect(sockfd, (const struct sockaddr *)&addr, sizeof(sockaddr_in));
  return ret;
}

int TcpClient::send(const std::string &msg) {
  size_t totalSent = 0;
  while (totalSent < msg.size()) {
    ssize_t sent =
        ::send(sockfd_, msg.c_str() + totalSent, msg.size() - totalSent, 0);
    if (sent < 0) {
      spdlog::error("Send failed: {0}", strerror(errno));
      close(sockfd_);
      return -1;
    }
    totalSent += sent;
  }
  return totalSent;
}

int TcpClient::recv(std::string &msg) {
  char buffer[4096];
  int bytesRead = 0;
  // set the timeout for the recv call
  fd_set read_fds;
  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;
  FD_ZERO(&read_fds);
  FD_SET(sockfd_, &read_fds);
  // receive the data
  bytesRead = ::recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    msg += buffer;
    spdlog::info("TcpClient::recv() {0} bytes", bytesRead);
  } else {
    spdlog::error("recv() failed: {0}", strerror(errno));
  }
  return bytesRead;
}

int TcpClient::disconnect() {
  spdlog::debug("TcpClient disconnecting");
  return close(sockfd_);
}
