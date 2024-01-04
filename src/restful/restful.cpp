#include <thread>

#include "restful.h"
#include "host.h"
#include "listener.h"

using namespace xuzy;

Restful::Restful(std::string appName) : m_app_name{appName}
{
}

void Restful::run()
{
    http::Host host("0.0.0.0", 8080);
    std::thread(&http::listener::run, std::move(host)).detach();
    http::listener::wait_until_ready();
}