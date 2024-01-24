#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "argsparser.hpp"
#include "visibility_control.hpp"

using json = nlohmann::json;

namespace xuzy {

/**
 *  @brief App脚手架
 */
class XUZY_API App {
 public:
  App(std::string t_app_name);
  virtual ~App();

  /**
   * @brief Set command line parser
   * @param
   */
  XUZY_API void set_cli_parser(ArgsParser* p_parser);

  /**
   * @brief Main program entrance
   * @param
   */
  static void main(int argc, char* argv[], const std::string& version,
                   App* app);

 protected:
  /**
   *  @brief Configuration in json format
   */
  json m_conf_;

  XUZY_API virtual void version_check(int argc, char* argv[],
                                      const std::string& version);
  XUZY_API virtual void load_configuration_from_file(
      const std::string& filename);
  XUZY_API virtual void setup();
  XUZY_API virtual void run() = 0;

  XUZY_API virtual void dumpError(std::string error);

 private:
  std::string m_app_name_;
  ArgsParser* p_cli_parser_;

  static void init_logger(const char* app);
};

}  // namespace xuzy