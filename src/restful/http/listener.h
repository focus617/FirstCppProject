#pragma once

#include <httplib.h>

#include "host.h"
#include "visibility_control.hpp"

namespace http::listener {

XUZY_API void run(Host host);

}  // namespace http::listener