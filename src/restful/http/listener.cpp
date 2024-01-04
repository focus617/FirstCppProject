#include <glog/logging.h>

#include "listener.h"
#include "handler.h"

namespace http::listener
{
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
                res.set_redirect("/"); });

        LOG(INFO) << "Http Server Start at " << host.ip << ":" << host.port;

        // Listen server to port
        server.listen(host.ip, host.port);

        LOG(INFO) << "Http Server Stopped.";
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