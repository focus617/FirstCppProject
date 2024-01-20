#include "handler.h"

#include <glog/logging.h>

#include "api/api.h"
#include "response_codes.h"

using namespace httplib;

namespace http::handler {
void setup_routing(Server& svr) {
  // Index page
  svr.Get("/", [](const Request&, Response& res) {
    res.set_content("<html><body><h1>Hello World!</h1></body></html>",
                    "text/html");
  });

  // Match the request path against a regular expression and extract its
  // captures
  svr.Get(R"(/api/v1/lists/(\d+))",
          [&](const Request& req, Response& res) -> void {
            api::lists_get(req, res);
          });

  svr.Post(R"(/api/v1/lists)", [&](const Request& req, Response& res) -> void {
    api::lists_post(req, res);
  });

  svr.Post("/empty",
           [&](const Request& req, Response& res) {
             res.set_content("empty", "text/plain");
           })
      .Post("/empty-no-content-type",
            [&](const Request& req, Response& res) {
              res.set_content("empty-no-content-type", "text/plain");
            })
      .Post("/path-only",
            [&](const Request& req, Response& res) {
              res.set_content("path-only", "text/plain");
            })
      .Post("/path-headers-only",
            [&](const Request& req, Response& res) {
              res.set_content("path-headers-only", "text/plain");
            })
      .Post("/post-large", [&](const Request& req, Response& res) {
        res.set_content(req.body, "text/plain");
      });
      
  svr.Put("/empty-no-content-type", [&](const Request& req, Response& res) {
    res.set_content("empty-no-content-type", "text/plain");
  });
}

void setup_security(httplib::Server& svr, Host& host) {
  svr.set_pre_routing_handler([&host](auto& req, auto& res) {
    auto ipaddr = req.get_header_value("REMOTE_ADDR");
    auto user_agent = req.get_header_value("User-Agent");

    // Reject banned remote IP
    if (host.isBanned(ipaddr)) {
      LOG(WARNING) << "IP Blocked: " << ipaddr << " " << user_agent;

      res.status = Code::Unauthorized;
      return httplib::Server::HandlerResponse::Handled;
    }
    return httplib::Server::HandlerResponse::Unhandled;
  });
}

void setup_errors(httplib::Server& svr) {
  svr.set_error_handler([](const auto& req, auto& res) {
    auto ipAddr = "IP:" + req.get_header_value("REMOTE_ADDR") + " ";
    auto methodAndPath = "Request:" + req.method + " " + req.path + " ";
    auto status = "Status:" + std::to_string(res.status);
    auto logStr = ipAddr + methodAndPath + status;

    LOG(WARNING) << "Error: " << logStr << std::endl;
    LOG(WARNING) << "Body: " << req.body;

    auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });
}

void setup(httplib::Server& svr, Host& host) {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  LOG(INFO) << "Setup Https Server";
#else
  LOG(INFO) << "Setup Http Server";
#endif

  setup_security(svr, host);
  setup_errors(svr);
  setup_routing(svr);
}
}  // namespace http::handler