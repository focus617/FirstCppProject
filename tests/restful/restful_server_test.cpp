#include "restful_server.hpp"

#include <thread>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "app/app.hpp"

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_CERT2_FILE "./cert2.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"
#define CA_CERT_FILE "./ca-bundle.crt"
#define CLIENT_CA_CERT_FILE "./rootCA.cert.pem"
#define CLIENT_CA_CERT_DIR "."
#define CLIENT_CERT_FILE "./client.cert.pem"
#define CLIENT_PRIVATE_KEY_FILE "./client.key.pem"
#define SERVER_ENCRYPTED_CERT_FILE "./cert_encrypted.pem"
#define SERVER_ENCRYPTED_PRIVATE_KEY_FILE "./key_encrypted.pem"
#define SERVER_ENCRYPTED_PRIVATE_KEY_PASS "test123!"

using namespace httplib;

const std::string LARGE_DATA = std::string(1024 * 1024 * 100, '@');  // 100MB

class RestfulServer_Test_Fixture : public testing::Test {
 public:
  const char* HOST = "localhost";
  const int PORT = 8080;

  RestfulServer_Test_Fixture();
  ~RestfulServer_Test_Fixture() {}

  void SetUp();
  void TearDown();

  http::RestfulServer server;

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  SSLClient cli;
#else
  Client cli;
#endif
};

RestfulServer_Test_Fixture::RestfulServer_Test_Fixture()
    : server("Restful-Server"), cli(HOST, PORT) {}

void RestfulServer_Test_Fixture::SetUp() {
  // GTEST_SKIP() << "Skipping single test";
  server.setup();
  server.start();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "SSLClient enable";
  cli.enable_server_certificate_verification(false);
#endif
  cli.set_connection_timeout(std::chrono::seconds(5));

  server.wait_until_ready();
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server is ready.";
#else
  LOG(INFO) << "Http Server is ready.";
#endif
}

void RestfulServer_Test_Fixture::TearDown() {
}

TEST_F(RestfulServer_Test_Fixture, HeadMethod404) {
  auto res = cli.Head("/invalid");
  ASSERT_TRUE(res);
  EXPECT_EQ(StatusCode::NotFound_404, res->status);
  EXPECT_TRUE(res->body.empty());
}

TEST_F(RestfulServer_Test_Fixture, GetMethod404) {
  auto res = cli.Get("/invalid");
  ASSERT_TRUE(res);
  EXPECT_EQ(StatusCode::NotFound_404, res->status);
}

TEST_F(RestfulServer_Test_Fixture, lists_GetMethod200) {
  auto res = cli.Get("/api/v1/lists/1");
  ASSERT_TRUE(res);
  EXPECT_EQ("HTTP/1.1", res->version);
  EXPECT_EQ(StatusCode::OK_200, res->status);
  EXPECT_EQ("OK", res->reason);
  EXPECT_EQ("text/plain", res->get_header_value("Content-Type"));
  EXPECT_EQ(1U, res->get_header_value_count("Content-Type"));
}

TEST_F(RestfulServer_Test_Fixture, lists_GetMethod200_return_correct_value) {
  cli.set_follow_location(true);

  std::string body;
  auto res = cli.Get("/api/v1/lists/2", [&](const char* data, size_t data_length) {
    body.append(data, data_length);
    return true;
  });

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
