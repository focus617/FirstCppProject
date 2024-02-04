#include "restful_server.hpp"

#include <glog/logging.h>

#include <chrono>
#include <filesystem>
#include <thread>

#include "http/handler.h"

using namespace http;

RestfulServer::RestfulServer(const std::string& t_app_name,
                             const std::string& t_version)
    : App{std::move(t_app_name), std::move(t_version)},
      p_server_{nullptr},
      p_host_{nullptr} {}

RestfulServer::~RestfulServer() {
  if ((nullptr != p_server_) && (p_server_->is_running())) {
    stop();
  }
  if (m_thread_.joinable()) {
    m_thread_.join();
  }
  if (nullptr != p_server_) delete p_server_;
  if (nullptr != p_host_) delete p_host_;
}

void RestfulServer::setup_based_on_conf() {
  LOG(INFO) << "JSON Configuration:\n" << std::setw(4) << m_conf_;

  std::string& ip = m_conf_["server"]["ip"].get_ref<std::string&>();
  uint port = m_conf_["server"]["webPort"].get<uint>();
  p_host_ = new http::Host(ip, port);

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  const std::string& sslcert =
      m_conf_["ssl-cert"].get_ref<const std::string&>();
  if (!std::filesystem::exists(sslcert)) {
    throw xuzy::FileNotFoundException(sslcert + " not exist.");
  }
  const std::string& sslkey = m_conf_["ssl-key"].get_ref<const std::string&>();
  if (!std::filesystem::exists(sslkey)) {
    throw xuzy::FileNotFoundException(sslkey + " not exist.");
  }

  p_server_ = new httplib::SSLServer(sslcert.c_str(), sslkey.c_str());
#else
  p_server_ = new httplib::Server();
#endif
}

void RestfulServer::setup_default_routing() {
  // Default Routing: Stop the server when the user access /stop
  p_server_->Get("/stop",
                     [&](const httplib::Request& req, httplib::Response& res) {
                       stop();
                       res.set_redirect("/");
                     });
}

void RestfulServer::setup() {
  App::setup();

  setup_based_on_conf();
  setup_default_routing();

  // Setup security, routing and error handler
  handler::setup(*p_server_, *p_host_);
}

void RestfulServer::launch_tasks() { start(); }

void RestfulServer::main_loop() {
  constexpr int idleTime = 5;
  while (m_running_) {
    std::this_thread::sleep_for(std::chrono::seconds(idleTime));
    LOG(INFO) << "Main thread idle...";
  }
  LOG(INFO) << "Main thread shutdown.";
}

void RestfulServer::start() {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server Start at " << p_host_->ip << ":"
            << p_host_->port;
#else
  LOG(INFO) << "Http Server Start at " << p_host_->ip << ":"
            << p_host_->port;
#endif

  // Launch Listener thread to port
  m_thread_ = std::thread(
      [&]() { p_server_->listen(p_host_->ip, p_host_->port); });
}

void RestfulServer::stop() {
  p_server_->stop();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server Stopped.";
#else
  LOG(INFO) << "Http Server Stopped.";
#endif

  // notify main thread to shutdown
  m_running_ = false;
}

void RestfulServer::wait_until_ready() {
  p_server_->wait_until_ready();
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server is ready to work...";
#else
  LOG(INFO) << "Http Server is ready to work...";
#endif
}

bool RestfulServer::is_running() {
  return ((nullptr != p_server_) && (p_server_->is_running()));
}

namespace xuzy {

const std::string APP_NAME{"Restful-Server"};
const std::string VERSION{"0.0.1"};

App* CreateApplication() {
  App* app = new http::RestfulServer(APP_NAME, VERSION);
  return app;
}

}  // namespace xuzy
