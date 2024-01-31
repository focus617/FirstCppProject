#pragma once

#include "core/platform.hpp"

#ifdef XUZY_OS_WINDOWS
#  ifndef NOMINMAX
// See
// github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#    define NOMINMAX
#  endif
#endif

#include <glog/logging.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/base.hpp"
#include "core/exception.hpp"
#include "core/platform.hpp"
#include "core/visibility_control.hpp"

#ifdef XUZY_OS_WINDOWS
#  include <Windows.h>
#endif
