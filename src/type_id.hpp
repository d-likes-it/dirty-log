#ifndef __DLOG_TYPEID__
#define __DLOG_TYPEID__

#include <cstdint>

namespace dlog {

  template < typename T>
  struct type_id;

#define DLOG_BASE_TYPE_ID __COUNTER__ 
#define DLOG_TYPE_ID(TYPE) \
  template <> \
  struct type_id< TYPE > { \
    static constexpr const char value = (__COUNTER__ - DLOG_BASE_TYPE_ID - 1); \
  }


  DLOG_TYPE_ID(char);
  DLOG_TYPE_ID(uint32_t);
  DLOG_TYPE_ID(uint64_t);
  DLOG_TYPE_ID(int32_t);
  DLOG_TYPE_ID(int64_t);
  DLOG_TYPE_ID(double);
  //DLOG_TYPE_ID(string_literal_t);
  DLOG_TYPE_ID(char *);

#undef DLOG_TYPE_ID
#undef DLOG_BASE_TYPE_ID

}

#endif
