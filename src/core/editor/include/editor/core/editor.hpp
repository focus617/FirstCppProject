#pragma once

#include "window/core/window_app.hpp"

namespace xuzy::Editor {

/**
 *  @brief Window Application Scaffold
 */
class XUZY_API Editor : public WindowApp {
 public:
  XUZY_API explicit Editor(const std::string& t_app_name,
                              const std::string& t_version);

  XUZY_API ~Editor() = default;

};

}  // namespace xuzy::Editor