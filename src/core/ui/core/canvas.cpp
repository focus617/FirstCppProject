#include "canvas.hpp"

#include "tools/event/key_event.hpp"
#include "window/core/inputs/input_manager.hpp"

namespace xuzy::UI {

#define Input xuzy::Window::Inputs

void Canvas::on_update() {
  if (Input::InputManager::is_key_pressed(Input::Key::Tab))
    LOG(INFO) << "Tab key is pressed (poll)!";
}

void Canvas::on_event(Ref<Event> event, bool& handled) {
  if (event->get_event_id() == EventId::KeyPressed) {
    Ref<KeyPressedEvent> e = std::static_pointer_cast<KeyPressedEvent>(event);
    if (e->get_key_code() == Input::Key::Tab) {
      LOG(INFO) << "Tab key is pressed (event)!";
      handled = true;
    }
    LOG(INFO) << "KeyCode is " << (char)(e->get_key_code());
  }
}

void Canvas::on_draw() {
  if (m_is_dockspace_) {
    // Create an explicit docking node within an existing window.
    // - Drag from window title bar or their tab to dock/undock. Hold SHIFT to
    // disable docking.
    // - Drag from window menu button (upper-left button) to undock an entire
    // node (all windows).

    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("##dockspace", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::SetWindowPos({0.f, 0.f});
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetWindowSize({(float)displaySize.x, (float)displaySize.y});
    ImGui::End();

    ImGui::PopStyleVar(3);
  }

  for (auto& panel : m_panels_) panel.get().on_draw();
}

void Canvas::add_panel(Panels::APanel& p_panel) {
  m_panels_.push_back(std::ref(p_panel));
}

void Canvas::remove_panel(Panels::APanel& p_panel) {
  m_panels_.erase(std::remove_if(
      m_panels_.begin(), m_panels_.end(),
      [&p_panel](std::reference_wrapper<Panels::APanel>& p_item) {
        return &p_panel == &p_item.get();
      }));
}

void Canvas::remove_all_panels() { m_panels_.clear(); }

void Canvas::make_dockspace(bool p_state) { m_is_dockspace_ = p_state; }

bool Canvas::is_dockspace() const { return m_is_dockspace_; }

}  // namespace xuzy::UI