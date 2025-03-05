#include "http.hpp"
#include "gtest/gtest.h"
#include <iostream>

// test simple get request
TEST(GET, OK) {
  HttpClient client("1.1");
  auto ret = client.get("httpbin.io", "/html");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test simple get request with headers
TEST(GET, OK_with_headers) {
  HttpClient client("1.1");
  auto ret = client.get("example.com", "/",
                        {{"User-Agent", "HttpClient/1.0"}, {"Accept", "*/*"}});
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test get request with failed connection establishment
TEST(GET, ConnectionFailed) {
  HttpClient client("1.1", "debug");
  auto ret = client.get("deadbeedcafe.com", "/");
  const Response exp = {RequestStatus::CONNECTION_FAILED, -1};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
TEST(GET, v1_0_callback) {
  HttpClient client("1.0");
  client.addGetCb([](const std::string &response) {
    if (response.size() != 1521) {
      throw std::runtime_error("Response size is not 1521 but " +
                               std::to_string(response.size()));
    }
    // check version 1.1
    if (response.find("HTTP/1.0 200 OK") == std::string::npos) {
      throw std::runtime_error(
          "Response does not contain valid HTTP/1.0 answer");
    }
  });
  auto ret = client.get("example.com", "/");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
TEST(GET, v1_1_callback) {
  HttpClient client("1.1");
  client.addGetCb([](const std::string &response) {
    if (response.size() != 1521) {
      throw std::runtime_error("Response size is not 1521 but " +
                               std::to_string(response.size()));
    }
    // check version 1.1
    if (response.find("HTTP/1.1 200 OK") == std::string::npos) {
      throw std::runtime_error(
          "Response does not contain valid HTTP/1.1 answer");
    }
  });
  auto ret = client.get("example.com", "/");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}

TEST(MvCtor, WO_cb) {
  HttpClient client("1.1", "debug");
  HttpClient client2(std::move(client));
  auto ret = client2.get("example.com", "/");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// TEST(GET, Error_cases) {

//     Response exp = {RequestStatus::SUCCESS, -1};
//     for (auto i : {301, 400, 404, 500, 503}) {
//         HttpClient client("1.1","debug");
//         client.addGetCb([](const std::string &response) {
//             std::cout << response << std::endl;
//         });
//         auto ret = client.get("httpbin.org,", "/status/" +
//         std::to_string(i)); exp.ret_code = i; EXPECT_EQ(ret.ret_code,
//         exp.ret_code); EXPECT_EQ(ret.status, exp.status);
//     }
// }
// todo: test unknown host (seems to hang on statuscode.me)
// todo: test invalid path
// todo: test reusing the same client object for multiple requests