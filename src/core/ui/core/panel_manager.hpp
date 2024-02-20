#pragma once

#include <algorithm>

#include "tools/event/event.hpp"
#include "ui/backend/opengl/imgui_include.h"
#include "ui/imgui/imgui.h"
#include "ui/panels/APanel.hpp"
#include "ui/panels/panel_window.hpp"
#include "window/core/layer.hpp"

namespace xuzy::UI {
/**
 * @brief A Layer handle creation and storage of UI canvas and panels
 */
class XUZY_API PanelManager : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Create Canvas. It will draw each panel
   */
  PanelManager() : ALayer("PanelManager") {}

  virtual void on_attach() override { LOG(INFO) << "PanelManager OnAttach"; }
  virtual void on_detach() override { LOG(INFO) << "PanelManager OnDetach"; }
  virtual void on_update() override;
  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

  /**
   * @brief Draw the canvas and its panels
   */
  virtual void on_draw() override;

    /**
   * @brief Check inputs for menubar shortcuts
   * @param p_deltaTime
   */
  void handle_shortcuts(float p_deltaTime = 0.01f);

  /**
   * Adds a panel to the canvas
   * @param p_panel
   */
  void add_panel(Panels::APanel& p_panel);

  /**
   * @brief Removes a panel from the canvas
   * @param p_panel
   */
  void remove_panel(Panels::APanel& p_panel);

  /**
   * @brief Removes every panels from the canvas
   */
  void remove_all_panels();

  /**
   * @brief Create a panel
   * @param p_id
   * @param p_args
   */
  template <typename T, typename... Args>
  void CreatePanel(const std::string& p_id, Args&&... p_args) {
    if constexpr (std::is_base_of<UI::Panels::PanelWindow, T>::value) {
      m_panel_map_.emplace(
          p_id, CreateScope<T>(p_id, std::forward<Args>(p_args)...));

      // T& instance = *static_cast<T*>(m_panel_map_.at(p_id).get());
      // GetPanelAs<OvEditor::Panels::MenuBar>("Menu Bar")
      //     .RegisterPane(instance.name, instance);

    } else {
      m_panel_map_.emplace(p_id,
                           CreateScope<T>(std::forward<Args>(p_args)...));
    }
    add_panel(*m_panel_map_.at(p_id));
  }

  /**
   * @brief Returns the panel identified by the given id casted in the given
   * type
   * @param p_id
   */
  template <typename T>
  T& GetPanelAs(const std::string& p_id) {
    return *static_cast<T*>(m_panel_map_[p_id].get());
  }

 private:
  std::vector<std::reference_wrapper<Panels::APanel>> m_panels_;

  std::unordered_map<std::string, std::unique_ptr<Panels::APanel>> m_panel_map_;
};

}  // namespace xuzy::UI
