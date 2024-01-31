#include "pch.h"

#include "app/app_impl.hpp"

#include <fstream>

#include "helper/stream_formatter.hpp"

namespace xuzy {
namespace internal {

/**
 * @brief Static method for accessing AppImpl singleton instance.
 */
AppImpl* AppImpl::GetInstance() {
  // CodeGear C++Builder insists on a public destructor for the
  // default implementation.  Use this implementation to keep good OO
  // design with private destructor.
#if defined(__BORLANDC__)
  static AppImpl* instance = new AppImpl();  //  lazy inits once
  return *instance;
#else
  static AppImpl instance;  // thread safe solution for init once
  return &instance;
#endif  // defined(__BORLANDC__)
}

void AppImpl::init(int argc, char* argv[]) {
  argvs_.clear();
  for (int i = 0; i != argc; i++) {
    argvs_.push_back(StreamableToString(argv[i]));
  }
}

void AppImpl::load_conf_from_file(const std::string& filename, json& conf) {
  // std::ifstream 支持 RAII
  // C++保证在对象超出范围时调用析构函数，而不管控制如何离开该范围。
  // 即使抛出异常，所有本地对象也会超出范围，因此它们的相关资源将被清除。
  std::ifstream config_file(filename.c_str());

  // Make sure the config is open
  if (!config_file.is_open()) {
    throw xuzy::OpenFileException("Failed to open " + filename);
  }
  // Make sure the config is not empty
  if (!config_file.good() || config_file.std::ios::eof()) {
    throw xuzy::FileNotReadyException(filename + " is empty");
  }

  config_file >> conf;
  config_file.close();
}

}  // namespace internal
}  // namespace xuzy