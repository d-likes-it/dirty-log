#include <iostream>

template < typename T, typename Tuple >
struct TupleIndex;

template < typename T,typename ... Types >
struct TupleIndex < T, std::tuple < T, Types... > > 
{
  static constexpr const std::size_t value = 0;
};

template < typename T, typename U, typename ... Types >
struct TupleIndex < T, std::tuple < U, Types... > > 
{
  static constexpr const std::size_t value = 1 + TupleIndex < T, std::tuple < Types... > >::value;
};

struct string_literal_t
{
  explicit string_literal_t(char const * s) : m_s(s) {}
  char const * m_s;
};

typedef std::tuple < char, uint32_t, uint64_t, int32_t, int64_t, double, string_literal_t, char * > SupportedTypes;

template<typename T>
struct alignas(64) LogLineEntry {
  uint8_t typeId;
  T value;
  LogLineEntry(T v) : typeId(TupleIndex<T, SupportedTypes>::value), value(v) {};
  
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
constexpr unsigned sizeof_args(Types &&...) {
  return sizeof...(Types);
}

template <typename... Args>
void writeLog(char const * formatString, Args... args) {
  static_assert(sizeof(char const *) + sizeof_args(args...) * 16 < 256, "log line too long");
  char buffer[sizeof_args(args...) * 16];
  char * end = copyArgs(buffer, args...);
  *end = 0;
  std::cout << buffer << std::endl;
}
  
int main() {
  writeLog("Hello", 1.0, 2, 3);
}

