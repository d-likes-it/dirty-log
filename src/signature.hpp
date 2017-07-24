#ifndef __DLOG_SIGNATURE__
#define __DLOG_SIGNATURE__

#include "type_id.hpp"

namespace dlog {

  namespace detail {
    template <typename ... Types>
    struct _sizeof;
    
    template <typename TFirst>
    struct _sizeof<TFirst>  {
      static const auto value = (sizeof(TFirst));
    };
    
    template <typename TFirst, typename ... TRemaining>
    struct _sizeof<TFirst, TRemaining ...> {
      static const auto value = (sizeof(TFirst) + _sizeof<TRemaining...>::value);
    };
  }

  
  template<typename ... Cs> struct signature_traits  {
    static constexpr std::size_t no_of_args = sizeof...(Cs);
    static constexpr std::size_t size = detail::_sizeof<Cs...>::value;
  };
 

  template<typename ... Cs> struct signature_string  {
    using traits = signature_traits<Cs...>;
    
    static constexpr std::size_t size = 1 + traits::no_of_args;
    static constexpr char value[size] = {traits::no_of_args, type_id<Cs>::value ...};    
  };

  
  // to create the signature_string text item for linking...
  template<typename ... Cs>
  constexpr char signature_string<Cs ...>::value[signature_string<Cs ...>::size];
  
}

#endif
