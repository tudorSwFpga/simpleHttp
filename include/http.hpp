#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <memory>
#include <optional>
#include "tcp.hpp"
#include <sys/types.h>
#include <functional>
#include <map>

enum class GET_STATUS {
    SOCKET_CREATION_FAILED,
    CONNECTION_FAILED,
    SEND_FAILED,
    RECV_FAILED,
    DISCONNECT_FAILED,
    SUCCESS
};

using ResponseCallback = std::function<void(const std::string&)>;

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
        explicit HttpClient(const std::string& version,const std::string& verbosity="info");
        /**
        * @brief Copy constructor for HttpClient. 
        * @param other The HttpClient object to copy from.
        */
        HttpClient(const HttpClient& other);
        /**
        * @brief Move constructor for HttpClient. 
        * @param other The HttpClient object to move from.
        */
        HttpClient(HttpClient&& other) noexcept;
        //HttpClient& operator=(const HttpClient& other);
        ~HttpClient();
        /**
        * @brief Makes an HTTP GET request to the specified host and path.
        * 
        * @param host The host to connect to.
        * @param path The path to request.
        * @param opt_headers Optional headers to include in the request.
        * @return GET_STATUS The status of the GET request.
        */
        GET_STATUS get(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers = {});
        /**
        * @brief Adds a callback to be called when a response is received.
        * 
        * @param callback The callback function to be called.
        */
        void addGetCb(ResponseCallback callback);

    private:
        ResponseCallback response_callback_; 
        std::unique_ptr<TcpClient> tcpClient_;
        const std::string version_;
        std::string response_;
        /**
        * @brief Formats the GET request string.
        * @param host The host to connect to.
        * @param path The path to request.
        * @param opt_headers Optional headers to include in the request.
        * @return std::string The formatted GET request string.
        */
        std::string formatGetReq(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers);
        /**
        * @brief Receives the response from the host.
        * @return int The number of bytes received.
        */
        int getAnswerFromHost();
};