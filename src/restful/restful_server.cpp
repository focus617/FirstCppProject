#include "restful_server.hpp"

#include <glog/logging.h>

#include <chrono>
#include <thread>

#include "http/handler.h"

using namespace http;

RestfulServer::RestfulServer(std::string t_app_name)
    : App{std::move(t_app_name)},
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

void RestfulServer::setup() {
  App::setup();

  LOG(INFO) << m_conf_;

  m_host_ptr_ =
      new http::Host(m_conf_["server"]["ip"], m_conf_["server"]["webPort"]);

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  // m_server_ptr_ =
  //     new httplib::SSLServer(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
  
  m_server_ptr_ = new httplib::SSLServer(
      m_conf_["ssl-cert"].get_ref<const std::string&>().c_str(),
      m_conf_["ssl-key"].get_ref<const std::string&>().c_str());
#else
  m_server_ptr_ = new httplib::Server();
#endif
}

void RestfulServer::run() {
  start();
  idle();
}

void RestfulServer::start() {
  // Setup listening IP address and routing
  handler::setup(*m_server_ptr_, *m_host_ptr_);

  // Default Routing: Stop the server when the user access /stop
  m_server_ptr_->Get("/stop",
                     [&](const httplib::Request& req, httplib::Response& res) {
                       stop();
                       res.set_redirect("/");
                     });

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

void RestfulServer::idle() {
  constexpr int idleTime = 5;
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(idleTime));
    LOG(INFO) << "Main thread idle...";
  }
}

void RestfulServer::wait_until_ready() { m_server_ptr_->wait_until_ready(); }
