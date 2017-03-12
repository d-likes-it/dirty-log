#include <iostream>
#include <cstring>

#include "signature.hpp"

namespace dlog {

  template <typename Arg>
  char * copyArg(char * buffer, Arg & t) {
    static_assert(std::is_trivially_copyable<Arg>::value, "trivially copyable types only");
    *reinterpret_cast<Arg*>(buffer) = t;
    return buffer + sizeof(Arg);
  }

  char * copyArgs(char * buffer) {
    return buffer;
  }

  template <typename Arg, typename... Args>
  char * copyArgs(char * buffer, Arg& arg, Args& ... args) {
    buffer = copyArg(buffer, arg);
    return copyArgs(buffer, args...);
  }

  template <typename... Args>
  void writeLog(char const * formatString, Args... args) {
    typedef signature<Args...> sig;
    
    auto const bufSize = sig::sizeOfSignatureString + sig::sizeOfArgs;
    static_assert(bufSize < 256, "log line too long");

    char buffer[bufSize];
    std::memcpy(buffer, sig::signatureString, sig::sizeOfSignatureString);
  
    char * end = copyArgs(buffer + sig::sizeOfSignatureString, args...);
    *end = 0;
    std::cout << buffer << std::endl;
  }

}
  
int main() {
  dlog::writeLog("Hello", 1.0, 2, 3);
}

