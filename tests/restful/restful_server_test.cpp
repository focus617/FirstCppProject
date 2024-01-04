#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "listener.h"

using namespace httplib;
class RestfulServer_Test_Fixture : public testing::Test
{
public:
    RestfulServer_Test_Fixture() {}
    ~RestfulServer_Test_Fixture() {}

    void SetUp();
    void TearDown();

    uint svr_port;
    std::thread thread;
};

void RestfulServer_Test_Fixture::SetUp()
{
    svr_port = 8080;
    http::Host host("localhost", svr_port);
    thread = std::thread(&http::listener::run, std::move(host));

    if (http::listener::p_server != nullptr)
    {
        http::listener::p_server->wait_until_ready();
    }
}

void RestfulServer_Test_Fixture::TearDown()
{
    http::listener::stop();
    thread.join();

    if (http::listener::p_server != nullptr)
    {
        ASSERT_FALSE(http::listener::p_server->is_running());
    }
}

// TEST_F(RestfulServer_Test_Fixture, server_stop)
// {
//     Client cli("localhost", svr_port);
//     cli.set_follow_location(true);

//     auto res = cli.Get("/stop");
//     ASSERT_FALSE(http::listener::server.is_running());
// }

TEST_F(RestfulServer_Test_Fixture, lists_get_basic)
{
    Client cli("localhost", svr_port);
    cli.set_follow_location(true);

    auto res = cli.Get("/lists/1");
    ASSERT_TRUE(res);
    EXPECT_EQ(StatusCode::OK_200, res->status);
}

TEST_F(RestfulServer_Test_Fixture, lists_get_return_correct_value)
{
    Client cli("localhost", svr_port);
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