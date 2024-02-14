#pragma once

#include <vector>

#include "ui/internal/memory_mode.h"
#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Internal {

/**
 * @brief The base class for any widget container
 */
class WidgetContainer {
 public:
  /**
   * @brief Constructor
   */
  WidgetContainer() = default;

  /**
   * @brief Destructor. Handle the memory de-allocation of every widgets
   * that are internally managed
   */
  virtual ~WidgetContainer();

  /**
   * @brief Remove a widget from the container
   * @param p_widget
   */
  void remove_widget(xuzy::UI::Widgets::AWidget& p_widget);

  /**
   * @brief Remove all widgets from the container
   */
  void remove_all_widgets();

  /**
   * @brief Consider a widget
   * @param p_manageMemory
   */
  void consider_widget(xuzy::UI::Widgets::AWidget& p_widget,
                      bool p_manageMemory = true);

  /**
   * @brief Unconsider a widget
   * @param p_widget
   */
  void unconsider_widget(xuzy::UI::Widgets::AWidget& p_widget);

  /**
   * @brief Collect garbages by removing widgets marked as "Destroyed"
   */
  void collect_garbages();

  /**
   * @brief Draw every widgets
   */
  void draw_widgets();

  /**
   * @brief Allow the user to reverse the draw order of this widget container
   */
  void reverse_draw_order(bool reversed = true);

  /**
   * @brief Create a widget
   * @param p_args
   */
  template <typename T, typename... Args>
  T& CreateWidget(Args&&... p_args) {
    m_widgets.emplace_back(new T(p_args...), MemoryMode::INTERNAL_MANAGMENT);
    T& instance = *reinterpret_cast<T*>(m_widgets.back().first);
    instance.set_parent(this);
    return instance;
  }

  /**
   * @brief Returns the widgets and their memory management mode
   */
  std::vector<std::pair<xuzy::UI::Widgets::AWidget*, MemoryMode>>& GetWidgets();

 protected:
  std::vector<std::pair<xuzy::UI::Widgets::AWidget*, MemoryMode>> m_widgets;
  bool m_reversedDrawOrder = false;
};

}  // namespace xuzy::UI::Internal