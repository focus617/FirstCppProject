#include "renderer/backend/opengl/opengl_shader.hpp"

#include <glad/glad.h>

namespace xuzy::Renderer {

OpenGLShader::OpenGLShader(const std::string& p_filepath)
    : m_filepath_(p_filepath) {}

OpenGLShader::OpenGLShader(const std::string& p_name,
                           const std::string& p_vertex_src,
                           const std::string& p_fragment_src)
    : m_name_(p_name) {}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_renderer_id_); }

void OpenGLShader::bind() const { glUseProgram(m_renderer_id_); }

void OpenGLShader::un_bind() const { glUseProgram(0); }

void OpenGLShader::set_int(const std::string& p_name, int p_value) {
  set_uniform_int(p_name, p_value);
}

void OpenGLShader::set_int_arry(const std::string& p_name, int* p_values,
                                uint32_t count) {
  set_uniform_int_array(p_name, p_values, count);
}

void OpenGLShader::set_float(const std::string& p_name, float p_value) {
  set_uniform_float(p_name, p_value);
}

void OpenGLShader::set_vec2(const std::string& p_name,
                            const Maths::FVector2& p_vec2) {
  set_uniform_vec2(p_name, p_vec2);
}

void OpenGLShader::set_vec3(const std::string& p_name,
                            const Maths::FVector3& p_vec3) {
  set_uniform_vec3(p_name, p_vec3);
}

void OpenGLShader::set_vec4(const std::string& p_name,
                            const Maths::FVector4& p_vec4) {
  set_uniform_vec4(p_name, p_vec4);
}

void OpenGLShader::set_mat4(const std::string& p_name,
                            const Maths::FMatrix4& p_mat4) {
  set_uniform_mat4(p_name, p_mat4);
}

int OpenGLShader::get_int(const std::string& p_name) {
  return get_uniform_int(p_name);
}

float OpenGLShader::get_float(const std::string& p_name) {
  return get_uniform_float(p_name);
}

Maths::FVector2 OpenGLShader::get_vec2(const std::string& p_name) {
  return get_uniform_vec2(p_name);
}

Maths::FVector3 OpenGLShader::get_vec3(const std::string& p_name) {
  return get_uniform_vec3(p_name);
}

Maths::FVector4 OpenGLShader::get_vec4(const std::string& p_name) {
  return get_uniform_vec4(p_name);
}

Maths::FMatrix4 OpenGLShader::get_mat4(const std::string& p_name) {
  return get_uniform_mat4(p_name);
}

uint32_t OpenGLShader::get_uniform_location(const std::string& p_name) {
  if (m_uniform_location_cache.find(p_name) != m_uniform_location_cache.end())
    return m_uniform_location_cache.at(p_name);

  const int location = glGetUniformLocation(m_renderer_id_, p_name.c_str());

  if (location == -1)
    LOG(WARNING) << "Uniform: '" << p_name << "' doesn't exist";

  m_uniform_location_cache[p_name] = location;

  return location;
}

void OpenGLShader::set_uniform_int(const std::string& p_name, int p_value) {
  glUniform1i(get_uniform_location(p_name), p_value);
}

void OpenGLShader::set_uniform_int_array(const std::string& p_name,
                                         int* p_values, uint32_t count) {
  glUniform1iv(get_uniform_location(p_name), count, p_values);
}

void OpenGLShader::set_uniform_float(const std::string& p_name, float p_value) {
  glUniform1f(get_uniform_location(p_name), p_value);
}

void OpenGLShader::set_uniform_vec2(const std::string& p_name,
                                    const Maths::FVector2& p_vec2) {
  glUniform2f(get_uniform_location(p_name), p_vec2.x, p_vec2.y);
}

void OpenGLShader::set_uniform_vec3(const std::string& p_name,
                                    const Maths::FVector3& p_vec3) {
  glUniform3f(get_uniform_location(p_name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void OpenGLShader::set_uniform_vec4(const std::string& p_name,
                                    const Maths::FVector4& p_vec4) {
  glUniform4f(get_uniform_location(p_name), p_vec4.x, p_vec4.y, p_vec4.z,
              p_vec4.w);
}

void OpenGLShader::set_uniform_mat4(const std::string& p_name,
                                    const Maths::FMatrix4& p_mat4) {
  glUniformMatrix4fv(get_uniform_location(p_name), 1, GL_TRUE, &p_mat4.data[0]);
}

int OpenGLShader::get_uniform_int(const std::string& p_name) {
  int value;
  glGetUniformiv(m_renderer_id_, get_uniform_location(p_name), &value);
  return value;
}

float OpenGLShader::get_uniform_float(const std::string& p_name) {
  float value;
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), &value);
  return value;
}

Maths::FVector2 OpenGLShader::get_uniform_vec2(const std::string& p_name) {
  GLfloat values[2];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector2&>(values);
}

Maths::FVector3 OpenGLShader::get_uniform_vec3(const std::string& p_name) {
  GLfloat values[3];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector3&>(values);
}

Maths::FVector4 OpenGLShader::get_uniform_vec4(const std::string& p_name) {
  GLfloat values[4];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector4&>(values);
}

Maths::FMatrix4 OpenGLShader::get_uniform_mat4(const std::string& p_name) {
  GLfloat values[16];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FMatrix4&>(values);
}

// UniformInfo* OpenGLShader::get_uniform_info(const std::string& p_name){}

}  // namespace xuzy::Renderer
