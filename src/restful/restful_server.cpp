#include <glog/logging.h>
#include <chrono>

#include "restful_server.h"
#include "handler.h"

using namespace http;

RestfulServer::RestfulServer(std::string t_app_name)
    : App{std::move(t_app_name)},
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
      m_server(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE)
#else
      m_server()
#endif
{
}

RestfulServer::~RestfulServer()
{
    if (m_server.is_running())
    {
        stop();
    }
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void RestfulServer::run()
{
    App::run();
    start();
    idle();
}

void RestfulServer::start()
{
    const char *HOST = "localhost";
    const int PORT = 8080;
    http::Host host(HOST, PORT);
    // TODO: host.setup(App::getConfig().at("httpServer")));

    // Setup listening IP address and routing
    handler::setup(m_server, host);

    // Default Routing: Stop the server when the user access /stop
    m_server.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
                 {                
                stop(); 
                res.set_redirect("/"); });

    // Launch Listener thread to port
    m_thread = std::thread([&]()
                           {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
            LOG(INFO) << "Https Server Start at " << host.ip << ":" << host.port;
#else
            LOG(INFO) << "Http Server Start at " << host.ip << ":" << host.port;
#endif                     
            m_server.listen(host.ip, host.port); });
    
    m_server.wait_until_ready();
}

void RestfulServer::stop()
{
    m_server.stop();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    LOG(INFO) << "Https Server Stopped.";
#else
    LOG(INFO) << "Http Server Stopped.";
#endif
}

void RestfulServer::idle()
{
    constexpr int idleTime = 5;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(idleTime));
        LOG(INFO) << "Main thread idle...";
    }
}

