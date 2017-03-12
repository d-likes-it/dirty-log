#ifndef __DLOG_LOGGABLETYPES__
#define __DLOG_LOGGABLETYPES__

#include "typeid.hpp"

namespace dlog {
  
  struct string_literal_t
  {
    explicit string_literal_t(char const * s) : m_s(s) {}
    char const * m_s;
  };
  
  typedef std::tuple < char,
		       uint32_t,
		       uint64_t,
		       int32_t,
		       int64_t,
		       double,
		       string_literal_t,
		       char * > SupportedTypes;
}

#endif
