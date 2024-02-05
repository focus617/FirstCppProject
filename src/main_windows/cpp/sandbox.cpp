#include "app/window_app.hpp"
#include "core.h"
#include "core/backend/imgui/imgui_layer.hpp"

using namespace xuzy;

class ExampleLayer : public Layer {
 public:
  ExampleLayer() : Layer("Example") {}

  void on_attach() {}
  void on_detach() {}

  void on_update() override {
    if (Input::is_key_pressed(Key::Tab))
      LOG(INFO) << "Tab key is pressed (poll)!";
  }

  void on_event(Ref<Event> event, bool& handled) override {
    if (event->get_event_id() == EventId::KeyPressed) {
      Ref<KeyPressedEvent> e = std::static_pointer_cast<KeyPressedEvent>(event);
      if (e->get_key_code() == Key::Tab){
        LOG(INFO) << "Tab key is pressed (event)!";
        handled = true;
      }
      LOG(INFO) << "KeyCode is " << (char)(e->get_key_code());
    }
  }
};

class Sandbox : public WindowApp {
 public:
  Sandbox(const std::string& t_app_name, const std::string& t_version)
      : WindowApp{std::move(t_app_name), std::move(t_version)} {
    push_layer(new ExampleLayer());

    m_overlay_ = new ImGuiLayer();
    push_overlay(m_overlay_);

  }

  ~Sandbox() {
    pop_overlay(m_overlay_);
    delete m_overlay_;
  }

 private:
  Layer* m_overlay_;
};

namespace xuzy {

const std::string APP_NAME{"Windows-App"};
const std::string VERSION{"0.0.1"};

App* CreateApplication() {
  App* app = new Sandbox(APP_NAME, VERSION);
  return app;
}

}  // namespace xuzy