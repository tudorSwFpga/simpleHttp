
#include "../include/http.hpp"
#include "../include/tcp.hpp"


HttpClient::HttpClient(const std::string& version,const std::string& verbosity):version_(version)
{
    spdlog::set_level(spdlog::level::from_str(verbosity));
    spdlog::debug("HttpClient Ctor");
    if (version_.empty() && version_ != "1.0" && version_ != "1.1")
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


GET_STATUS HttpClient::get(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers)
{
    std::string request = "GET " + formatGetReq(host, path,opt_headers);
    spdlog::debug("HttpClient::get \n{0}", request);
    //connect to the host
    if (tcpClient_->connect(80, host) < 0)
    {
        return GET_STATUS::CONNECTION_FAILED;
    }
    //send request
    int bytes_sent = tcpClient_->send(request);
    if (bytes_sent<=0) {
        spdlog::error("HttpClient::get failed to send request");
        return GET_STATUS::SEND_FAILED;
    }
    //get response from the server and call the callback
    if (getAnswerFromHost() <= 0) {
        spdlog::error("HttpClient::get failed to receive response");
        return GET_STATUS::RECV_FAILED;
    }
    response_callback_(response_);
    //disconnect from the server
    tcpClient_->disconnect();
    return GET_STATUS::SUCCESS;
}


std::string HttpClient::formatGetReq(const std::string& host, const std::string& path,const std::map<std::string, std::string>& opt_headers)
{
    std::string request = path + " " + "HTTP/" + version_ + "\r\n";
    request += "Host: " + host + "\r\n";
    for (const auto& [key, value] : opt_headers) {
        request += key + ": " + value + "\r\n";
    }
    request += "Connection: close\r\n\r\n";

    return request;
}


int HttpClient::getAnswerFromHost()
{
    return tcpClient_->recv(response_);
}


void HttpClient::addGetCb(ResponseCallback callback) {
    response_callback_ = callback;
}