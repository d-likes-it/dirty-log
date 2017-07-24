#define DLOG_DEBUG

#include <iostream>
#include <cstring>
#include <chrono>

#include "dlog.hpp"
#include "log_writer.hpp"


using namespace dlog;
namespace sc = std::chrono;

template<typename Logger>
void run(Logger & logger, uint64_t n) {
  for (uint64_t i = 0; i < n; ++i) {
    write_log(logger, "Hello World {} {}", i, n);
  }
}

template<typename Fun>
void measure(std::string label, Fun fun) {
  auto start = sc::high_resolution_clock::now();

  fun();

  auto end = sc::high_resolution_clock::now();
  auto dur = sc::duration_cast<sc::nanoseconds>(end - start);
  
  std::cout << "measured time(" << label << "): " << dur.count() << "ns" << std::endl;
}


int main() {
  //  StreamLogWriter logger(std::cout);
  
  //  write_log(logger, "Hello World {} {}", 1.0, 1);

  constexpr auto N = 1024;
  constexpr auto LEN = 64;
  BenchmarkRollingLogWriter<N, LEN> logger;

  measure("", [&]() {run(logger, N);});

  while (char const * buf = logger.front()) {
    for (auto i = 0; i < LEN; ++i) {
      if (buf[i] > '0' && buf[i] < 'z') {
        std::cout << buf[i];
      } else {
        std::cout << "0x" << std::hex << (int32_t) buf[i];
      }
    }
    std::cout << std::endl;
    logger.pop();
  }
}
