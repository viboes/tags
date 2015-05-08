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


#ifndef YAFPL_V1_DATA_TYPE_MAYBE_MAYBE_HPP
#define YAFPL_V1_DATA_TYPE_MAYBE_MAYBE_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/data_type/boost/variant.hpp>
#include <yafpl/v1/functional/pattern.hpp>
#include <yafpl/v1/meta/type.hpp>

#include <yafpl/v1/type_class/monad/monad.hpp>

#include <utility>

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

    template <class R, class F>
    friend R match_custom(meta::types<R, maybe<T>>, maybe<T> const& x, F&& f)
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

#endif // header
