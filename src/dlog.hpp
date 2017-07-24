#ifndef __DLOG_DLOG__
#define __DLOG_DLOG__

#include "signature.hpp"
#include "copy_args.hpp"

namespace dlog {

  // the format string itself must be a compile time defined char array (constexpr)
  template <typename Logger, uint8_t format_length, typename... Args>
  void write_log(Logger & writer, char const (&format)[format_length], Args... args) {
    using sig_traits = signature_traits<Args...>;
    using sig_str = signature_string<Args...>;

    static_assert(format_length > 0, "empty format string");
    static_assert(((format_length - 1)
                   + sig_str::size
                   + sig_traits::size
                   + 3) < Logger::traits::alignment, "log line too long");

    char * buffer = writer.prepare();
    {
      char * pointer = buffer;
      
      *reinterpret_cast<uint8_t *>(pointer) = format_length - 1;
      pointer += sizeof(uint8_t);
      std::memcpy(pointer, format, format_length - 1);
      pointer += format_length - 1;
      
      std::memcpy(pointer, sig_str::value, sig_str::size);
      pointer += sig_str::size;
      
      char * end = copy_args(pointer, args...);
#ifdef DLOG_DEBUG
      std::fill(end, buffer + Logger::traits::alignment, '\0');
#else
      *end = 0;
#endif
    }

    writer.emit();
  }

}

#endif


