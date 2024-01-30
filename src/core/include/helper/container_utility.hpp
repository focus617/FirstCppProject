#pragma once

#include <algorithm>

#include "resource/random.hpp"
#include "core/utilities.hpp"

namespace xuzy {

/**
 * @brief STL container utilities.
 */

/**
 * @brief Returns the number of elements in the given container that satisfy the
 * given predicate.
 */
template <class Container, typename Predicate>
inline int CountIf(const Container& c, Predicate predicate) {
  // Implemented as an explicit loop since std::count_if() in libCstd on
  // Solaris has a non-standard signature.
  int count = 0;
  for (auto it = c.begin(); it != c.end(); ++it) {
    if (predicate(*it)) ++count;
  }
  return count;
}

/**
 * @brief Applies a function/functor to each element in the container.
 */
template <class Container, typename Functor>
void ForEach(const Container& c, Functor functor) {
  std::for_each(c.begin(), c.end(), functor);
}

/**
 * @brief Returns the i-th element of the vector, or default_value if i is not
 * in range [0, v.size()).
 */
template <typename E>
inline E GetElementOr(const std::vector<E>& v, int i, E default_value) {
  return (i < 0 || i >= static_cast<int>(v.size())) ? default_value
                                                    : v[static_cast<size_t>(i)];
}

/**
 * @brief Performs an in-place shuffle of a range of the vector's elements.
 *
 * @param begin and @param end are element indices as an STL-style range;
 * i.e. [begin, end) are shuffled, where 'end' == size() means to shuffle to the
 * end of the vector.
 */
template <typename E>
void ShuffleRange(xuzy::Random* random, int begin, int end, std::vector<E>* v) {
  const int size = static_cast<int>(v->size());
  XUZY_CHECK_(0 <= begin && begin <= size)
      << "Invalid shuffle range start " << begin << ": must be in range [0, "
      << size << "].";
  XUZY_CHECK_(begin <= end && end <= size)
      << "Invalid shuffle range finish " << end << ": must be in range ["
      << begin << ", " << size << "].";

  // Fisher-Yates shuffle, from
  // https://en.wikipedia.org/wiki/Fisher-Yates_shuffle
  for (int range_width = end - begin; range_width >= 2; range_width--) {
    const int last_in_range = begin + range_width - 1;
    const int selected =
        begin +
        static_cast<int>(random->Generate(static_cast<uint32_t>(range_width)));
    std::swap((*v)[static_cast<size_t>(selected)],
              (*v)[static_cast<size_t>(last_in_range)]);
  }
}

/**
 * @brief Performs an in-place shuffle of the vector's elements.
 */
template <typename E>
inline void Shuffle(xuzy::Random* random, std::vector<E>* v) {
  ShuffleRange(random, 0, static_cast<int>(v->size()), v);
}

}  // namespace xuzy