#include "restful_server.hpp"

#include <glog/logging.h>

#include <chrono>
#include <filesystem>
#include <thread>

#include "http/handler.h"

using namespace http;

RestfulServer::RestfulServer(const std::string& t_app_name,
                             const std::string& t_version)
    : WindowApp{std::move(t_app_name), std::move(t_version)},
      m_server_ptr_{nullptr},
      m_host_ptr_{nullptr} {}

RestfulServer::~RestfulServer() {
  if ((nullptr != m_server_ptr_) && (m_server_ptr_->is_running())) {
    stop();
  }
  if (m_thread_.joinable()) {
    m_thread_.join();
  }
  if (nullptr != m_server_ptr_) delete m_server_ptr_;
  if (nullptr != m_host_ptr_) delete m_host_ptr_;
}

void RestfulServer::setup_based_on_conf() {
  LOG(INFO) << "JSON Configuration:\n" << std::setw(4) << m_conf_;

  std::string& ip = m_conf_["server"]["ip"].get_ref<std::string&>();
  uint port = m_conf_["server"]["webPort"].get<uint>();
  m_host_ptr_ = new http::Host(ip, port);

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

  m_server_ptr_ = new httplib::SSLServer(sslcert.c_str(), sslkey.c_str());
#else
  m_server_ptr_ = new httplib::Server();
#endif
}

void RestfulServer::setup_default_routing() {
  // Default Routing: Stop the server when the user access /stop
  m_server_ptr_->Get("/stop",
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
  handler::setup(*m_server_ptr_, *m_host_ptr_);
}

void RestfulServer::launch_tasks() { start(); }

// void RestfulServer::main_loop() {
//   constexpr int idleTime = 5;
//   while (true) {
//     std::this_thread::sleep_for(std::chrono::seconds(idleTime));
//     LOG(INFO) << "Main thread idle...";
//   }
// }

void RestfulServer::start() {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server Start at " << m_host_ptr_->ip << ":"
            << m_host_ptr_->port;
#else
  LOG(INFO) << "Http Server Start at " << m_host_ptr_->ip << ":"
            << m_host_ptr_->port;
#endif

  // Launch Listener thread to port
  m_thread_ = std::thread(
      [&]() { m_server_ptr_->listen(m_host_ptr_->ip, m_host_ptr_->port); });
}

void RestfulServer::stop() {
  m_server_ptr_->stop();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server Stopped.";
#else
  LOG(INFO) << "Http Server Stopped.";
#endif
}

void RestfulServer::wait_until_ready() {
  m_server_ptr_->wait_until_ready();
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Https Server is ready to work...";
#else
  LOG(INFO) << "Http Server is ready to work...";
#endif
}

bool RestfulServer::is_running() {
  return ((nullptr != m_server_ptr_) && (m_server_ptr_->is_running()));
}

namespace xuzy {

const std::string APP_NAME{"Restful-Server"};
const std::string VERSION{"0.0.1"};

WindowApp* CreateApplication() {
  WindowApp* app = new http::RestfulServer(APP_NAME, VERSION);
  return app;
}

}  // namespace xuzy
