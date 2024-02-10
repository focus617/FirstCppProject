#include "pch.h"

#include "layer.hpp"

namespace xuzy::Window {

Layer::Layer(const std::string& debugName) : m_debug_name_(debugName) {}

Layer::~Layer() {}

}  // namespace xuzy::Window