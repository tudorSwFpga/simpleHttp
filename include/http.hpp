#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <memory>
#include <optional>
#include "tcp.hpp"
#include <sys/types.h>
#include <functional>
#include <map>

enum class ConnectionError {
    SOCKET_CREATION_FAILED,
    CONNECTION_FAILED,
    SEND_FAILED,
    RECV_FAILED,
    DISCONNECT_FAILED

};

using ResponseCallback = std::function<void(const std::string&)>;


class HttpClient {

    public:
        explicit HttpClient(const std::string& version);
        HttpClient(const HttpClient& other);
        HttpClient(HttpClient&& other) noexcept;
        //HttpClient& operator=(const HttpClient& other);
        //HttpClient& operator=(HttpClient&& other) noexcept;
        ~HttpClient();
        std::optional<ConnectionError> get(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers = {});
        void add_get_cb(ResponseCallback callback);
    
    private:
        ResponseCallback response_callback_; 
        std::unique_ptr<TcpClient> tcpClient_;
        const std::string version_;
        std::string response_;
        std::string setHeader(const std::string& host, const std::string& path);
        std::optional<ConnectionError> getAnswer();
}