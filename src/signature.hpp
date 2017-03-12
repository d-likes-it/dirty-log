#ifndef __DLOG_SIGNATURE__
#define __DLOG_SIGNATURE__

#include "type_id.hpp"

namespace dlog {

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
  
  template<typename ... Cs> struct signature  {
    static constexpr std::size_t noOfArgs = sizeof...(Cs);
    static constexpr std::size_t sizeOfSignatureString = noOfArgs + 1;
    static constexpr std::size_t sizeOfArgs = _sizeof<Cs...>::value;
    
    static constexpr char signatureString[sizeOfSignatureString] = {noOfArgs, type_id<Cs>::value ...};
  };

  // to create the signatureString variable text item for linking... Wow!!!
  template<typename ... Cs>
  constexpr char signature<Cs ...>::signatureString[signature<Cs ...>::sizeOfSignatureString];
}

#endif
