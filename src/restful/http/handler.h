#pragma once

#include <httplib.h>

#include "host.h"
namespace http::handler {
void setup(httplib::Server& svr, Host& host);

}  // namespace http::handler