#pragma once

#include <glog/logging.h>
#include <httplib.h>

#include "host.h"

namespace http
{
    namespace api
    {
        void list(const httplib::Request &req, httplib::Response &res)
        {
            auto numbers = req.matches[1];
            res.set_content(numbers, "text/plain");
        }
    }

    namespace handler
    {
        void setup_routing(httplib::Server &svr)
        {
            // Index page
            svr.Get("/", [](const httplib::Request &, httplib::Response &res)
                    { res.set_content("<html><body><h1>Hello World!</h1></body></html>", "text/html"); });

            // Match the request path against a regular expression and extract its captures
            svr.Get(R"(/list/(\d+))", [&](const httplib::Request &req, httplib::Response &res) -> void
                    { api::list(req, res); });
        }

        void setup_security(httplib::Server &svr, Host &host)
        {
        }

        void setup_errors(httplib::Server &svr)
        {
        }

        void setup(httplib::Server &svr, Host &host)
        {
            setup_security(svr, host);
            setup_errors(svr);
            setup_routing(svr);
        }
    }

    namespace listener
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
    }
}