#pragma once

#include "renderer/renderer.h"

#include "tools/maths/FVector3.hpp"

namespace xuzy::UI {
/**
 * @brief A Layer used for experimental purpose
 */
class XUZY_API ExampleLayer : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Constructor
   */
  ExampleLayer();

  /**
   * @brief Destructor
   */
  ~ExampleLayer();

  virtual void on_attach() override { LOG(INFO) << "ExampleLayer OnAttach"; }
  virtual void on_detach() override { LOG(INFO) << "ExampleLayer OnDetach"; }
  virtual void on_update(Renderer::Times::Timestep p_ts) override;
  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

  /**
   * @brief Render its components
   */
  virtual void on_draw() override;

 private:
  Ref<Renderer::AShader> m_triangle_shader_;
  Ref<Renderer::Buffer::AVertexArray> m_triangle_vertex_array_;

  Ref<Renderer::AShader> m_square_shader_;
  Ref<Renderer::Buffer::AVertexArray> m_square_vertex_array_;
  Maths::FVector3 m_square_color_ = Maths::FVector3(0.2f, 0.3f, 0.8f);

  Renderer::Camera::OrthographicCamera m_camera_;
  glm::vec3 m_camera_position_;
  float m_camera_rotation_;
  float m_camera_move_speed_ = 5.0f;
  float m_camera_rotate_speed_ = 180.0f;
};

}  // namespace xuzy::UI