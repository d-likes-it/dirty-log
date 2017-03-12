#include <iostream>

#include "loggabletypes.hpp"

constexpr unsigned MAX_ARGS = 8;

template<typename T>
struct alignas(64) LogLineEntry {
  uint8_t typeId;
  T value;
  LogLineEntry(T v) : typeId(TypeID<T, SupportedTypes>::value), value(v) {};
  
  LogLineEntry(LogLineEntry &&) = default;
  LogLineEntry & operator=(LogLineEntry &&) = default;
};

template <typename Arg>
char * copyArg(char * buffer, Arg & t) {
  static_assert(std::is_trivially_copyable<Arg>::value, "trivially copyable types only");
  *reinterpret_cast<LogLineEntry<Arg>*>(buffer) = LogLineEntry<Arg>(t);
  return buffer + sizeof(LogLineEntry<Arg>);
}

char * copyArgs(char * buffer) {
  return buffer;
}

template <typename Arg, typename... Args>
char * copyArgs(char * buffer, Arg& arg, Args& ... args) {
	buffer = copyArg(buffer, arg);
    return copyArgs(buffer, args...);
}

template <typename... Types>
constexpr unsigned sizeofArgs(Types &&...) {
  return sizeof...(Types);
}

constexpr size_t constStrLength(char const * str){
  return (*str == 0) ? 0 : constLength(str + 1) + 1;
}


template <typename... Args>
void writeLog(char const * formatString, Args... args) {
  static_assert(sizeofArgs(args...) < 9, "too many args");
  static_assert(sizeof(char const *) + sizeofArgs(args...) * 16 < 256, "log line too long");

  char buffer[MAX_ARGS + sizeof_args(args...) * 16];

  auto len = signature<args>::value::size;
  std::memcpy(buffer, signature<args>::value::str, len);
  buffer += len;
  
  char * end = copyArgs(buffer, args...);
  *end = 0;
  std::cout << buffer << std::endl;
}
  
int main() {
  writeLog("Hello", 1.0, 2, 3);
}

