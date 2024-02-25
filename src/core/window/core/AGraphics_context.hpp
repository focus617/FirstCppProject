#pragma once

#include "window/core/window_props.hpp"

namespace xuzy::Window {

/**
 * @brief Represents the backend context
 */
class AGraphicsContext {
 public:
  /**
   * @brief Destroy the context
   */
  virtual ~AGraphicsContext() = default;

  /**
   * @brief Initialize the context
   */
  virtual void init() = 0;

  /**
   * @brief Dump backend vendor and version
   */
  virtual void vendor_info() = 0;

  // Instance by concrete subclass
  static Scope<AGraphicsContext> Create(const WindowProps& p_props);
};

}  // namespace xuzy::Window