#pragma once
#include "tcp.hpp"
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <sys/types.h>

enum class RequestStatus {
  SOCKET_CREATION_FAILED,
  CONNECTION_FAILED,
  SEND_FAILED,
  RECV_FAILED,
  DISCONNECT_FAILED,
  SUCCESS
};
/**
 * @struct Response
 * @brief A struct to hold the response from an HTTP GET request.
 * status: The status of the request, indicating eventual nework issues
 * ret_code: The return code of the request (e.g., 200, 404, etc.).
 */
struct Response {
  RequestStatus status;
  int ret_code;
};

using ResponseCallback = std::function<void(const std::string &)>;

/**
 * @class HttpClient
 * @brief A class for making HTTP GET requests.
 */
class HttpClient {

public:
  /**
   * @brief Constructs a new HttpClient object.
   * @param version The HTTP version to use (e.g., "1.0" or "1.1").
   * @param verbosity The verbosity level for logging.
   * @throws std::invalid_argument if the HTTP version is invalid.
   */
  explicit HttpClient(const std::string &version,
                      const std::string &verbosity = "info");

  // no copy constructor allowed because of tcpClient using an unique FD
  HttpClient(const HttpClient &other) = delete;
  /**
   * @brief Move constructor for HttpClient.
   * @param other The HttpClient object to move from.
   */
  HttpClient(HttpClient &&other) noexcept;
  HttpClient &operator=(const HttpClient &other) = delete;
  ~HttpClient();
  /**
   * @brief Makes an HTTP GET request to the specified host and path.
   *
   * @param host The host to connect to.
   * @param path The path to request.
   * @param opt_headers Optional headers to include in the request.
   * @return Response The status of the GET request.
   */
  Response get(const std::string &host, const std::string &path,
               const std::map<std::string, std::string> &opt_headers = {});
  /**
   * @brief Adds a callback to be called when a response is received.
   *
   * @param callback The callback function to be called.
   */
  void addGetCb(ResponseCallback callback);

private:
  // response callback called when a response is received
  ResponseCallback response_callback_;
  // tcp client object used to communicate with peer
  std::unique_ptr<TcpClient> tcpClient_;
  std::string version_;
  std::string response_;
  /**
   * @brief Formats the GET request string.
   * @param host The host to connect to.
   * @param path The path to request.
   * @param opt_headers Optional headers to include in the request.
   * @return std::string The formatted GET request string.
   */
  std::string
  formatGetReq(const std::string &host, const std::string &path,
               const std::map<std::string, std::string> &opt_headers);
  /**
   * @brief Receives the response from the host.
   * @return int The number of bytes received.
   */
  int getAnswerFromHost();
  /**
   * @brief Parses the server answer and detects the return code .
   * @return int The return code (e.g., 200, 404, etc.).
   */
  int generateGETReturnCode(const std::string &response);
};