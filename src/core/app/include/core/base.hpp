#pragma once

#include <glog/logging.h>

#include "visibility_control.hpp"

namespace xuzy {

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

namespace internal {

// A helper for suppressing warnings on constant condition.  It just
// returns 'condition'.
XUZY_API bool IsTrue(bool condition);

}  // namespace internal

}  // namespace xuzy

#define BIT(x) (1 << x)

// Defines some utility macros.

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.  This leads to problems with code like:
//
//   if (gate)
//     ASSERT_*(condition) << "Some message";
//
// The "switch (0) case 0:" idiom is used to suppress this.
#ifdef __INTEL_COMPILER
#  define XUZY_AMBIGUOUS_ELSE_BLOCKER_
#else
#  define XUZY_AMBIGUOUS_ELSE_BLOCKER_ \
    switch (0)                         \
    case 0:                            \
    default:  // NOLINT
#endif

#if !defined(XUZY_CHECK_)
/**
 * @brief XUZY_CHECK_ is an all-mode assert. It aborts the program if the
 * condition is not satisfied.
 *
 *  Synopsis:
 *    XUZY_CHECK_(boolean_condition);
 *     or
 *    XUZY_CHECK_(boolean_condition) << "Additional message";
 *
 *    This checks the condition and if the condition is not satisfied
 *    it prints message about the condition violation, including the
 *    condition itself, plus additional message streamed into it, if any,
 *    and then it aborts the program. It aborts the program irrespective of
 *    whether it is built in the debug mode or not.
 */
#  ifdef NDEBUG
#    define XUZY_CHECK_(condition)           \
      XUZY_AMBIGUOUS_ELSE_BLOCKER_           \
      if (xuzy::internal::IsTrue(condition)) \
        ;                                    \
      else                                   \
        LOG(FATAL) << "Condition " #condition " failed. "
#  else
#    define XUZY_CHECK_(condition)           \
      XUZY_AMBIGUOUS_ELSE_BLOCKER_           \
      if (xuzy::internal::IsTrue(condition)) \
        ;                                    \
      else                                   \
        LOG(WARNING) << "Condition " #condition " failed. "
#  endif  // ifdef NDEBUG
#endif    // !defined(XUZY_CHECK_)