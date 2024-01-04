#pragma once

#include <httplib.h>

#include "host.h"

namespace http::listener
{
    void run(Host host);
    void wait_until_ready();
    void stop();

} // namespace http::listener