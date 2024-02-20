#pragma once

#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::InputField {
/**
 * @brief Input widget of type string
 */
class InputText : public DataWidget<std::string>,
                  public std::enable_shared_from_this<InputText> {
 public:
  /**
   * @brief Constructor
   * @param p_content
   * @param p_label
   */
  InputText(const std::string& p_content = "", const std::string& p_label = "");

  Ref<InputText> getptr() { return this->shared_from_this(); }

 protected:
  void _on_draw_impl() override;

 public:
  std::string content;
  std::string label;
  bool select_all_on_click = false;

  Events::EventDispatcher<void()> event_dispatcher;
};

}  // namespace xuzy::UI::Widgets::InputField