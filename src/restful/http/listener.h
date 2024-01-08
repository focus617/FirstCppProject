#pragma once

#include <httplib.h>

#include "host.h"

namespace http::listener
{
    
    void run(Host host);

} // namespace http::listener