#pragma once

#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

#include "argsparser.hpp"
#include "core/exception.hpp"
#include "core/visibility_control.hpp"

using json = nlohmann::json;

namespace xuzy {

namespace internal {

class XUZY_LOCAL AppImpl;

}

/**
 *  @brief App scaffold
 */
class XUZY_API App {
 public:
  App(const std::string& t_app_name, const std::string& t_version);
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
  static void main(int argc, char* argv[], App* app);

 protected:
  /**
   *  @brief Configuration in json format
   */
  json m_conf_;

  XUZY_API virtual void init_logger();
  XUZY_API virtual void close_logger();

  XUZY_API virtual void version_check(int argc, char* argv[]);
  XUZY_API virtual void load_conf(const std::string& filename);
  XUZY_API virtual void setup();
  XUZY_API virtual void run() = 0;

  XUZY_API virtual void dumpError(std::string error);

  // Accessors for the implementation object.
  static internal::AppImpl* GetImpl() { return p_impl_; }

 private:
  std::string m_app_name_;
  std::string m_version_;
  ArgsParser* p_cli_parser_;

  // Protects mutable state in *p_impl_.
  // This is mutable as some const methods need to lock it too.
  static std::mutex m_mutex_;

  // Opaque implementation object.  This field is never changed once
  // the object is constructed.  We don't mark it as const here, as
  // doing so will cause a warning in the constructor of App.
  // Mutable state in *p_impl_ is protected by m_mutex_.
  static internal::AppImpl* p_impl_;


  // These classes and functions are friends as they need to access private
  // members of App.
};

// To be defined in CLIENT
XUZY_API App* CreateApplication();

}  // namespace xuzy