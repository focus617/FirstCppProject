#pragma once

#include <vector>

#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/layout/group.hpp"

namespace xuzy::UI::Widgets::Menu {

/**
 * Widget that behave like a group with a menu display
 */
class MenuList : public Layout::Group,
                 public std::enable_shared_from_this<MenuList> {
 public:
  /**
   * @brief Constructor
   * @param p_name
   * @param p_locked
   */
  MenuList(const std::string& p_name, bool p_locked = false);

  std::shared_ptr<MenuList> getptr() { return shared_from_this(); }

 protected:
  virtual void _on_draw_impl() override;

 public:
  std::string name;
  bool locked;
  Events::EventDispatcher<void()> event_dispatcher;

 private:
  bool m_opened;
};

}  // namespace xuzy::UI::Widgets::Menu