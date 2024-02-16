#include "editor/panels/editor_main_menubar.hpp"

#include "ui/widgets/text/text.hpp"
#include "ui/widgets/visual/separator.hpp"

namespace xuzy::Editor::Panels {

// 一个能够处理事件的静态函数
void EventHandler(Ref<Event> evt, bool& handled) {
  std::cout << "Event Callback: " << *evt << std::endl;
}

MainMenuBar::MainMenuBar() : UI::Panels::PanelMenuBar() {
  create_file_menu();
  create_edit_menu();
  create_help_menu();
}

void MainMenuBar::create_file_menu() {
  auto file_menu = CreateWidget<UI::Widgets::Menu::MenuList>("File");
  file_menu->event_dispatcher += EventHandler;
  auto file_new_menu =
      file_menu->CreateWidget<UI::Widgets::Menu::MenuItem>("New", "CTRL + N");
  file_new_menu->event_dispatcher += EventHandler;
}

void MainMenuBar::create_edit_menu() {
  auto edit_menu = CreateWidget<UI::Widgets::Menu::MenuList>("Edit");
  edit_menu->CreateWidget<UI::Widgets::Menu::MenuItem>("Search", "CTRL + F");
}

void MainMenuBar::create_help_menu() {
  auto help_menu = CreateWidget<UI::Widgets::Menu::MenuList>("Help");
  help_menu->CreateWidget<UI::Widgets::Menu::MenuItem>("About");
  help_menu->CreateWidget<UI::Widgets::Visual::Separator>();
  help_menu->CreateWidget<UI::Widgets::Text::Text>("Version: 1.3.0");
}

}  // namespace xuzy::Editor::Panels