#pragma once

#include <string>

#include "app/app.h"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"
#endif

namespace http
{
    class Restful : public xuzy::App
    {
    private:
        void start();
        static void idle();

        void setup(const json &configuration) override;


    public:
        Restful(std::string t_app_name);
        virtual ~Restful() final = default;

        Restful(const Restful &) = delete;
        Restful &operator=(const Restful &) = delete;
        Restful(Restful &&) = delete;
        Restful &operator=(Restful &&) = delete;

        void run() override;
    };
}