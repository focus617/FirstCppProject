#pragma once

#include <glog/logging.h>

#include <nlohmann/json.hpp>

#include "core/visibility_control.hpp"
#include "core/exception.hpp"

using json = nlohmann::json;

namespace xuzy {
namespace internal {

/**
 *  @brief App internal implementation
 */
class XUZY_LOCAL AppImpl {
 public:
  /**
   * @brief Static method for accessing AppImpl singleton instance.
   * The first time this method is called, a AppImpl object is constructed
   * and returned.  Consecutive calls will return the same object.
   * @return AppImpl instance.
   */
  static AppImpl* GetInstance();
  virtual ~AppImpl() = default;

  // We disallow copying AppImpl.
  AppImpl(const AppImpl&) = delete;
  AppImpl& operator=(const AppImpl&) = delete;

  /**
   * @brief init argvs_ via command line flags
   */
  void init(int argc, char* argv[]);

  /**
   * @brief returns true if and only if the user has initialized App.
   * Useful for catching the user mistake of not initializing before calling
   * RUN().
   */
  inline bool is_initialized() { return !argvs_.empty(); }

  void load_conf_from_file(const std::string& filename, json& conf);

 private:
  AppImpl() = default;

  // A copy of all command line arguments.
  std::vector<std::string> argvs_;
};

}  // namespace internal
}  // namespace xuzy