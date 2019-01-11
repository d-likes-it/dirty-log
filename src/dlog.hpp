#ifndef __DLOG_DLOG__
#define __DLOG_DLOG__

#include "copy_args.hpp"
#include "format_signature.hpp"
#include "len_prefixed_buffer.hpp"

namespace dlog {

// the format string itself must be a compile time defined char array
// (constexpr)
struct _write_log {

  template <typename Logger, typename... Args>
  static void emit(Logger &writer, Args... args) {
    using args_traits = array_traits<Args...>;
    using format_sig = dlog::format_signature<Args...>;

    static_assert(format_sig::size + args_traits::size <
                      Logger::traits::alignment,
                  "log line too long");
    static_assert(format_sig::size == args_traits::count + 1,
                  "length mismatch");

    char *buffer = writer.prepare();
    {
      char *pointer = buffer;
      // constexpr auto descriptor = format_sig(format);

      std::memcpy(pointer, &format_sig::value[0], format_sig::size);
      pointer += format_sig::size;

      char *end = copy_args(pointer, args...);
#ifdef DLOG_DEBUG
      std::fill(end, buffer + Logger::traits::alignment, '\0');
#else
      *end = 0;
#endif
    }

    writer.emit();
  }
};
} // namespace dlog

#define write_log(l, f, ...) dlog::_write_log().emit(l, __VA_ARGS__)

#endif
