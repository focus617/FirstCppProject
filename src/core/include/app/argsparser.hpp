#pragma once

#include <string>

namespace xuzy {

class ArgsParser {
 private:
 public:
  ArgsParser() = default;
  virtual ~ArgsParser() = default;

  virtual bool parse_commandline(int argc, char* argv[]) = 0;

  static bool GetEnvVar(const char* var_name, std::string& var_value);
};

}  // namespace xuzy