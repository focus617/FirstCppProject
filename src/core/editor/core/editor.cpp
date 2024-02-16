#include "editor/core/editor.hpp"

#include "ui/core/panel_manager.hpp"
#include "ui/panels/panel_menubar.hpp"
#include "editor/panels/editor_main_menubar.hpp"

namespace xuzy::Editor {

Editor::Editor(const std::string& t_app_name, const std::string& t_version)
    : WindowApp{std::move(t_app_name), std::move(t_version)} {

  Ref<UI::PanelManager> panel_manager = CreateRef<UI::PanelManager>();

  panel_manager->CreatePanel<Panels::MainMenuBar>("MainMenuBar");

  push_layer(panel_manager);
}

}  // namespace xuzy::Editor