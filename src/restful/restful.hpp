#pragma once

#include <string>

#include "app/app.hpp"
#include "visibility_control.hpp"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#  define SERVER_CERT_FILE "./cert.pem"
#  define SERVER_PRIVATE_KEY_FILE "./key.pem"
#endif

namespace http {
class XUZY_API Restful : public xuzy::App {
 private:
  void start();
  static void idle();

  void setup(const json& configuration) override;

 public:
  XUZY_API
  Restful(std::string t_app_name);

  XUZY_API
  virtual ~Restful() final = default;

  Restful(const Restful&) = delete;
  Restful& operator=(const Restful&) = delete;
  Restful(Restful&&) = delete;
  Restful& operator=(Restful&&) = delete;

  XUZY_API
  void run() override;
};
}  // namespace http