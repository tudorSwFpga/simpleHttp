#include "../include/http.hpp"
#include "../include/tcp.hpp"


HttpClient::HttpClient(const std::string& version):version_(version)
{
    spdlog::debug("HttpClient Ctor");
    if (version_.empty() && version_ != "1.0" && version_ != "1.1" && version_ != "2.0")
    {
        throw std::invalid_argument("Invalid HTTP version");
    }
    //create a new TcpClient object
    tcpClient_ = std::make_unique<TcpClient>("HttpClient");
}   

// Copy constructor
HttpClient::HttpClient(const HttpClient& other) : version_(other.version_), response_callback_(other.response_callback_) {}

// Move constructor
HttpClient::HttpClient(HttpClient&& other) noexcept : version_(std::move(other.version_)), response_callback_(std::move(other.response_callback_)) {}

// Copy assignment operator
// HttpClient::HttpClient& operator=(const HttpClient& other) {
//     if (this != &other) {
//         version_ = other.version_;
//         response_callback_ = other.response_callback_;
//     }
//     return *this;
// }

// // Move assignment operator
// HttpClient& operator=(HttpClient&& other) noexcept {
//     if (this != &other) {
//         version_ = std::move(other.version_);
//         response_callback_ = std::move(other.response_callback_);
//     }
//     return *this;
// }

HttpClient::~HttpClient()
{
    spdlog::debug("HttpClient Dtor");
}   

std::optional<ConnectionError> HttpClient::get(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers)
{
    spdlog::debug("HttpClient get");
    std::string request = "GET " + setHeader(host, path);
    if (tcpClient_->connect(80, host) < 0)
    {
        return ConnectionError::CONNECTION_FAILED;
    }

    tcpClient_->send(request);
    getAnswer();
    response_callback_;
    tcpClient_->disconnect();
    return {};
}

std::string HttpClient::setHeader(const std::string& host, const std::string& path)
{
    std::string request = path + " " + "HTTP/" + version_ + "\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";

    return request;
}

std::optional<ConnectionError> HttpClient::getAnswer()
{
    if ( tcpClient_->recv(response_) <= 0) {
        return ConnectionError::RECV_FAILED;
    }

    return {};
}

void HttpClient::add_get_cb(ResponseCallback callback) {
    response_callback_ = callback;
}