//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_DATA_TYPE_BOOST_VARIANT_MATCH_VARIANT_HPP
#define YAFPL_V1_DATA_TYPE_BOOST_VARIANT_MATCH_VARIANT_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/meta/id.hpp>
#include <yafpl/v1/meta/types.hpp>
#include <yafpl/v1/functional/match.hpp>
#include <yafpl/v1/functional/overload.hpp>
#include <yafpl/v1/type_class/sum_type/sum_type_alternatives.hpp>

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <typeindex>

namespace yafpl
{
    template < class ...Ts >
    struct type_list
    {};

    // just to show that we can use another template for enumerating the alternatives.
    template <class ...Types>
    struct sum_type_alternatives<boost::variant<Types...>> : yafpl::meta::identity<type_list<Types...>> {};
};

namespace std
{
  template < class ...Types >
  struct tuple_size<yafpl::type_list<Types...>>
  : public integral_constant<size_t, sizeof...(Types)> { };

  template <size_t I>
  class tuple_element<I, yafpl::type_list<> >
  {
  public:
      static_assert(I == 0, "tuple_element index out of range");
      static_assert(I != 0, "tuple_element index out of range");
  };

  template <class T, class ...Ts>
  class tuple_element<0, yafpl::type_list<T, Ts...> >
  {
  public:
      typedef T type;
  };

  template <size_t I, class T, class ...Ts>
  class tuple_element<I, yafpl::type_list<T, Ts...> >
  {
  public:
      typedef typename tuple_element<I-1, yafpl::type_list<Ts...> >::type type;
  };
}

namespace boost
{

#if ! defined YAFPL_X1
  template <class R, class ...Types, class F>
  R match(yafpl::meta::id<R>, boost::variant<Types...> const& x, F&& f)
  {
    //return variant_detail::match_variant<R>(x, yafpl::meta::types<Types...>{}, std::forward<F>(f));
    return boost::apply_visitor(std::forward<F>(f), x);
  }
  template <class R, class ...Types, class F>
  R match(yafpl::meta::id<R>, boost::variant<Types...> & x, F&& f)
  {
    //return variant_detail::match_variant<R>(x, yafpl::meta::types<Types...>{}, std::forward<F>(f));
    return boost::apply_visitor(std::forward<F>(f), x);
  }
#else
  template <class ...Types, class F>
  auto match(boost::variant<Types...> const& x, F&& f)
  {
    return boost::apply_visitor(std::forward<F>(f), x);
  }
  template <class ...Types, class F>
  auto match(boost::variant<Types...> & x, F&& f)
  {
    return boost::apply_visitor(std::forward<F>(f), x);
  }
#endif
}

#endif // header
