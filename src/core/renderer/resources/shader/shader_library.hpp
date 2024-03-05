#pragma once

#include "renderer/resources/shader/AShader.hpp"

namespace xuzy::Renderer {

class XUZY_API ShaderLibrary {
 public:
  void add(const std::string& p_name, const Ref<AShader>& p_shader);
  void add(const Ref<AShader>& p_shader);
  Ref<AShader> load(const std::string& p_filepath);
  Ref<AShader> load(const std::string& p_name, const std::string& p_filepath);

  Ref<AShader> get(const std::string& p_name);

  bool exists(const std::string& p_name) const;

 private:
  std::unordered_map<std::string, Ref<AShader>> m_shaders_;
};

}  // namespace xuzy::Renderer