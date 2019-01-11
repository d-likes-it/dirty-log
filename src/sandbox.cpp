#define DLOG_DEBUG

#include <array>
#include <iostream>
#include <utility>

// #include "compiletime_string.hpp"
#include "format_signature.hpp"

template <class... Args> void func(Args... args) {
  constexpr auto signature = dlog::format_signature<Args...>::value;
  std::cout << "I was here: " << signature.size() << std::endl;
  static_assert(signature.size() == 3, "size does not match");
}

int main() {
  func(1, 2);
  // std::cout << hello2_t::c_str() << std::endl;
}
