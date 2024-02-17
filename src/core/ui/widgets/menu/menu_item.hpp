#pragma once

#include <memory>

#include "core/base.hpp"
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

  Ref<MenuItem> getptr() { return shared_from_this(); }

  std::string get_name() { return name; }

 protected:
  virtual void _on_draw_impl() override;

 public:
  Events::EventDispatcher<void()> event_dispatcher;
  
 private:
  std::string name;
  std::string shortcut;
  bool checkable;
  bool checked;
  bool selected;
};

}  // namespace xuzy::UI::Widgets::Menu