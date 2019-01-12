#include <cstring>

namespace dlog {

template <typename Arg> char *copy_arg(char *buffer, Arg &t) {
  static_assert(std::is_trivially_copyable<Arg>::value,
                "trivially copyable types only");
  *reinterpret_cast<Arg *>(buffer) = t;
  return buffer + sizeof(Arg);
}

char *copy_args(char *buffer) { return buffer; }

template <typename Arg, typename... Args>
char *copy_args(char *buffer, Arg &arg, Args &... args) {
  buffer = copy_arg(buffer, arg);
  return copy_args(buffer, args...);
}

} // namespace dlog
