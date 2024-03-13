#pragma once

#include "renderer/resources/shader/AShader.hpp"

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace xuzy::Renderer {
/**
 * @brief OpenGL shader program wrapper
 */
class OpenGLShader : public AShader {
 public:
  OpenGLShader(const std::string& p_filepath);

  OpenGLShader(const std::string& p_name, const std::string& p_vertex_src,
               const std::string& p_fragment_src);

  virtual ~OpenGLShader();

  /**
   * @brief Bind the program
   */
  virtual void bind() const override;

  /**
   * @brief Unbind the program
   */
  virtual void un_bind() const override;

  /**
   * @brief Send a int to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_int(const std::string& p_name, int p_value) override;

  /**
   * @brief Send a int array to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_int_arry(const std::string& p_name, int* p_values,
                            uint32_t count) override;

  /**
   * @brief Send a float to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_float(const std::string& p_name, float p_value) override;

  /**
   * @brief Send a vec2 to the GPU
   * @param p_name
   * @param p_vec2
   */
  virtual void set_fvec2(const std::string& p_name,
                        const Maths::FVector2& p_fvec2) override;

  /**
   * @brief Send a vec3 to the GPU
   * @param p_name
   * @param p_vec3
   */
  virtual void set_fvec3(const std::string& p_name,
                        const Maths::FVector3& p_fvec3) override;

  /**
   * @brief Send a vec4 to the GPU
   * @param p_name
   * @param p_vec4
   */
  virtual void set_fvec4(const std::string& p_name,
                        const Maths::FVector4& p_fvec4) override;

  /**
   * @brief Send a mat4 to the GPU
   * @param p_name
   * @param p_mat4
   */
  virtual void set_mat4(const std::string& p_name,
                        const glm::mat4& p_mat4) override;

  /**
   * @brief Query the shader name
   */
  virtual const std::string& get_name() const override { return m_name_; }

  /**
   * @brief Returns the int uniform value identified by the given name
   * @param p_name
   */
  virtual int get_int(const std::string& p_name) override;

  /**
   * @brief Returns the float uniform value identified by the given name
   * @param p_name
   */
  virtual float get_float(const std::string& p_name) override;

  /**
   * @brief Returns the vec2 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector2 get_fvec2(const std::string& p_name) override;

  /**
   * @brief Returns the vec3 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector3 get_fvec3(const std::string& p_name) override;

  /**
   * @brief Returns the vec4 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector4 get_fvec4(const std::string& p_name) override;

  /**
   * @brief Returns the mat4 uniform value identified by the given name
   * @param p_name
   */
  virtual glm::mat4 get_mat4(const std::string& p_name) override;

 private:
  uint32_t get_uniform_location(const std::string& p_name);

  void set_uniform_int(const std::string& p_name, int p_value);
  void set_uniform_int_array(const std::string& p_name, int* p_values,
                             uint32_t count);
  void set_uniform_float(const std::string& p_name, float p_value);
  void set_uniform_fvec2(const std::string& p_name,
                        const Maths::FVector2& p_fvec2);
  void set_uniform_fvec3(const std::string& p_name,
                        const Maths::FVector3& p_fvec3);
  void set_uniform_fvec4(const std::string& p_name,
                        const Maths::FVector4& p_fvec4);
  void set_uniform_mat4(const std::string& p_name, const glm::mat4& p_mat4);

  int get_uniform_int(const std::string& p_name);
  float get_uniform_float(const std::string& p_name);
  Maths::FVector2 get_uniform_fvec2(const std::string& p_name);
  Maths::FVector3 get_uniform_fvec3(const std::string& p_name);
  Maths::FVector4 get_uniform_fvec4(const std::string& p_name);
  glm::mat4 get_uniform_mat4(const std::string& p_name);

  std::string read_file(const std::string& p_filepath);
  bool pre_process(const std::string& p_source);
  void compile();

  // void CompileOrGetVulkanBinaries(
  //     const std::unordered_map<GLenum, std::string>& p_shader_sources);
  // void CompileOrGetOpenGLBinaries();
  // void create_program();

 private:
  uint32_t m_renderer_id_;
  std::string m_filepath_;
  std::string m_name_;

  std::unordered_map<std::string, int> m_uniform_location_cache_;

  std::unordered_map<GLenum, std::string> m_shader_source_code_;

  // std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
  // std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;
};

}  // namespace xuzy::Renderer