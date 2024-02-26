#pragma once

#include "renderer/resources/shader/AShader.hpp"

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
  virtual void set_vec2(const std::string& p_name,
                        const Maths::FVector2& p_vec2) override;

  /**
   * @brief Send a vec3 to the GPU
   * @param p_name
   * @param p_vec3
   */
  virtual void set_vec3(const std::string& p_name,
                        const Maths::FVector3& p_vec3) override;

  /**
   * @brief Send a vec4 to the GPU
   * @param p_name
   * @param p_vec4
   */
  virtual void set_vec4(const std::string& p_name,
                        const Maths::FVector4& p_vec4) override;

  /**
   * @brief Send a mat4 to the GPU
   * @param p_name
   * @param p_mat4
   */
  virtual void set_mat4(const std::string& p_name,
                        const Maths::FMatrix4& p_mat4) override;

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
   * @brief Send a int array to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_int_arry(const std::string& p_name, int* p_values,
                            uint32_t count) override;

  /**
   * @brief Returns the float uniform value identified by the given name
   * @param p_name
   */
  virtual float get_float(const std::string& p_name) override;

  /**
   * @brief Returns the vec2 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector2 get_vec2(const std::string& p_name) override;

  /**
   * @brief Returns the vec3 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector3 get_vec3(const std::string& p_name) override;

  /**
   * @brief Returns the vec4 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector4 get_vec4(const std::string& p_name) override;

  /**
   * @brief Returns the mat4 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FMatrix4 get_mat4(const std::string& p_name) override;

  /**
   * @brief Returns information about the uniform identified by the
   * given name or nullptr if not found
   * @param p_name
   */
  // virtual const UniformInfo* get_info(const std::string& p_name)
  // const override;

  /**
   * @brief Query the uniforms from the program and store them in the
   * uniform vector
   */
  // virtual void query_uniforms() override;

 private:
  uint32_t get_uniform_location(const std::string& p_name);

  void set_uniform_int(const std::string& p_name, int p_value);
  void set_uniform_int_array(const std::string& p_name, int* p_values,
                             uint32_t count);
  void set_uniform_float(const std::string& p_name, float p_value);
  void set_uniform_vec2(const std::string& p_name,
                        const Maths::FVector2& p_vec2);
  void set_uniform_vec3(const std::string& p_name,
                        const Maths::FVector3& p_vec3);
  void set_uniform_vec4(const std::string& p_name,
                        const Maths::FVector4& p_vec4);
  void set_uniform_mat4(const std::string& p_name,
                        const Maths::FMatrix4& p_mat4);
  int get_uniform_int(const std::string& p_name);
  float get_uniform_float(const std::string& p_name);
  Maths::FVector2 get_uniform_vec2(const std::string& p_name);
  Maths::FVector3 get_uniform_vec3(const std::string& p_name);
  Maths::FVector4 get_uniform_vec4(const std::string& p_name);
  Maths::FMatrix4 get_uniform_mat4(const std::string& p_name);

  // const UniformInfo* get_uniform_info(const std::string& p_name) const;

  // static bool IsEngineUBOMember(const std::string& p_uniformName);

 private:
  uint32_t m_renderer_id_;
  std::string m_filepath_;
  std::string m_name_;

  //   std::vector<UniformInfo> uniforms;
  std::unordered_map<std::string, int> m_uniform_location_cache;
};

}  // namespace xuzy::Renderer