#pragma once

#include "ui/widgets/AWidget.hpp"
#include "ui/plugins/data_dispatcher.hpp"

namespace xuzy::UI::Widgets {

/**
 * @brief DataWidget are widgets that contains a value. 
 * It is very usefull in combination with DataDispatcher plugin
 */
template <typename T>
class DataWidget : public AWidget {
 public:
  /**
   * @brief Create a DataWidget with the data specification
   * @param p_dataHolder
   */
  DataWidget(T& p_dataHolder) : m_data(p_dataHolder){};

  /**
   * @brief Draw the widget
   */
  virtual void on_draw() override;

  /**
   * @brief Notify that the widget data has changed to allow the data
   * dispatcher to execute its behaviour
   */
  void notify_change();

 private:
  T& m_data;
};

template <typename T>
inline void DataWidget<T>::on_draw() {
  if (enabled) {
    TRY_GATHER(T, m_data);
    AWidget::on_draw();
    TRY_PROVIDE(T, m_data);
  }
}

template <typename T>
inline void DataWidget<T>::notify_change() {
  TRY_NOTIFY_CHANGE(T);
}

}  // namespace xuzy::UI::Widgets
