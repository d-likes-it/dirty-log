#ifndef __DLOG_TRAITS__
#define __DLOG_TRAITS__

#include "type_id.hpp"

namespace dlog {

namespace detail {
template <typename... Types> struct _sizeof;

template <typename TFirst> struct _sizeof<TFirst> {
  static constexpr auto value = (sizeof(TFirst));
};

template <typename TFirst, typename... TRemaining>
struct _sizeof<TFirst, TRemaining...> {
  static constexpr auto value =
      (sizeof(TFirst) + _sizeof<TRemaining...>::value);
};
} // namespace detail

template <typename... Args> struct array_traits {
  static constexpr std::size_t count = sizeof...(Args);
  static constexpr std::size_t size = detail::_sizeof<Args...>::value;
};

} // namespace dlog

#endif
