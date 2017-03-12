#ifndef __DLOG_SIGNATURE__
#define __DLOG_SIGNATURE__

#include "seq.hpp"
#include "typeid.hpp"

namespace dlog {

  template<typename... Args>
  struct signature {
    typedef concat_seq<
      std::tuple<
	seq<
	  type_id<Args, Types>::value> ...
	> > value;
  };

}

#endif
