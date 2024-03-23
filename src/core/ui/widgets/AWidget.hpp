#pragma once

#include <string>

#include "ui/imgui/imgui.h"

#include "window/API/IDrawable.h"
#include "ui/plugins/Pluginable.hpp"

namespace xuzy::UI::Internal {
class WidgetContainer;
}

namespace xuzy::UI::Widgets {

/**
 * @brief A AWidget is the base class for any widget of UI.
 * It is basically a visual element that can be placed into a panel.
 * It is drawable and can receive plugins.
 */
class XUZY_API AWidget
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

  /**
   * @brief Defines the tool tip of this widget
   * @param p_tool_desc
   */
  void set_tool_tip_desc(const std::string& p_desc);

  /**
   * @brief Enable or disable showing of widget tip
   */
  void enable_tool_tip(bool p_enable);

  /**
   * @brief Returns true if enable showing of widget tip
   */
  bool has_enable_tool_tip() const;

 protected:
  virtual void _on_draw_impl() = 0;

 public:
  bool enabled = true;
  bool lineBreak = true;

 protected:
  Internal::WidgetContainer* m_parent;
  std::string m_widget_id = "?";
  std::string m_tool_tip = "";
  bool m_auto_execute_plugins = true;

 private:
  static uint64_t __WIDGET_ID_INCREMENT;
  bool m_destroyed = false;
  bool m_enable_tool_tip = false;
};

}  // namespace xuzy::UI::Widgets