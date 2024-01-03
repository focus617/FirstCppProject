#include "log.h"
#include "httplib.h"

int test_httplib()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    LOG_INFO("%s: CPPHTTPLIB_OPENSSL_SUPPORT is defined", __FUNCTION__);
    httplib::SSLClient cli("jsonplaceholder.typicode.com");
    cli.enable_server_certificate_verification(true);
#else
        httplib::Client cli(http://jsonplaceholder.typicode.com);
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