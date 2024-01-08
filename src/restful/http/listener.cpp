#include <glog/logging.h>

#include "listener.h"
#include "handler.h"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"
#endif

namespace http::listener
{

    void run(Host host)
    {
        // HTTP Server
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        httplib::SSLServer server(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
        httplib::Server server;
#endif

        LOG(INFO) << "Launching Http Server in thread.";

        // Setup listening IP address and routing
        handler::setup(server, host);

        // Default Routing: Stop the server when the user access /stop
        server.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
                   {                
                server.stop(); 
                LOG(INFO) << "Http Server Stopped.";

                res.set_redirect("/"); });

        LOG(INFO) << "Http Server Start at " << host.ip << ":" << host.port;

        // Listen server to port
        server.listen(host.ip, host.port);
    }

} // namespace http::listener