#pragma once

#include <vector>

#include "core/base.hpp"
#include "ui/widgets/AWidget.hpp"

namespace xuzy::UI::Internal {

/**
 * @brief The base class for any widget container
 */
class XUZY_API WidgetContainer {
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
  void remove_widget(Ref<Widgets::AWidget>& p_widget);

  /**
   * @brief Remove all widgets from the container
   */
  void remove_all_widgets();

  /**
   * @brief Consider a widget
   * @param p_widget
   */
  void consider_widget(Ref<Widgets::AWidget>& p_widget);

  /**
   * @brief Unconsider a widget
   * @param p_widget
   */
  void unconsider_widget(Ref<Widgets::AWidget>& p_widget);

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
   * @return std::shared_ptr<T>
   */
  template <typename T, typename... Args>
  Ref<T> CreateWidget(Args&&... p_args) {
    Ref<T> instance = CreateRef<T>(p_args...);
    instance->set_parent(this);
    m_widgets.emplace_back(instance);
    return instance;
  }

  /**
   * @brief Returns the widgets and their memory management mode
   */
  std::vector<Ref<UI::Widgets::AWidget>>& GetWidgets();

 protected:
  std::vector<Ref<UI::Widgets::AWidget>> m_widgets;
  bool m_reversed_draw_order = false;
};

}  // namespace xuzy::UI::Internal