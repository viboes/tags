//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_TYPE_CONSTRUCTOR_HPP
#define YAFPL_V1_META_TYPE_CONSTRUCTOR_HPP

#include <yafpl/v1/config.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  namespace meta
  {
    // type constructor tag type used to check if a class is a type constructor
    struct type_constructor_tag {};

    // type trait based on inheritance from type_constructor_tag
    // todo change for has TC::template type
    template <class TC >
    struct is_type_constructor : is_base_of<type_constructor_tag, TC> {};

    // apply a type constuctor TC to the type parameters Args
    template<class TC, class... Args>
    using apply = typename TC::template apply<Args...>;

    // type constructor customization point.
    // Default implementation make use of a nested type type_constructor
    template <class M >
    struct type_constructor : identity<typename  M::type_constructor> {};

    // type constructor getter meta-function
    template <class M >
    using type_constructor_t = typename type_constructor<M>::type;

    // rebinds a type having a underlying type with another underlying type
    template <class M, class U>
    using rebind = apply<type_constructor_t<M>, U>;

    // transforms a template class into a type_constructor that adds the parameter at the end
    template <template <class ...> class TC, class... Args>
    struct lift : type_constructor_tag
    {
      template<class... Args2>
      using apply = TC<Args..., Args2...>;
    };

    // transforms a template class into a type_constructor that adds the parameter at the begining
    template <template <class ...> class TC, class... Args>
    struct reverse_lift : type_constructor_tag
    {
      template<class... Args2>
      using apply = TC<Args2..., Args...>;
    };


//  // undelying_type customization point.
//  // Default implementation make use of a nested type underlying_type
//  template <class M >
//  struct underlying_type : identity<typename  M::underlying_type> {};
//
//  // underlying_type getter meta-function
//  template <class M >
//  using underlying_type_t = typename underlying_type<M>::type;


  }
}
}
#endif // header
