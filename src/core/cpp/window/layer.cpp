#include "pch.h"

#include "window/layer.hpp"

namespace xuzy {

Layer::Layer(const std::string& debugName) : m_debug_name_(debugName) {}

Layer::~Layer() {}

}  // namespace xuzy