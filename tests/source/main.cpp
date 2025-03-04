#include "http.hpp"
#include "gtest/gtest.h"
#include <iostream>

// test simple get request
TEST(GET, OK) {
  HttpClient client("1.1");
  auto ret = client.get("example.com", "/");
  EXPECT_EQ(ret, GET_STATUS::SUCCESS);
}
// test simple get request with headers
TEST(GET, OK_with_headers) {
  HttpClient client("1.1");
  auto ret = client.get("example.com", "/",
                        {{"User-Agent", "HttpClient/1.0"}, {"Accept", "*/*"}});
  EXPECT_EQ(ret, GET_STATUS::SUCCESS);
}
// test get request with failed connection establishment
TEST(GET, ConnectionFailed) {
  HttpClient client("1.1");
  auto ret = client.get("examplexyz.com", "/");
  EXPECT_EQ(ret, GET_STATUS::CONNECTION_FAILED);
}
TEST(GET, AddCb) {
  HttpClient client("1.1");
  client.addGetCb([](const std::string &response) {
    if (response.size() != 1521) {
      throw std::runtime_error("Response size is not 1521 but " +
                               std::to_string(response.size()));
    }
  });
  auto ret = client.get("example.com", "/");
  EXPECT_EQ(ret, GET_STATUS::SUCCESS);
}
TEST(MvCtor, with_cb) {
  HttpClient client("1.1", "debug");
  client.addGetCb(
      [](const std::string &response) { std::cout << response << std::endl; });
  HttpClient client2(std::move(client));
  auto ret = client2.get("example.com", "/");
  EXPECT_EQ(ret, GET_STATUS::SUCCESS);
}

TEST(MvCtor, WO_cb) {
  HttpClient client("1.1", "debug");
  HttpClient client2(std::move(client));
  auto ret = client2.get("example.com", "/");
  EXPECT_EQ(ret, GET_STATUS::SUCCESS);
}