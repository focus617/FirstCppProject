#pragma once

#include <string>

#include "window/API/IDrawable.h"
#include "ui/plugins/Pluginable.h"

namespace xuzy::UI::Internal {
class WidgetContainer;
}

namespace xuzy::UI::Widgets {

/**
 * @brief A AWidget is the base class for any widget of UI.
 * It is basically a visual element that can be placed into a panel.
 * It is drawable and can receive plugins.
 */
class AWidget
    : public xuzy::Window::API::IDrawable, public Plugins::Pluginable
{
 public:
  /**
   * @brief Constructor
   */
  AWidget();

  /**
   * @brief Draw the widget on the panel
   */
  virtual void on_draw() override;

  /**
   * @brief Link the widget to another one by making its id identical to the
   * given widget
   * @param p_widget
   */
  void link_to(const AWidget& p_widget);

  /**
   * @brief Mark the widget as destroyed. It will be removed from the owner
   * panel by the garbage collector
   */
  void destroy();

  /**
   * @brief Returns true if the widget is marked as destroyed
   */
  bool is_destroyed() const;

  /**
   * @brief Defines the parent of this widget
   * @param p_parent
   */
  void set_parent(Internal::WidgetContainer* p_parent);

  /**
   * @brief Returns true if the widget has a parent
   */
  bool has_parent() const;

  /**
   * @brief Returns the parent of the widget or nullptr if there is no parent
   */
  Internal::WidgetContainer* get_parent();

 protected:
  virtual void _on_draw_impl() = 0;

 public:
  bool enabled = true;
  bool lineBreak = true;

 protected:
  Internal::WidgetContainer* m_parent;
  std::string m_widget_id = "?";
  bool m_auto_execute_plugins = true;

 private:
  static uint64_t __WIDGET_ID_INCREMENT;
  bool m_destroyed = false;
};

}  // namespace xuzy::UI::Widgets