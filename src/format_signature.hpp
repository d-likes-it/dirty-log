#ifndef __DLOG_FORMAT_SIGNATURE__
#define __DLOG_FORMAT_SIGNATURE__

#include <array>

#include "build_options.hpp"

#include "traits.hpp"
#include "type_id.hpp"

namespace dlog {

namespace detail {

#if CPP_VERSION >= 17

template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, N1 + N2> concatenate(const std::array<T, N1> &lhs,
                                             const std::array<T, N2> &rhs) {
  std::array<T, N1 + N2> result{};
  std::size_t index{0};
  for (std::size_t i{0}; i < lhs.size(); ++i, ++index)
    result[index] = lhs[i];
  for (std::size_t i{0}; i < rhs.size(); ++i, ++index)
    result[index] = rhs[i];
  return result;
}
#elif CPP_VERSION >= 14

  //  ...

#endif

} // namespace detail

template <class... Args> struct format_signature {
  static constexpr auto no_of_args = dlog::array_traits<Args...>::count;

  static constexpr std::array<char, no_of_args + 1> value = detail::concatenate(
      std::array<char, 1>{no_of_args},
      std::array<char, no_of_args>{dlog::type_id<Args>::value...});

  static constexpr std::size_t size = no_of_args + 1;

  static_assert(size < 256, "excess length");
};

} // namespace dlog

#endif
