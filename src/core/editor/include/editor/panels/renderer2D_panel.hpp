#pragma once

#include "renderer/renderer.h"
#include "tools/maths/FVector4.hpp"
#include "ui/panels/panel_window.hpp"

namespace xuzy::UI::Panels {

/**
 * @brief A Panel Window used for OpenGL 2D experimental purpose
 */
class XUZY_API Renderer2DPanel : public PanelWindow {
 public:
  /**
   * @brief Constructor
   * @param p_name
   * @param p_opened
   * @param p_panelSettings
   */
  Renderer2DPanel(const std::string& p_name = "", bool p_opened = true,
                  const Settings::PanelWindowSettings& p_panelSettings =
                      Settings::PanelWindowSettings{});

  /**
   * @brief Destructor
   */
  ~Renderer2DPanel();

  Ref<Renderer::Buffer::AFrameBuffer> get_framebuffer() {
    return m_framebuffer_;
  }

  virtual void on_update(Renderer::Times::Timestep p_ts) override;

  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

 protected:
  virtual void _on_draw_impl() override;

 private:
  Renderer::Camera::OrthographicCameraController m_camera_controller_;

  Ref<Renderer::Buffer::AFrameBuffer> m_framebuffer_;

  Ref<Renderer::ATexture2D> m_check_board_texture_;
  Ref<Renderer::ATexture2D> m_sprite_texture_;

  Ref<Renderer::SubTexture2D> m_default_subtexture_;

  uint32_t m_map_width_, m_map_height;
  std::unordered_map<char, Ref<Renderer::SubTexture2D>> s_texture_map_;
};

}  // namespace xuzy::UI::Panels