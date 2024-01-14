#pragma once

#include <httplib.h>
#include <thread>
#include <string>

#include "app/app.hpp"
#include "host.h"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"
#endif
namespace http
{
    class RestfulServer : public xuzy::App
    {
    private:
        std::thread m_thread;

        // HTTP Server
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        httplib::SSLServer m_server;
#else
        httplib::Server m_server;
#endif
        void start();
        void stop();
        static void idle();

    public:
        RestfulServer(std::string t_app_name);
        ~RestfulServer();

        RestfulServer(const RestfulServer &) = delete;
        RestfulServer &operator=(const RestfulServer &) = delete;
        RestfulServer(RestfulServer &&) = delete;
        RestfulServer &operator=(RestfulServer &&) = delete;

        void run() override;
    };

} // namespace http