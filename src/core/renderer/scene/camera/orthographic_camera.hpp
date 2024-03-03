#pragma once

#include <glm/glm.hpp>

namespace xuzy::Renderer::Camera {

class XUZY_API OrthographicCamera {
 public:
  OrthographicCamera(float p_left, float p_right, float p_bottom, float p_top);

  void set_projection_matrix(float p_left, float p_right, float p_top, float p_bottom);

  const glm::vec3& get_position() const { return m_position_; }

  void set_position(const glm::vec3& p_position) {
    m_position_ = p_position;
    recalculate_view_matrix();
  }

  float get_rotation() const { return m_rotation_; }

  void set_rotation(float p_rotation) {
    m_rotation_ = p_rotation;
    recalculate_view_matrix();
  }

  const glm::mat4& get_projection_matrix() const {
    return m_projection_matrix_;
  }

  const glm::mat4& get_view_matrix() const { return m_view_matrix_; }

  const glm::mat4& get_view_projection_matrix() const {
    return m_view_projection_matrix_;
  }

 private:
  void recalculate_view_matrix();

 private:
  glm::mat4 m_projection_matrix_;
  glm::mat4 m_view_matrix_;
  glm::mat4 m_view_projection_matrix_;

  glm::vec3 m_position_ = {0.0f, 0.0f, 0.0f};
  float m_rotation_ = 0.0f;
};

}  // namespace xuzy::Renderer::Camera