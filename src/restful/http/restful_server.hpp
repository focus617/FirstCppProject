#pragma once

#include <httplib.h>

#include <string>

#include "app/app.hpp"
#include "host.hpp"
#include "visibility_control.hpp"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#  define SERVER_CERT_FILE "./cert.pem"
#  define SERVER_PRIVATE_KEY_FILE "./key.pem"
#endif

namespace http {

/**
 * @brief HTTP Server App
 */
class XUZY_API RestfulServer : public xuzy::App {
 public:
  XUZY_API explicit RestfulServer(std::string t_app_name);
  XUZY_API ~RestfulServer();

  RestfulServer(const RestfulServer&) = delete;
  RestfulServer& operator=(const RestfulServer&) = delete;
  RestfulServer(RestfulServer&&) = delete;
  RestfulServer& operator=(RestfulServer&&) = delete;

  XUZY_API void start();
  XUZY_API void stop();
  XUZY_API void wait_until_ready();
  XUZY_API bool is_running();

  // Move to public for testing
  XUZY_API void setup() override;

 protected:
  XUZY_API void run() override;

 private:
  std::thread m_thread_;

/**
 * @brief HTTP Server
 */
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  httplib::SSLServer* m_server_ptr_;
#else
  httplib::Server* m_server_ptr_;
#endif

  /**
   * @brief Port at which HTTP Server is listening
   */
  http::Host* m_host_ptr_;

  /**
   * @brief Setup host& http(s)-server based on json configuration
   */
  void setup_based_on_conf();

  /**
   * @brief Setup default routing related to http server management
   */
  void setup_default_routing();

  static void idle();
};

}  // namespace http