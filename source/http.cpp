
#include "../include/http.hpp"
#include "../include/tcp.hpp"
#include <regex>

HttpClient::HttpClient(const std::string &version, const std::string &verbosity)
    : version_(version) {
  spdlog::set_level(spdlog::level::from_str(verbosity));
  spdlog::debug("HttpClient Ctor");
  if (version_.empty() && version_ != "1.0" && version_ != "1.1") {
    throw std::invalid_argument("Invalid HTTP version");
  }
  // create a new TcpClient object
  tcpClient_ = std::make_unique<TcpClient>("HttpClient");
}

// Move constructor
HttpClient::HttpClient(HttpClient &&other) noexcept
    : version_(std::move(other.version_)),
      response_callback_(std::move(other.response_callback_)),
      tcpClient_(std::move(other.tcpClient_)) {
  spdlog::debug("HttpClient Move Ctor");
}

HttpClient::~HttpClient() { spdlog::debug("HttpClient Dtor"); }

Response
HttpClient::get(const std::string &host, const std::string &path,
                const std::map<std::string, std::string> &opt_headers) {
  Response ret;
  ret.ret_code = -1;
  std::string request = "GET " + formatGetReq(host, path, opt_headers);
  spdlog::debug("HttpClient::get \n{0}", request);
  // connect to the host
  if (tcpClient_->connect(80, host) < 0) {
    ret.status = RequestStatus::CONNECTION_FAILED;
    return ret;
  }
  // send request
  int bytes_sent = tcpClient_->send(request);
  if (bytes_sent <= 0) {
    spdlog::error("HttpClient::get failed to send request");
    ret.status = RequestStatus::SEND_FAILED;
    return ret;
  }
  // get response from the server and call the callback
  if (getAnswerFromHost() <= 0) {
    spdlog::error("HttpClient::get failed to receive response");
    ret.status = RequestStatus::RECV_FAILED;
    return ret;
  }
  // set the return code
  ret.ret_code = generateGETReturnCode(response_);
  if (response_callback_) {
    response_callback_(response_);
  }
  // disconnect from the server
  tcpClient_->disconnect();
  ret.status = RequestStatus::SUCCESS;
  return ret;
}

std::string HttpClient::formatGetReq(
    const std::string &host, const std::string &path,
    const std::map<std::string, std::string> &opt_headers) {
  std::string request = path + " " + "HTTP/" + version_ + "\r\n";
  request += "Host: " + host + "\r\n";
  for (const auto &[key, value] : opt_headers) {
    request += key + ": " + value + "\r\n";
  }
  request += "Connection: close\r\n\r\n";

  return request;
}

int HttpClient::getAnswerFromHost() { return tcpClient_->recv(response_); }

void HttpClient::addGetCb(ResponseCallback callback) {
  response_callback_ = callback;
}

int HttpClient::generateGETReturnCode(const std::string &response) {
  std::string hdr = "HTTP/" + version_ + " 200 OK";
  std::regex statusRegex(
      R"(HTTP\/\d\.\d\s+(\d{3}))"); // Regex pattern to match HTTP status code
  std::smatch match;
  int ret_code = -1;
  if (std::regex_search(response, match, statusRegex)) {
    ret_code = std::stoi(
        match[1].str()); // Convert extracted status code to an integer
  }
  return ret_code;
}
