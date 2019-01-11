#ifndef __DLOG_LEN_PREFIXED_BUFFER__
#define __DLOG_LEN_PREFIXED_BUFFER__

#include <array>

namespace dlog {

template <uint32_t N> struct len_prefixed_buffer {
  uint8_t value[N+1] = "";
  uint32_t size = N+1;

  static constexpr uint32_t max_size = 128;

  constexpr len_prefixed_buffer(const char (&lit)[N]) {
    static_assert(N < max_size, "format too long");
    value[0] = static_cast<uint8_t>(N);
    for (int i = 0; i < N; ++i) {
      value[i + 1] = lit[i];
    }
  }

  constexpr len_prefixed_buffer(const std::array<char, N> & lit) {
    static_assert(N < max_size, "format too long");
    value[0] = static_cast<uint8_t>(N);
    for (int i = 0; i < N; ++i) {
      value[i + 1] = lit[i];
    }
  }
};

} // namespace dlog

#endif
