#include <glog/logging.h>
#include <chrono>
#include <thread>

#include "restful.h"
#include "host.h"
#include "listener.h"

using namespace http;

Restful::Restful(std::string t_app_name)
    : xuzy::App{std::move(t_app_name)}
{
}

void Restful::run()
{
    App::run();
    start();
    idle();
}

void Restful::start()
{
    const char *HOST = "localhost";
    const int PORT = 8080;
    http::Host host(HOST, PORT);
    // TODO: host.setup(App::getConfig().at("httpServer")));

    std::thread(&http::listener::run, std::move(host)).detach();
}

void Restful::idle()
{
    constexpr int idleTime = 5;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(idleTime));
        LOG(INFO) << "Main thread idle...";
    }
}

void Restful::setup(const json &configuration)
{
    try
    {
        const auto &jsconfig = configuration;
        if (jsconfig.empty())
        {
            LOG(WARNING) << "Failure during config: empty configuration file."
                      << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        LOG(WARNING) << "Failure during config: " << ex.what() << std::endl;
    }
}
