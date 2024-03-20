#include "pch.h"

#include "renderer/scene/camera/orthographic_camera_controller.hpp"

#include "window/inputs/input_manager.hpp"

namespace xuzy::Renderer::Camera {

OrthographicCameraController::OrthographicCameraController(
    float p_aspect_ratio, bool p_enable_rotation)
    : m_aspect_ratio_(p_aspect_ratio),
      m_camera_(-m_aspect_ratio_ * m_zoom_level_,
                m_aspect_ratio_ * m_zoom_level_, -m_zoom_level_, m_zoom_level_),
      m_enable_rotation_(p_enable_rotation) {}

void OrthographicCameraController::on_update(Times::Timestep p_ts) {
  XUZY_PROFILE_FUNCTION();

  if (Window::Inputs::InputManager::is_key_pressed(Window::Inputs::Key::A)) {
    m_camera_position_.x += m_camera_translation_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::D)) {
    m_camera_position_.x -= m_camera_translation_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::W)) {
    m_camera_position_.y -= m_camera_translation_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::S)) {
    m_camera_position_.y += m_camera_translation_speed_ * p_ts;
  }

  m_camera_.set_position(m_camera_position_);

  if (m_enable_rotation_) {
    if (Window::Inputs::InputManager::is_key_pressed(Window::Inputs::Key::Q)) {
      m_camera_rotation_ += m_camera_rotate_speed_ * p_ts;
    } else if (Window::Inputs::InputManager::is_key_pressed(
                   Window::Inputs::Key::E)) {
      m_camera_rotation_ -= m_camera_rotate_speed_ * p_ts;
    }

    m_camera_.set_rotation(m_camera_rotation_);
  }

  m_camera_translation_speed_ = m_zoom_level_;
}

void OrthographicCameraController::set_zoom_level(float p_level) {
  m_zoom_level_ = std::max(p_level, 0.25f);
  calculate_view();
}

void OrthographicCameraController::calculate_view() {
  m_camera_.set_projection_matrix(-m_aspect_ratio_ * m_zoom_level_,
                                  m_aspect_ratio_ * m_zoom_level_,
                                  -m_zoom_level_, m_zoom_level_);
}

void OrthographicCameraController::on_event(Ref<Events::Event> event,
                                            bool& handled) {
  XUZY_PROFILE_FUNCTION();

  handled = false;

  switch (event->get_event_id()) {
    case Events::EventId::MouseScrolled:
      handled = on_mouse_scrolled(
          std::static_pointer_cast<Events::MouseScrolledEvent>(event));
      return;

    case Events::EventId::WindowResize:
      handled = on_window_resized(
          std::static_pointer_cast<Events::WindowResizeEvent>(event));
      break;

    default:
      break;
  }
}

bool OrthographicCameraController::on_mouse_scrolled(
    Ref<Events::MouseScrolledEvent> e) {
  LOG(INFO) << "Mouse Scrolled Event: " << *e << std::endl;

  m_zoom_level_ -= e->get_y_offset() * 0.25f;
  m_zoom_level_ = std::max(m_zoom_level_, 0.25f);
  calculate_view();
  return true;
}

bool OrthographicCameraController::on_window_resized(
    Ref<Events::WindowResizeEvent> e) {
  LOG(INFO) << "Window Resized Event: " << *e << std::endl;

  if ((e->get_width() == 0) || (e->get_height() == 0)) {
    return false;
  }

  m_aspect_ratio_ = (float)e->get_width() / (float)e->get_height();
  calculate_view();
  return false;
}

}  // namespace xuzy::Renderer::Camera