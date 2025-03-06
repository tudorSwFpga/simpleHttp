#include "http.hpp"
#include "gtest/gtest.h"
#include <iostream>

// test simple get request for 1.0 version
TEST(get, version_1_0) {
  HttpClient client("1.0");
  auto ret = client.get("httpbin.io", "/html");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test simple get request for 1.1 version
TEST(get, version_1_1) {
  HttpClient client("1.1");
  auto ret = client.get("httpbin.io", "/html");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}

// test get request with failed connection establishment
TEST(get, host_connection_failed) {
  HttpClient client("1.1", "debug");
  auto ret = client.get("deadbeedcafe.com", "/");
  const Response exp = {RequestStatus::CONNECTION_FAILED, -1};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
TEST(get, wrong_path) {
  HttpClient client("1.1", "debug");
  auto ret =
      client.get("httpbin.io", "/doesnotexist", {{"Connection", "keep-alive"}});
  //{"Keep-Alive": "timeout=5, max=200"}});
  Response exp = {RequestStatus::SUCCESS, 404};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
  ret = client.get("httpbin.io", "/json");
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test 1.0 http get with callback
TEST(add_callback, v1_0) {
  HttpClient client("1.0");
  client.addGetCb([](const std::string &response) {
    if (response.size() != 3910) {
      throw std::runtime_error("Response size is not 3910 but " +
                               std::to_string(response.size()));
    }
    // check version 1.0
    if (response.find("HTTP/1.0 200 OK") == std::string::npos) {
      throw std::runtime_error(
          "Response does not contain valid HTTP/1.0 answer");
    }
  });
  auto ret = client.get("httpbin.io", "/html");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test 1.1 http get with callback
TEST(add_callback, v1_1) {
  HttpClient client("1.1");
  client.addGetCb([](const std::string &response) {
    std::cout << response << std::endl;
    if (response.size() != 636) {
      throw std::runtime_error("Response size is not 636 but " +
                               std::to_string(response.size()));
    }
    // check version 1.1
    if (response.find("HTTP/1.1 200 OK") == std::string::npos) {
      throw std::runtime_error(
          "Response does not contain valid HTTP/1.1 answer");
    }
  });
  auto ret = client.get("httpbin.io", "/json");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test move constructor
TEST(move_constructor, successful_request) {
  HttpClient client("1.1", "debug");
  HttpClient client2(std::move(client));
  auto ret = client2.get("example.com", "/");
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test simple get request with headers
TEST(headers, keepavalive) {
  HttpClient client("1.1", "debug");
  auto ret = client.get("httpbin.io", "/html", {{"Connection", "keep-alive"}});
  const Response exp = {RequestStatus::SUCCESS, 200};
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
  ret = client.get("httpbin.io", "/json");
  EXPECT_EQ(ret.ret_code, exp.ret_code);
  EXPECT_EQ(ret.status, exp.status);
}
// test simple get request with headers
TEST(headers, other_headers) {
  HttpClient client("1.1");
  auto ret = client.get("example.com", "/",
                        {{"User-Agent", "HttpClient/1.0"}, {"Accept", "*/*"}});
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

// Run all the tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}