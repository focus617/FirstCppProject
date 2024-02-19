#pragma once

#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/data_widget.hpp"

namespace xuzy::UI::Widgets::Selection {
class RadioButtonLinker;

/**
 * @brief Widget that represent a button that is selectable. 
 * Only one radio button can be selected in the same RadioButtonLinker
 */
class RadioButton : public DataWidget<bool>,
                    public std::enable_shared_from_this<RadioButton> {
  friend RadioButtonLinker;

 public:
  /**
   * @brief Constructor
   * @param p_selected
   * @param p_label
   */
  RadioButton(bool p_selected = false, const std::string& p_label = "");

  Ref<RadioButton> getptr() { return shared_from_this(); }

  /**
   * @brief Make the radio button selected
   */
  void select();

  /**
   * @brief Returns true if the radio button is selected
   */
  bool is_selected() const;

  /**
   * @brief Returns the radio button ID
   */
  bool get_radio_id() const;

 protected:
  void _on_draw_impl() override;

 public:
  std::string label;

  Events::EventDispatcher<void()> event_dispatcher;

 private:
  bool m_selected = false;
  int m_radio_id = 0;
};
}  // namespace xuzy::UI::Widgets::Selection