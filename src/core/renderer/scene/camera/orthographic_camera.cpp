#include "renderer/scene/camera/orthographic_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace xuzy::Renderer::Camera {

OrthographicCamera::OrthographicCamera(float p_left, float p_right,
                                       float p_bottom, float p_top)
    : m_projection_matrix_(
          glm::ortho(p_left, p_right, p_bottom, p_top, -1.0f, 1.0f)),
      m_view_matrix_(1.0f) {
  m_view_projection_matrix_ = m_projection_matrix_ * m_view_matrix_;
}

void OrthographicCamera::set_projection_matrix(float p_left, float p_right,
                                               float p_bottom, float p_top) {
  m_projection_matrix_ =
      glm::ortho(p_left, p_right, p_bottom, p_top, -1.0f, 1.0f);
  m_view_projection_matrix_ = m_projection_matrix_ * m_view_matrix_;
}

void OrthographicCamera::recalculate_view_matrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position_) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_),
                                    glm::vec3(0, 0, 1));

  m_view_matrix_ = glm::inverse(transform);
  m_view_projection_matrix_ = m_projection_matrix_ * m_view_matrix_;
}

}  // namespace xuzy::Renderer::Camera