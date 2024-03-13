#pragma once

#include "renderer/scene/camera/orthographic_camera.hpp"
#include "tools/event/application_event.hpp"
#include "tools/event/mouse_event.hpp"
#include "window/core/time_step.hpp"

namespace xuzy::Renderer::Camera {

class XUZY_API OrthographicCameraController {
 public:
  OrthographicCameraController(float p_aspect_ratio,
                               bool p_enable_rotation = false);

  void on_update(Times::Timestep p_ts);
  void on_event(Ref<Events::Event> event, bool& handled);

  OrthographicCamera& get_camera() { return m_camera_; }
  const OrthographicCamera& get_camera() const { return m_camera_; }

  float get_zoom_level() const { return m_zoom_level_; }
  void set_zoom_level(float p_level) { m_zoom_level_ = p_level; }

 private:
  bool on_mouse_scrolled(Ref<Events::MouseScrolledEvent> e);
  bool on_window_resized(Ref<Events::WindowResizeEvent> e);

 private:
  float m_aspect_ratio_;
  float m_zoom_level_ = 1.0f;

  OrthographicCamera m_camera_;

  bool m_enable_rotation_;

  glm::vec3 m_camera_position_ = {0.0f, 0.0f, 0.0f};
  float m_camera_rotation_ = 0.0f; //In degrees, in the anti-clockwise direction
  float m_camera_translation_speed_ = 5.0f, m_camera_rotate_speed_ = 180.0f;
};

}  // namespace xuzy::Renderer::Camera