#pragma once

#include "renderer/buffers/AVertex_array.hpp"
#include "renderer/renderer/ARenderer_api.hpp"
#include "renderer/resources/shader/AShader.hpp"
#include "renderer/scene/camera/orthographic_camera.hpp"

namespace xuzy::Renderer {

class XUZY_API Renderer {
 public:
  static void init();
  static void shutdown();

  static void on_window_resize(uint32_t p_width, uint32_t p_height);

  static void begin_scene(Camera::OrthographicCamera& p_camera);
  static void end_scene();

  static void submit(const Ref<AShader>& p_shader,
                     const Ref<Buffer::AVertexArray>& p_vertex_array,
                     const glm::mat4& p_transform = glm::mat4(1.0f));

  static ARendererAPI::API get_api() { return ARendererAPI::get_api(); }

 private:
  struct SceneData {
    glm::mat4 view_projection_matrix;
  };

  static Scope<SceneData> s_scene_data;
};

}  // namespace xuzy::Renderer