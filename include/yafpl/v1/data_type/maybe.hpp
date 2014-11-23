//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
//
//  maybe<int> x = nothing;
//  maybe<int> x = just{1};
//
//  maybe<void> x = nothing;
//  maybe<void> x = just{};


#ifndef YAFPL_V1_DATA_TYPE_MAYBE_HPP
#define YAFPL_V1_DATA_TYPE_MAYBE_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/data_type/match_variant.hpp>
#include <yafpl/v1/functional/pattern.hpp>
#include <yafpl/v1/meta/type.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  namespace monad
  {

    // mreturn : T -> M<T>
    // I don't know how this function can be implemented for each instance of a monad.
    // We need the context of the monad M<>. that means that we should use it as e.g.
    //   mreturn<maybe>(a)
    //
    // An alternative could be to define mreturn as
    //   mreturn : T -> mreturn_t<T>
    //
    // and require that the monad must define the implicit construction from this boxed value
    //   M<T>(mreturn_t<T> x);
    //
    // E.g. for maybe<T>, it can be defined as
    //   maybe<T>(just(x.value))
    //
    // That has the disadvantage that maybe cannot be defined directly as a sum type
    //   using maybe = ::boost::variant< nothing_t, just_t<T> >;
    // We need to wrap ::boost::variant< nothing_t, just_t<T> > so that we are able to add this
    // implicit constructor from box<T>.
    //
    // This has an additional disadvantage, that is, that the type maybe depends on monad :(

    YAFPL_NEWTYPE_1(mreturn);

    // mbind : M<T> -> (T -> M<U>) -> M<U>
    template <class M, class F>
      auto mbind(M const& x, F&& fct)
    {
      return mbind(meta::type<M>{}, x, std::forward<F>(fct));

    }
  }
}
}

// maybe
namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{

  YAFPL_NEWTYPE(nothing);
  YAFPL_NEWTYPE_1(just);

  template <class T>
  class maybe
  {
    ::boost::variant< nothing_t, just_t<T> > data;

    template <class R, class T1, class F>
    friend R match(meta::type<R>, meta::type<maybe<T1>>, maybe<T> const& x, F&& f)
    {
      return match<R>(x.data, std::forward<F>(f));
    }
  public:
    using types = meta::types<nothing_t, just_t<T>>;

    maybe() : data() {}
    maybe(nothing_t) : data(nothing) {}
    maybe(just_t<T> x) : data(x) {}
    maybe(monad::mreturn_t<T> x) : maybe(just(x.value)) {}

    friend bool operator==(maybe const& x, maybe const& y) {
      return x.data == y.data;
    }
    friend bool operator!=(maybe const& x, maybe const& y) {
      return ! (x == y);
    }
    friend bool operator<(maybe const& x, maybe const& y) {
      return x.data < y.data;
    }
    friend bool operator<=(maybe const& x, maybe const& y) {
      return ( x < y ) || ( x==y );
    }
    friend bool operator>(maybe const& x, maybe const& y) {
      return ! (x <= y);
    }
    friend bool operator>=(maybe const& x, maybe const& y) {
      return ! (x < y);
    }
  };

}
}

// maybe monad instance
namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  // mreturn : T -> maybe<U>
  // Make use of impicit construction from mreturn_t<T>
  //
  // mbind : maybe<T> -> (T -> maybe<U>) -> maybe<U>
  // The definition using pattern matching should be no the more efficient. Just presented here as an example.
  template <class T, class F>
    auto mbind(meta::type<maybe<T>>, maybe<T> const& x, F&& fct)
  {
    using R = decltype(fct(std::declval<T>()));
    return match<R>(x,
        [fct](just_t<T> a) { return fct(a.value); },
        [](nothing_t) { return nothing; }
    );
  }
}
}

#endif // header
