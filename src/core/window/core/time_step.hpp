#pragma once

namespace xuzy::Renderer::Times {

class Timestep {
 public:
  Timestep(float time = 0.0f) : m_time_(time) {}

  operator float() const { return m_time_; }

  float get_seconds() const { return m_time_; }
  float get_milliseconds() const { return m_time_ * 1000.0f; }

 private:
  float m_time_;
};

}  // namespace xuzy::Renderer::Times