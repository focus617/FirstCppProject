#pragma once

#include <memory>
#include <vector>

#include "tools/maths/FVector2.hpp"
#include "tools/event/event.hpp"
#include "ui/panels/APanel.hpp"
#include "ui/settings/alignment.h"

namespace xuzy::UI::Panels {
/**
 * APanelTransformable is a panel that is localized in the canvas
 */
class XUZY_API APanelTransformable : public APanel {
 public:
  /**
   * Create a APanelTransformable
   * @param p_defaultPosition
   * @param p_defaultSize
   * @param p_defaultHorizontalAlignment
   * @param p_defaultVerticalAlignment
   * @param p_ignoreConfigFile
   */
  APanelTransformable(
      const Maths::FVector2& p_defaultPosition =
          Maths::FVector2(-1.f, -1.f),
      const Maths::FVector2& p_defaultSize = Maths::FVector2(-1.f,
                                                                         -1.f),
      UI::Settings::HorizontalAlignment p_defaultHorizontalAlignment =
          UI::Settings::HorizontalAlignment::LEFT,
      UI::Settings::VerticalAlignment p_defaultVerticalAlignment =
          UI::Settings::VerticalAlignment::TOP,
      bool p_ignoreConfigFile = false);

  /**
   * Defines the position of the panel
   * @param p_position
   */
  void set_position(const Maths::FVector2& p_position);

  /**
   * Defines the size of the panel
   * @param p_size
   */
  void set_size(const Maths::FVector2& p_size);

  /**
   * Defines the alignment of the panel
   * @param p_horizontalAlignment
   * @param p_verticalAligment
   */
  void set_alignment(UI::Settings::HorizontalAlignment p_horizontalAlignment,
                    UI::Settings::VerticalAlignment p_verticalAligment);

  /**
   * Returns the current position of the panel
   */
  const Maths::FVector2& get_position() const;

  /**
   * Returns the current size of the panel
   */
  const Maths::FVector2& get_size() const;

  /**
   * Returns the current horizontal alignment of the panel
   */
  UI::Settings::HorizontalAlignment get_horizontal_alignment() const;

  /**
   * Returns the current vertical alignment of the panel
   */
  UI::Settings::VerticalAlignment get_vertical_alignment() const;

 protected:
  virtual void _on_draw_impl() override;

 private:
  Maths::FVector2 calculate_position_alignment_offset(
      bool p_default = false);

  void update_position();
  void update_size();
  void copy_imgui_position();
  void copy_imgui_size();

 public:
  bool auto_size = true;

 protected:
  Maths::FVector2 m_default_position;
  Maths::FVector2 m_default_size;
  UI::Settings::HorizontalAlignment m_default_horizontal_alignment;
  UI::Settings::VerticalAlignment m_default_vertical_alignment;
  bool m_ignore_configfile;

  Maths::FVector2 m_position = Maths::FVector2(0.0f, 0.0f);
  Maths::FVector2 m_size = Maths::FVector2(0.0f, 0.0f);

  bool m_position_changed = false;
  bool m_size_changed = false;

  UI::Settings::HorizontalAlignment m_horizontal_alignment =
      UI::Settings::HorizontalAlignment::LEFT;
  UI::Settings::VerticalAlignment m_vertical_alignment =
      UI::Settings::VerticalAlignment::TOP;

  bool m_alignment_changed = false;
  bool m_first_frame = true;
};

}  // namespace xuzy::UI::Panels