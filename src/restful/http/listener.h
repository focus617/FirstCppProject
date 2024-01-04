#pragma once

#include <httplib.h>

#include "host.h"

namespace http::listener
{
    // HTTP Server
    httplib::Server *p_server = nullptr;

    void run(Host host);
    void stop();

} // namespace http::listener