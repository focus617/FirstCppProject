#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "listener.h"

using namespace httplib;

const char *HOST = "localhost";
const int PORT = 8080;

const std::string LARGE_DATA = std::string(1024 * 1024 * 100, '@'); // 100MB

class RestfulServer_Test_Fixture : public testing::Test
{
public:
    RestfulServer_Test_Fixture() : host(HOST, PORT), cli(HOST, PORT)
    {
    }
    ~RestfulServer_Test_Fixture() {}

    void SetUp();
    void TearDown();

    http::Host host;
    std::thread thread;

    Client cli;
};

void RestfulServer_Test_Fixture::SetUp()
{
    thread = std::thread(&http::listener::run, std::move(host));
    http::listener::wait_until_ready();
}

void RestfulServer_Test_Fixture::TearDown()
{
    http::listener::stop();
    thread.join();
}

// TEST_F(RestfulServer_Test_Fixture, server_stop)
// {
//     Client cli("localhost", svr_port);
//     cli.set_follow_location(true);

//     auto res = cli.Get("/stop");
//     ASSERT_FALSE(http::listener::server.is_running());
// }

TEST_F(RestfulServer_Test_Fixture, HeadMethod404) {
  auto res = cli.Head("/invalid");
  ASSERT_TRUE(res);
  EXPECT_EQ(StatusCode::NotFound_404, res->status);
  EXPECT_TRUE(res->body.empty());
}

TEST_F(RestfulServer_Test_Fixture, GetMethod404)
{
    auto res = cli.Get("/invalid");
    ASSERT_TRUE(res);
    EXPECT_EQ(StatusCode::NotFound_404, res->status);
}

TEST_F(RestfulServer_Test_Fixture, lists_GetMethod200)
{
    auto res = cli.Get("/lists/1");
    ASSERT_TRUE(res);
    EXPECT_EQ("HTTP/1.1", res->version);
    EXPECT_EQ(StatusCode::OK_200, res->status);
    EXPECT_EQ("OK", res->reason);
    EXPECT_EQ("text/plain", res->get_header_value("Content-Type"));
    EXPECT_EQ(1U, res->get_header_value_count("Content-Type"));
}

TEST_F(RestfulServer_Test_Fixture, lists_GetMethod200_return_correct_value)
{
    cli.set_follow_location(true);

    std::string body;
    auto res = cli.Get("/lists/2", [&](const char *data, size_t data_length)
                       {
      body.append(data, data_length);
      return true; });

    ASSERT_TRUE(res);
    EXPECT_EQ(StatusCode::OK_200, res->status);
    EXPECT_EQ("2", body);
}


TEST_F(RestfulServer_Test_Fixture, PostEmptyContent) {
  auto res = cli.Post("/empty", "", "text/plain");
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  ASSERT_EQ("empty", res->body);
}

TEST_F(RestfulServer_Test_Fixture, PostEmptyContentWithNoContentType) {
  auto res = cli.Post("/empty-no-content-type");
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  ASSERT_EQ("empty-no-content-type", res->body);
}

TEST_F(RestfulServer_Test_Fixture, PostPathOnly) {
  auto res = cli.Post("/path-only");
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  ASSERT_EQ("path-only", res->body);
}

TEST_F(RestfulServer_Test_Fixture, PostPathAndHeadersOnly) {
  auto res = cli.Post("/path-headers-only",
                       Headers({{"hello", "world"}, {"hello2", "world2"}}));
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  ASSERT_EQ("path-headers-only", res->body);
}

TEST_F(RestfulServer_Test_Fixture, PostLarge) {
  auto res = cli.Post("/post-large", LARGE_DATA, "text/plain");
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  EXPECT_EQ(LARGE_DATA, res->body);
}

TEST_F(RestfulServer_Test_Fixture, PutEmptyContentWithNoContentType) {
  auto res = cli.Put("/empty-no-content-type");
  ASSERT_TRUE(res);
  ASSERT_EQ(StatusCode::OK_200, res->status);
  ASSERT_EQ("empty-no-content-type", res->body);
}
