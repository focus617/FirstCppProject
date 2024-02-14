#include "app/context.hpp"

namespace xuzy::Core {

Context::Context(const std::string& p_app_name, const std::string& p_version)
    : m_app_name_{p_app_name}, m_version_{p_version} {}

/**
 * Destructor
 */
Context::~Context() {}

}  // namespace xuzy::Core