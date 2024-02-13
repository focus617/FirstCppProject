#pragma once

#include <memory>

#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Menu {

/**
 * @brief Widget that can be added to a menu list.
 * It is clickable and can be checkable
 */
class MenuItem : public DataWidget<bool>,
                 public std::enable_shared_from_this<MenuItem> {
 public:
  /**
   * Constructor
   * @param p_name
   * @param p_shortcut
   * @param p_checkable
   * @param p_checked
   */
  MenuItem(const std::string& p_name, const std::string& p_shortcut = "",
           bool p_checkable = false, bool p_checked = false);

  std::shared_ptr<MenuItem> getptr() { return shared_from_this(); }

 protected:
  virtual void _on_draw_impl() override;

 private:
  std::string name;
  std::string shortcut;
  bool checkable;
  bool checked;
  bool selected;
  EventDispatcher<void()> event_dispatcher;
};

}  // namespace xuzy::UI::Widgets::Menu