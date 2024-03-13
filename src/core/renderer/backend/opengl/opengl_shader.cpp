#include "renderer/backend/opengl/opengl_shader.hpp"

#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::Renderer {

namespace Utils {

static GLenum ShaderTypeFromString(const std::string& type) {
  if (type == "vertex") return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

  LOG(ERROR) << "Unknown shader type!";
  return 0;
}

static const char* GetCacheDirectory() {
  // TODO: make sure the assets directory is valid
  return "../assets/shaders/opengl";
}

static void CreateCacheDirectoryIfNeeded() {
  std::string cacheDirectory = GetCacheDirectory();
  if (!std::filesystem::exists(cacheDirectory))
    std::filesystem::create_directories(cacheDirectory);
}

}  // namespace Utils

OpenGLShader::OpenGLShader(const std::string& p_filepath)
    : m_filepath_(p_filepath) {
  Utils::CreateCacheDirectoryIfNeeded();

  std::string source = read_file(p_filepath);
  if (pre_process(source)) {
    compile();
  } else {
    LOG(ERROR) << "Wrong shader file loaded!";
  }

  // Extract name from filepath
  auto lastSlash = p_filepath.find_last_of("/\\");
  lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  auto lastDot = p_filepath.rfind('.');
  auto count = lastDot == std::string::npos ? p_filepath.size() - lastSlash
                                            : lastDot - lastSlash;
  m_name_ = p_filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& p_name,
                           const std::string& p_vertex_src,
                           const std::string& p_fragment_src)
    : m_name_(p_name) {
  m_shader_source_code_[GL_VERTEX_SHADER] = p_vertex_src;
  m_shader_source_code_[GL_FRAGMENT_SHADER] = p_fragment_src;
  compile();
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_renderer_id_); }

std::string OpenGLShader::read_file(const std::string& p_filepath) {
  std::string result;

  // ifstream closes itself due to RAII
  std::ifstream in(p_filepath, std::ios::in | std::ios::binary);

  if (in) {
    in.seekg(0, std::ios::end);
    int size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
    } else {
      LOG(ERROR) << "Could not read from file '" << p_filepath << "'";
    }
  } else {
    LOG(ERROR) << "Could not open file '" << p_filepath << "'";
  }

  return result;
}

bool OpenGLShader::pre_process(const std::string& p_source) {
  bool result = false;

  const char* typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos =
      p_source.find(typeToken, 0);  // Start of shader type declaration line

  while (pos != std::string::npos) {
    size_t eol = p_source.find_first_of(
        "\r\n", pos);  // End of shader type declaration line
    XUZY_CHECK_(eol != std::string::npos) << "Shader syntax error";

    // Start of shader type name (after "#type " keyword)
    size_t begin = pos + typeTokenLength + 1;
    std::string type = p_source.substr(begin, eol - begin);
    XUZY_CHECK_(Utils::ShaderTypeFromString(type))
        << "Invalid shader type specified";

    // Start of shader code after shader type declaration line
    size_t nextLinePos = p_source.find_first_not_of("\r\n", eol);
    XUZY_CHECK_(nextLinePos != std::string::npos) << "Shader syntax error";

    // Start of next shader type declaration line
    pos = p_source.find(typeToken, nextLinePos);
    m_shader_source_code_[Utils::ShaderTypeFromString(type)] =
        (pos == std::string::npos)
            ? p_source.substr(nextLinePos)
            : p_source.substr(nextLinePos, pos - nextLinePos);

    // Success parsing shader code
    result = true;
  }

  return result;
}

void OpenGLShader::compile() {
  XUZY_CHECK_(m_shader_source_code_.size() <= 2)
      << "We only support 2 shaders for now";

  // Get a program object.
  GLuint program = glCreateProgram();

  std::array<GLenum, 2> opengl_shader_ids;
  int opengl_shader_id_index = 0;

  for (auto& kv : m_shader_source_code_) {
    GLenum type = kv.first;
    const std::string& source = kv.second;

    // Create an empty shader handle
    GLuint shader = glCreateShader(type);

    // Send the shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, 0);

    // Compile the vertex shader
    glCompileShader(shader);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(shader);

      LOG(ERROR) << "Shader compilation failure!" << infoLog.data();
      break;
    }
    // Attach shader to program
    glAttachShader(program, shader);
    opengl_shader_ids[opengl_shader_id_index++] = shader;
  }
  m_renderer_id_ = program;

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(program);
    // Don't leak shaders either.
    for (auto id : opengl_shader_ids) glDeleteShader(id);

    LOG(ERROR) << "Shader link failure!" << infoLog.data();
    return;
  }

  // Always detach shaders after a successful link.
  for (auto id : opengl_shader_ids) {
    glDetachShader(program, id);
    glDeleteShader(id);
  }
}

// void OpenGLShader::CompileOrGetVulkanBinaries(
//     const std::unordered_map<GLenum, std::string>& p_shader_sources) {}

// void OpenGLShader::CompileOrGetOpenGLBinaries() {}

// void OpenGLShader::create_program() {
//   GLuint program = glCreateProgram();

//   std::vector<GLuint> shaderIDs;
//   for (auto&& [stage, spirv] : m_OpenGLSPIRV) {
//     GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
//     glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V,
//     spirv.data(),
//                    spirv.size() * sizeof(uint32_t));
//     glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
//     glAttachShader(program, shaderID);
//   }

//   glLinkProgram(program);

//   GLint is_linked;
//   glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
//   if (is_linked == GL_FALSE) {
//     GLint max_length;
//     glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

//     std::vector<GLchar> infoLog(max_length);
//     glGetProgramInfoLog(program, max_length, &max_length, infoLog.data());
//     LOG(ERROR) << "Shader linking failed (" << m_filepath_ << "):\n"
//                << infoLog.data();

//     glDeleteProgram(program);

//     for (auto id : shaderIDs) glDeleteShader(id);
//   }

//   for (auto id : shaderIDs) {
//     glDetachShader(program, id);
//     glDeleteShader(id);
//   }

//   m_renderer_id_ = program;
// }

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

void OpenGLShader::set_fvec2(const std::string& p_name,
                            const Maths::FVector2& p_fvec2) {
  set_uniform_fvec2(p_name, p_fvec2);
}

void OpenGLShader::set_fvec3(const std::string& p_name,
                            const Maths::FVector3& p_fvec3) {
  set_uniform_fvec3(p_name, p_fvec3);
}

void OpenGLShader::set_fvec4(const std::string& p_name,
                            const Maths::FVector4& p_fvec4) {
  set_uniform_fvec4(p_name, p_fvec4);
}

void OpenGLShader::set_mat4(const std::string& p_name,
                            const glm::mat4& p_mat4) {
  set_uniform_mat4(p_name, p_mat4);
}

int OpenGLShader::get_int(const std::string& p_name) {
  return get_uniform_int(p_name);
}

float OpenGLShader::get_float(const std::string& p_name) {
  return get_uniform_float(p_name);
}

Maths::FVector2 OpenGLShader::get_fvec2(const std::string& p_name) {
  return get_uniform_fvec2(p_name);
}

Maths::FVector3 OpenGLShader::get_fvec3(const std::string& p_name) {
  return get_uniform_fvec3(p_name);
}

Maths::FVector4 OpenGLShader::get_fvec4(const std::string& p_name) {
  return get_uniform_fvec4(p_name);
}

glm::mat4 OpenGLShader::get_mat4(const std::string& p_name) {
  return get_uniform_mat4(p_name);
}

uint32_t OpenGLShader::get_uniform_location(const std::string& p_name) {
  if (m_uniform_location_cache_.find(p_name) != m_uniform_location_cache_.end())
    return m_uniform_location_cache_.at(p_name);

  const int location = glGetUniformLocation(m_renderer_id_, p_name.c_str());

  if (location == -1)
    LOG(WARNING) << "Uniform: '" << p_name << "' doesn't exist";

  m_uniform_location_cache_[p_name] = location;

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

void OpenGLShader::set_uniform_fvec2(const std::string& p_name,
                                    const Maths::FVector2& p_fvec2) {
  glUniform2f(get_uniform_location(p_name), p_fvec2.x, p_fvec2.y);
}

void OpenGLShader::set_uniform_fvec3(const std::string& p_name,
                                    const Maths::FVector3& p_fvec3) {
  glUniform3f(get_uniform_location(p_name), p_fvec3.x, p_fvec3.y, p_fvec3.z);
}

void OpenGLShader::set_uniform_fvec4(const std::string& p_name,
                                    const Maths::FVector4& p_fvec4) {
  glUniform4f(get_uniform_location(p_name), p_fvec4.x, p_fvec4.y, p_fvec4.z,
              p_fvec4.w);
}

void OpenGLShader::set_uniform_mat4(const std::string& p_name,
                                    const glm::mat4& p_mat4) {
  glUniformMatrix4fv(get_uniform_location(p_name), 1, GL_FALSE,
                     glm::value_ptr(p_mat4));
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

Maths::FVector2 OpenGLShader::get_uniform_fvec2(const std::string& p_name) {
  GLfloat values[2];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector2&>(values);
}

Maths::FVector3 OpenGLShader::get_uniform_fvec3(const std::string& p_name) {
  GLfloat values[3];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector3&>(values);
}

Maths::FVector4 OpenGLShader::get_uniform_fvec4(const std::string& p_name) {
  GLfloat values[4];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<Maths::FVector4&>(values);
}

glm::mat4 OpenGLShader::get_uniform_mat4(const std::string& p_name) {
  GLfloat values[16];
  glGetUniformfv(m_renderer_id_, get_uniform_location(p_name), values);
  return reinterpret_cast<glm::mat4&>(values);
}

}  // namespace xuzy::Renderer
