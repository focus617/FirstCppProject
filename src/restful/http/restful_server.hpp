#pragma once

#include <core.h>
#include <httplib.h>

#include <string>

#include "app/app.hpp"
#include "host.hpp"

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
  XUZY_API explicit RestfulServer(const std::string& t_app_name,
                                  const std::string& t_version);
  XUZY_API ~RestfulServer();

  RestfulServer(const RestfulServer&) = delete;
  RestfulServer& operator=(const RestfulServer&) = delete;
  RestfulServer(RestfulServer&&) = delete;
  RestfulServer& operator=(RestfulServer&&) = delete;

  XUZY_API void setup() override;
  XUZY_API void launch_tasks() override;
  XUZY_API void main_loop() override;

  XUZY_API void start();
  XUZY_API void stop();
  XUZY_API void wait_until_ready();
  XUZY_API bool is_running();

 private:
  std::thread m_thread_;
  bool m_running_ = true;

/**
 * @brief HTTP Server
 */
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  httplib::SSLServer* p_server_;
#else
  httplib::Server* p_server_;
#endif

  /**
   * @brief Port at which HTTP Server is listening
   */
  http::Host* p_host_;

  /**
   * @brief Setup host& http(s)-server based on json configuration
   */
  void setup_based_on_conf();

  /**
   * @brief Setup default routing related to http server management
   */
  void setup_default_routing();
};

}  // namespace http