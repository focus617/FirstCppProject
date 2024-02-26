#include "renderer/core/renderer.hpp"

#include "renderer/core/render_command.hpp"

namespace xuzy::Renderer {

void Renderer::init() { RenderCommand::init(); }

void Renderer::shutdown() {}

void Renderer::on_window_resize(uint32_t p_width, uint32_t p_height) {
  RenderCommand::set_viewport(0, 0, p_width, p_height);
}

void Renderer::begin_scene() {}
void Renderer::end_scene() {}

void Renderer::submit(const Ref<AShader>& p_shader,
                      const Ref<Buffer::AVertexArray>& p_vertex_array) {
  p_shader->bind();

  p_vertex_array->bind();
  RenderCommand::draw_indexed(p_vertex_array);
}

}  // namespace xuzy::Renderer