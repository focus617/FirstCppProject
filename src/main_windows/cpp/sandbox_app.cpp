#include "app/window_app.hpp"
#include "window/imgui_layer.hpp"

class Sandbox : public xuzy::WindowApp {
 public:
  Sandbox(const std::string& t_app_name, const std::string& t_version)
      : WindowApp{std::move(t_app_name), std::move(t_version)} {
    m_overlay_ = new xuzy::ImGuiLayer();
    push_overlay(m_overlay_);
  }

  ~Sandbox() {
    pop_overlay(m_overlay_);
    delete m_overlay_;
  }

 private:
  xuzy::Layer* m_overlay_;
};

namespace xuzy {

const std::string APP_NAME{"Windows-App"};
const std::string VERSION{"0.0.1"};

App* CreateApplication() {
  App* app = new Sandbox(APP_NAME, VERSION);
  return app;
}

}  // namespace xuzy