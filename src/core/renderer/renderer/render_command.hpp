#pragma once

#include "renderer/buffers/AVertex_array.hpp"
#include "renderer/renderer/ARenderer_api.hpp"

namespace xuzy::Renderer {

class XUZY_API RenderCommand {
 public:
  static void init() { s_renderer_api->init(); }

  static void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) {
    s_renderer_api->set_viewport(x, y, width, height);
  }

  static void set_clear_color(float p_red, float p_green, float p_blue,
                              float p_alpha = 1.0f) {
    s_renderer_api->set_clear_color(p_red, p_green, p_blue, p_alpha);
  }

  static void clear() { s_renderer_api->clear(); }

  static void draw_indexed(
      const Ref<Buffer::AVertexArray>& p_vertex_array,
      uint32_t p_index_count = 0) {
    s_renderer_api->draw_indexed(p_vertex_array, p_index_count);
  }

 private:
  static Scope<ARendererAPI> s_renderer_api;
};

}  // namespace xuzy::Renderer