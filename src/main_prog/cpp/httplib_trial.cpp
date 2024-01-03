#include <glog/logging.h>
#include <glog/stl_logging.h>
#include <httplib.h>

#include "host.h"

using namespace httplib;

int test_httplib_client()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    LOG(INFO) << __FUNCTION__ << ": CPPHTTPLIB_OPENSSL_SUPPORT is defined";

    httplib::SSLClient cli("jsonplaceholder.typicode.com");
    cli.enable_server_certificate_verification(true);
#else
        Client cli(http://jsonplaceholder.typicode.com);
#endif

    if (auto res = cli.Get("/todos/1"))
    {
        if (res->status == 200)
        {
            for (auto &header : res->headers)
            {
                std::cout << header.first << " : " << header.second << std::endl;
            }

            std::cout << res->body << std::endl;
        }
    }
    return 0;
}

int test_httplib_server()
{
    // HTTP
    Server svr;
    LOG(INFO) << "Server Start...";

    // Index page
    svr.Get("/", [](const Request &, Response &res)
            { res.set_content("<html><body><h1>Hello World!</h1></body></html>", "text/html"); });

    // Match the request path against a regular expression and extract its captures
    svr.Get(R"(/numbers/(\d+))", [&](const Request &req, Response &res)
            {
                auto numbers = req.matches[1];
                res.set_content(numbers, "text/plain"); });

    // Stop the sever when the user access /stop
    svr.Get("/stop", [&](const Request &req, Response &res)
            {
                LOG(INFO) << "Server Stopped.";
                svr.stop(); 
                res.set_redirect("/"); });

    http::host host("0.0.0.0", 8080);

    // Listen server to port
    svr.listen(host.ip, host.port);

    return 0;
}