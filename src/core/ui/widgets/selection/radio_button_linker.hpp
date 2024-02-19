#pragma once

#include "tools/event/event.hpp"
#include "tools/event/event_dispatcher.hpp"
#include "ui/widgets/selection/radio_button.hpp"

namespace xuzy::UI::Widgets::Selection {
/**
 * @brief Handle the link of radio buttons.
 * Necessary to enable the correct behaviour of radio buttons
 */
class RadioButtonLinker
    : public DataWidget<int>,
      public std::enable_shared_from_this<RadioButtonLinker> {
 public:
  /**
   * @brief Constructor
   */
  RadioButtonLinker();

  Ref<RadioButtonLinker> getptr() { return shared_from_this(); }

  /**
   * @brief Link the given radio button
   */
  void link(RadioButton& p_radio_button);

  /**
   * @brief Unlink the given radio button
   */
  void unlink(RadioButton& p_radio_button);

  /**
   * @brief Returns the id of the selected radio button
   */
  int get_selected() const;

 protected:
  void _on_draw_impl() override;

 private:
  void on_radio_button_clicked(Ref<Events::Event> evt, bool& handled);

 public:
  Events::EventDispatcher<void()> event_dispatcher;

 private:
  int m_available_radio_id = 0;
  int m_selected = -1;
  std::vector<std::pair<Events::HandlerID, std::reference_wrapper<RadioButton>>>
      m_radio_buttons;
};

}  // namespace xuzy::UI::Widgets::Selection