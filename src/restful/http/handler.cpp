#include <glog/logging.h>

#include "handler.h"
#include "api.h"
#include "response_codes.h"

using namespace httplib;

namespace http::handler
{
    void setup_routing(Server &svr)
    {
        // Index page
        svr.Get("/", [](const Request &, Response &res)
                { res.set_content("<html><body><h1>Hello World!</h1></body></html>", "text/html"); });

        // Match the request path against a regular expression and extract its captures
        svr.Get(R"(/lists/(\d+))", [&](const Request &req, Response &res) -> void
                { api::list(req, res); });

        svr.Post("/empty",
                 [&](const Request &req, Response &res)
                 {
                     res.set_content("empty", "text/plain");
                 })
            .Post("/empty-no-content-type",
                  [&](const Request &req, Response &res)
                  {
                      res.set_content("empty-no-content-type", "text/plain");
                  })
            .Post("/path-only",
                  [&](const Request &req, Response &res)
                  {
                      res.set_content("path-only", "text/plain");
                  })
            .Post("/path-headers-only",
                  [&](const Request &req, Response &res)
                  {
                      res.set_content("path-headers-only", "text/plain");
                  })
            .Post("/post-large",
                  [&](const Request &req, Response &res)
                  {
                      res.set_content(req.body, "text/plain");
                  })
            .Put("/empty-no-content-type",
                 [&](const Request &req, Response &res)
                 {
                     res.set_content("empty-no-content-type", "text/plain");
                 });
    }

    void setup_security(httplib::Server &svr, Host &host)
    {
        svr.set_pre_routing_handler(
            [&host](auto &req, auto &res)
            {
                auto ipaddr = req.get_header_value("REMOTE_ADDR");
                auto user_agent = req.get_header_value("User-Agent");

                // Reject banned remote IP
                if (host.isBanned(ipaddr))
                {
                    LOG(WARNING) << "IP Blocked: " << ipaddr << " " << user_agent;

                    res.status = Code::Unauthorized;
                    return httplib::Server::HandlerResponse::Handled;
                }
                return httplib::Server::HandlerResponse::Unhandled;
            });
    }

    void setup_errors(httplib::Server &svr)
    {
        svr.set_error_handler(
            [](const auto &req, auto &res)
            {
                auto ipAddr = "IP:" + req.get_header_value("REMOTE_ADDR") + " ";
                auto methodAndPath = req.method + " " + req.path;
                auto status = " " + std::to_string(res.status);
                auto logStr = ipAddr + methodAndPath + status;

                LOG(WARNING) << logStr << std::endl;
                LOG(WARNING) << "Body: " << req.body;
            });
    }

    void setup(httplib::Server &svr, Host &host)
    {
        setup_security(svr, host);
        setup_errors(svr);
        setup_routing(svr);
    }
}