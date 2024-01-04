#include "restful.h"

namespace http::listener
{
    void run(Host host)
    {
        // Launch HTTP Server
        httplib::Server svr;
        LOG(INFO) << "Http Server Start...";

        // Setup listening IP address and routing
        handler::setup(svr, host);

        // Default Routing: Stop the sever when the user access /stop
        svr.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
                {                
                svr.stop(); 
                res.set_redirect("/"); });

        // Listen server to port
        svr.listen(host.ip, host.port);

        LOG(INFO) << "Http Server Stopped.";
    }
} // namespace http::listener