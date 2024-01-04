#include <glog/logging.h>

#include "listener.h"
#include "handler.h"

namespace http::listener
{
    // HTTP Server
    httplib::Server *p_server = nullptr;

    void run(Host host)
    {
        // HTTP Server
        httplib::Server server;
        p_server = &server;

        // Setup listening IP address and routing
        handler::setup(server, host);

        // Default Routing: Stop the server when the user access /stop
        server.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
                   {                
                server.stop(); 
                p_server = nullptr;
                res.set_redirect("/"); });

        LOG(INFO) << "Http Server Start at " << host.ip << ":" << host.port;

        // Listen server to port
        server.listen(host.ip, host.port);

        LOG(INFO) << "Http Server Stopped.";
    }


    void wait_until_ready()
    {
        if ((p_server != nullptr) && (p_server->is_running()))
        {
            p_server->wait_until_ready();
        }
    }

    void stop()
    {
        if ((p_server != nullptr) && (p_server->is_running()))
        {
            p_server->stop();
            p_server = nullptr;
        }
    }

} // namespace http::listener