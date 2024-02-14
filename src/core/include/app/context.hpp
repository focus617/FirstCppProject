#pragma once

namespace xuzy::Core {

/**
 * @brief The Context handle the engine features setup
 */
class Context {
 public:
  /**
   * Constructor
   * @param p_projectPath
   * @param p_projectName
   */
  Context(const std::string& p_app_name, const std::string& p_version);

  /**
   * Destructor
   */
  ~Context();

 public:
  const std::string m_app_name_;
  const std::string m_version_;
};

}  // namespace xuzy::Core