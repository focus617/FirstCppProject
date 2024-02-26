#pragma once

#include "renderer/buffers/AVertex_array.hpp"
#include "renderer/core/ARenderer_api.hpp"
#include "renderer/resources/shader/AShader.hpp"

namespace xuzy::Renderer {

class XUZY_API Renderer {
 public:
  static void init();
  static void shutdown();

  static void on_window_resize(uint32_t p_width, uint32_t p_height);

  static void begin_scene();
  static void end_scene();

  static void submit(const Ref<AShader>& p_shader,
                     const Ref<Buffer::AVertexArray>& p_vertex_array);

  static ARendererAPI::API get_api() { return ARendererAPI::get_api(); }
};

}  // namespace xuzy::Renderer