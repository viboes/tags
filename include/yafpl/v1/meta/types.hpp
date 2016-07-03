//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_TYPES_HPP
#define YAFPL_V1_META_TYPES_HPP

#include <yafpl/v1/config.hpp>
#include <tuple>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  namespace meta
  {

    template < class ...Ts >
    struct types
    {};

  }
}
}

namespace std
{
  template < class ...Types >
  class tuple_size<yafpl::meta::types<Types...>>
  : public integral_constant<size_t, sizeof...(Types)> { };

  template <size_t I>
  class tuple_element<I, yafpl::meta::types<> >
  {
  public:
      static_assert(I == 0, "tuple_element index out of range");
      static_assert(I != 0, "tuple_element index out of range");
  };

  template <class T, class ...Ts>
  class tuple_element<0, yafpl::meta::types<T, Ts...> >
  {
  public:
      typedef T type;
  };

  template <size_t I, class T, class ...Ts>
  class tuple_element<I, yafpl::meta::types<T, Ts...> >
  {
  public:
      typedef typename tuple_element<I-1, yafpl::meta::types<Ts...> >::type type;
  };
}

#endif // header
