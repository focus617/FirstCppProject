#include "utilities.hpp"

namespace xuzy {
namespace internal {

// A helper for suppressing warnings on constant condition.  It just
// returns 'condition'.
bool IsTrue(bool condition) { return condition; }

} // namespace internal
} // namespace xuzy