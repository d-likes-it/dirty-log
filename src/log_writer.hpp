#ifndef __DLOG_LOG_WRITER__
#define __DLOG_LOG_WRITER__

#include <array>
#include <iostream>

namespace dlog {

template <std::size_t CAPACITY, std::size_t ALIGNMENT> struct logger_traits {
  static constexpr std::size_t capacity = CAPACITY;
  static constexpr std::size_t alignment = ALIGNMENT;
};

// ATTENTION: prepare and emit must not be interleaved
// (well that is my contract...)

class StreamLogWriter {
public:
  StreamLogWriter(std::ostream &os) : os_(os) {}

  using traits = logger_traits<0, 4096>;

  char *prepare() { return copy_buf_; }

  void emit() { os_ << "<" << copy_buf_ << ">" << std::endl; }

private:
  std::ostream &os_;
  char copy_buf_[traits::alignment];
};

template <std::size_t capacity, std::size_t alignment = 256>
class BenchmarkRollingLogWriter {
public:
  using traits = logger_traits<capacity, alignment>;

  BenchmarkRollingLogWriter() : write_pos_(0), read_pos_(0) {}

  char *prepare() { return mem_[write_pos_ % mem_.size()]; }

  void emit() {
    ++write_pos_;
    // capacity is limited and we throw away old stuff
    read_pos_ += (write_pos_ - read_pos_ > mem_.size()) ? 1 : 0;
  }

  char const *front() {
    if (read_pos_ < write_pos_) {
      auto *ret = mem_[read_pos_ % mem_.size()];
      return ret;
    }
    return nullptr;
  }

  void pop() {
    if (read_pos_ < write_pos_) {
      ++read_pos_;
    }
  }

private:
  std::array<char[alignment], capacity> mem_;
  uint64_t write_pos_;
  uint64_t read_pos_;
};

} // namespace dlog

#endif
