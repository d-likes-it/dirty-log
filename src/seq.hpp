#ifndef __DLOG_SEQ__
#define __DLOG_SEQ__

namespace dlog {

  template<char...Cs> struct seq
  {
    typedef const char (&arr_type)[sizeof...(Cs) + 1];

    static constexpr arr_type c_str() { return str; }
    static constexpr char str[sizeof...(Cs) + 1] = {Cs..., '\0'};

    static constexpr std::size_t size = sizeof...(Cs);
  };

  //template<char...Cs>
  //constexpr char seq<Cs...>::str[sizeof...(Cs) + 1];
  

  template<typename T, typename ... Ts>
  struct concat_seq;

  template<char...Cs>
  struct concat_seq<seq<Cs...>> : seq<Cs...> {};

  template<char...Cs1, char...Cs2>
  struct concat_seq<seq<Cs1...>, seq<Cs2...>> : seq<Cs1..., Cs2...> {};

  template<char...Cs1, char...Cs2, typename ... Ts>
  struct concat_seq<seq<Cs1...>, seq<Cs2...>, Ts...> :
    concat_seq<seq<Cs1..., Cs2...>, Ts...> {};

  template<typename ... Ts>
  struct concat_seq<std::tuple<Ts...>> : concat_seq<Ts...> {};

}

#endif
