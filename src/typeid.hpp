#ifndef __DLOG_TYPEID__
#define __DLOG_TYPEID__


namespace dlog {

  template < typename T, typename TypesTuple >
  struct type_id;
  
  template < typename T,typename ... Types >
  struct type_id < T, std::tuple < T, Types... > > 
  {
    static constexpr const char value = 0;
  };
  
  template < typename T, typename U, typename ... Types >
  struct type_id < T, std::tuple < U, Types... > > 
  {
    static constexpr const char value = 1 + type_id< T, std::tuple < Types... > >::value;
  };

}

#endif
