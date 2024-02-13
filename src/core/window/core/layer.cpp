#include "pch.h"

#include "layer.hpp"

namespace xuzy::Window {

ALayer::ALayer(const std::string& debugName) : m_debug_name_(debugName) {}

ALayer::~ALayer() {}

}  // namespace xuzy::Window