#include "renderer/renderer/renderer.hpp"

#include "renderer/renderer/render_command.hpp"

namespace xuzy::Renderer {

Scope<Renderer::SceneData> Renderer::s_scene_data =
    CreateScope<Renderer::SceneData>();

void Renderer::init() { RenderCommand::init(); }

void Renderer::shutdown() {}

void Renderer::on_window_resize(uint32_t p_width, uint32_t p_height) {
  RenderCommand::set_viewport(0, 0, p_width, p_height);
}

void Renderer::begin_scene(Camera::OrthographicCamera& p_camera) {
  s_scene_data->view_projection_matrix = p_camera.get_view_projection_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const Ref<AShader>& p_shader,
                      const Ref<Buffer::AVertexArray>& p_vertex_array,
                      const glm::mat4& p_transform) {
  p_shader->bind();
  p_shader->set_mat4("u_ViewProjection", s_scene_data->view_projection_matrix);
  p_shader->set_mat4("u_Transform", p_transform);

  p_vertex_array->bind();
  RenderCommand::draw_indexed(p_vertex_array);
}

}  // namespace xuzy::Renderer