#pragma once

#include "renderer/core/ARenderer_api.hpp"

namespace xuzy::Renderer {

class Renderer {
 public:
  static ARendererAPI::API get_api() { return ARendererAPI::get_api(); }
};

}  // namespace xuzy::Renderer