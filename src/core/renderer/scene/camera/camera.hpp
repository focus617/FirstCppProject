#pragma once

#include "tools/maths/FMatrix4.hpp"

namespace xuzy::Renderer::Camera {

/**
 * @brief Represents a camera.
 * Wraps projection and view calculation based on applied rotation and
 * the given positions.
 */
class Camera {
 public:
  /**
   * @brief Constructor
   */
  Camera() = default;
  Camera(const Maths::FMatrix4& projection) : m_projection_matrix(projection) {}

  /**
   * @brief Destructor
   */
  virtual ~Camera() = default;

  /**
   * @brief Returns the cached projection matrix
   */
  const Maths::FMatrix4& get_projection_matrix() const {
    return m_projection_matrix;
  }

 protected:
  Maths::FMatrix4 m_projection_matrix = Maths::FMatrix4::Identity;
  Maths::FMatrix4 m_view_matrix;
};

} // namespace xuzy::Renderer::Camera