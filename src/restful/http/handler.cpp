#include <glog/logging.h>

#include "handler.h"
#include "api.h"
#include "response_codes.h"

namespace http::handler
{
    void setup_routing(httplib::Server &svr)
    {
        // Index page
        svr.Get("/", [](const httplib::Request &, httplib::Response &res)
                { res.set_content("<html><body><h1>Hello World!</h1></body></html>", "text/html"); });

        // Match the request path against a regular expression and extract its captures
        svr.Get(R"(/lists/(\d+))", [&](const httplib::Request &req, httplib::Response &res) -> void
                { api::list(req, res); });
    }

    void setup_security(httplib::Server &svr, Host &host)
    {
        svr.set_pre_routing_handler([&host](auto &req, auto &res)
                                    {
                auto ipaddr = req.get_header_value("REMOTE_ADDR");
                auto useragent = req.get_header_value("User-Agent");

                if(host.isBanned(ipaddr))
                {
                    res.status = Code::Unauthorized;
                    LOG(WARNING) << "IP Blocked: " << ipaddr;
                    return httplib::Server::HandlerResponse::Handled;
                }
                return httplib::Server::HandlerResponse::Unhandled; });
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