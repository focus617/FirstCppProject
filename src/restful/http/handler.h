#pragma once

#include <httplib.h>

#include "host.hpp"

namespace http::handler {
    
void setup(httplib::Server& svr, Host& host);

}  // namespace http::handler