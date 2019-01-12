//#define DLOG_DEBUG

#include <chrono>
#include <cstring>
#include <iostream>

#include "dlog.hpp"
#include "log_writer.hpp"

using namespace dlog;
namespace sc = std::chrono;

template <typename Logger> void run(Logger &logger, uint64_t n) {
  for (uint64_t i = 0; i < n; ++i) {
    write_log(logger, "Hello World {} {}", i*1.0, n);
  }
}

template <typename Fun> long measure(Fun fun) {
  auto begin = sc::high_resolution_clock::now();

  fun();

  auto end = sc::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

  return duration;
}

int main() {
  // use 256 cache lines
  constexpr auto N = 256;
  constexpr auto LEN = 64;
  BenchmarkRollingLogWriter<N, LEN> logger;

  constexpr auto CYCLES = 1000000;
  auto label = std::string("dlog");
  auto duration = measure([&]() { run(logger, CYCLES); });

  /*
  while (char const *buf = logger.front()) {
    for (auto i = 0; i < LEN; ++i) {
      if (buf[i] > '0' && buf[i] < 'z') {
        std::cout << buf[i];
      } else {
        std::cout << "0x" << std::hex << (int32_t)buf[i];
      }
    }
    std::cout << std::endl;
    logger.pop();
  }
  */
  std::cout << "measured time (" << label << ", " << CYCLES
            << "): " << duration / 1000000000.0
            << "s, per iteration: " << duration / CYCLES << "ns" << std::endl;
}
