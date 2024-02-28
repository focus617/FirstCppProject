#pragma once

#include <unordered_map>

#include "tools/maths/FMatrix4.hpp"
#include "tools/maths/FVector2.hpp"
#include "tools/maths/FVector3.hpp"
#include "tools/maths/FVector4.hpp"

// #include "renderer/resources/info.hpp"

namespace xuzy::Renderer {
// namespace Loaders { class ShaderLoader; }

/**
 * @brief Generic shader program wrapper
 */
class XUZY_API AShader {
  // friend class Loaders::ShaderLoader;

 public:
  /**
   * @brief Concrete shader creater
   * @param p_filepath
   */
  static Ref<AShader> Create(const std::string& p_filepath);

  /**
   * @brief Concrete shader creater
   * @param p_name
   * @param p_vertex_src    Vertex source code
   * @param p_fragment_src  Fragment source code
   */
  static Ref<AShader> Create(const std::string& p_name,
                             const std::string& p_vertex_src,
                             const std::string& p_fragment_src);

  /**
   * @brief Concrete shader creater
   * @param p_vertex_src    Vertex source code
   * @param p_fragment_src  Fragment source code
   */
  static Ref<AShader> Create(const std::string& p_vertex_src,
                             const std::string& p_fragment_src);

  /**
   * @brief Default destructer
   */
  virtual ~AShader() = default;

  /**
   * @brief Bind the program
   */
  virtual void bind() const = 0;

  /**
   * @brief Unbind the program
   */
  virtual void un_bind() const = 0;

  /**
   * @brief Send a int to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_int(const std::string& p_name, int p_value) = 0;

  /**
   * @brief Send a int array to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_int_arry(const std::string& p_name, int* p_values,
                            uint32_t count) = 0;

  /**
   * @brief Send a float to the GPU
   * @param p_name
   * @param p_value
   */
  virtual void set_float(const std::string& p_name, float p_value) = 0;

  /**
   * @brief Send a vec2 to the GPU
   * @param p_name
   * @param p_vec2
   */
  virtual void set_vec2(const std::string& p_name,
                        const Maths::FVector2& p_vec2) = 0;

  /**
   * @brief Send a vec3 to the GPU
   * @param p_name
   * @param p_vec3
   */
  virtual void set_vec3(const std::string& p_name,
                        const Maths::FVector3& p_vec3) = 0;

  /**
   * @brief Send a vec4 to the GPU
   * @param p_name
   * @param p_vec4
   */
  virtual void set_vec4(const std::string& p_name,
                        const Maths::FVector4& p_vec4) = 0;

  /**
   * @brief Send a mat4 to the GPU
   * @param p_name
   * @param p_mat4
   */
  virtual void set_mat4(const std::string& p_name,
                        const Maths::FMatrix4& p_mat4) = 0;

  /**
   * @brief Query the shader name
   */
  virtual const std::string& get_name() const = 0;

  /**
   * @brief Returns the int uniform value identified by the given name
   * @param p_name
   */
  virtual int get_int(const std::string& p_name) = 0;

  /**
   * @brief Returns the float uniform value identified by the given name
   * @param p_name
   */
  virtual float get_float(const std::string& p_name) = 0;

  /**
   * @brief Returns the vec2 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector2 get_vec2(const std::string& p_name) = 0;

  /**
   * @brief Returns the vec3 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector3 get_vec3(const std::string& p_name) = 0;

  /**
   * @brief Returns the vec4 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FVector4 get_vec4(const std::string& p_name) = 0;

  /**
   * @brief Returns the mat4 uniform value identified by the given name
   * @param p_name
   */
  virtual Maths::FMatrix4 get_mat4(const std::string& p_name) = 0;

  /**
   * @brief Returns information about the uniform identified by the
   * given name or nullptr if not found
   * @param p_name
   */
  // virtual const UniformInfo* get_info(const std::string& p_name)
  // const = 0;

  /**
   * @brief Query the uniforms from the program and store them in the
   * uniform vector
   */
  // virtual void query_uniforms() = 0;
};

}  // namespace xuzy::Renderer