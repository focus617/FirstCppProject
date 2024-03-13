#pragma once

#include "renderer/renderer.h"
#include "tools/maths/FVector4.hpp"

namespace xuzy::UI {

/**
 * @brief A Layer used for 2D experimental purpose
 */
class XUZY_API Sandbox2D : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Constructor
   */
  Sandbox2D();

  /**
   * @brief Destructor
   */
  ~Sandbox2D();

  virtual void on_attach() override;
  virtual void on_detach() override;
  virtual void on_update(Renderer::Times::Timestep p_ts) override;
  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

  /**
   * @brief Render its components
   */
  virtual void on_draw() override;

 private:
  Renderer::Camera::OrthographicCameraController m_camera_controller_;

  // Temp
  Ref<Renderer::AShader> m_flat_square_shader_;
  Ref<Renderer::Buffer::AVertexArray> m_square_vertex_array_;
  Maths::FVector4 m_square_color_ = Maths::FVector4(0.2f, 0.3f, 0.8f, 1.0f);

  Ref<Renderer::ATexture2D> m_check_board_texture_;
};

}  // namespace xuzy::UI