#include "renderer/resources/shader/shader_library.hpp"

namespace xuzy::Renderer {

void ShaderLibrary::add(const std::string& p_name,
                        const Ref<AShader>& p_shader) {
  XUZY_CHECK_(!exists(p_name)) << "Shader already exists!";
  m_shaders_[p_name] = p_shader;
}

void ShaderLibrary::add(const Ref<AShader>& p_shader) {
  auto& p_name = p_shader->get_name();
  add(p_name, p_shader);
}

Ref<AShader> ShaderLibrary::load(const std::string& p_filepath) {
  auto shader = AShader::Create(p_filepath);
  add(shader);
  return shader;
}

Ref<AShader> ShaderLibrary::load(const std::string& p_name,
                                 const std::string& p_filepath) {
  auto shader = AShader::Create(p_filepath);
  add(p_name, shader);
  return shader;
}

Ref<AShader> ShaderLibrary::get(const std::string& p_name) {
  XUZY_CHECK_(exists(p_name)) << "Shader not found!";
  return m_shaders_[p_name];
}

bool ShaderLibrary::exists(const std::string& p_name) const {
  return m_shaders_.find(p_name) != m_shaders_.end();
}

}  // namespace xuzy::Renderer