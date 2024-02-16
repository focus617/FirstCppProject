#include "ui/core/panel_manager.hpp"

#include "tools/event/key_event.hpp"
#include "window/inputs/input_manager.hpp"
#include "window/inputs/key_state.h"

namespace xuzy::UI {

#define Input xuzy::Window::Inputs

void PanelManager::on_update() {
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
  // tell if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data
  // to your main application, or clear/overwrite your copy of the mouse
  // data.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
  // data to your main application, or clear/overwrite your copy of the
  // keyboard data. Generally you may always pass all inputs to dear imgui,
  // and hide them from your application based on those two flags.
  handle_shortcuts();
}

void PanelManager::handle_shortcuts(float p_deltaTime) {
  if (Input::InputManager::get_key_state(Input::Key::LeftControl) ==
      Input::Key::KeyState::KEY_PRESS) {

    if (Input::InputManager::is_key_pressed(Input::Key::Tab))
      LOG(INFO) << "LCtrl+Tab key is pressed (poll)!";
  }
}

void PanelManager::on_event(Ref<Event> event, bool& handled) {
  if (event->get_event_id() == EventId::KeyPressed) {
    Ref<KeyPressedEvent> e = std::static_pointer_cast<KeyPressedEvent>(event);
    if (e->get_key_code() == Input::Key::Tab) {
      LOG(INFO) << "Tab key is pressed (event)!";
      handled = true;
    }
    LOG(INFO) << "KeyCode is " << (char)(e->get_key_code());
  }
}

void PanelManager::on_draw() {
  for (auto& panel : m_panels_) panel.get().on_draw();
}

void PanelManager::add_panel(Panels::APanel& p_panel) {
  m_panels_.push_back(std::ref(p_panel));
}

void PanelManager::remove_panel(Panels::APanel& p_panel) {
  m_panels_.erase(std::remove_if(
      m_panels_.begin(), m_panels_.end(),
      [&p_panel](std::reference_wrapper<Panels::APanel>& p_item) {
        return &p_panel == &p_item.get();
      }));
}

void PanelManager::remove_all_panels() { m_panels_.clear(); }

}  // namespace xuzy::UI