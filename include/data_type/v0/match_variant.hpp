//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef DATA_TYPE_V0_MATCH_VARIANT_HPP
#define DATA_TYPE_V0_MATCH_VARIANT_HPP

#include <data_type/v0/config.hpp>
#include <meta/v0/type.hpp>
#include <meta/v0/types.hpp>
#include <functional/v0/match.hpp>
#include <functional/v0/overload.hpp>

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <typeindex>

namespace functional
{
    template <class ...Types>
    struct sum_types<boost::variant<Types...>> : meta::identity<meta::types<Types...>> {};
};

namespace boost
{
  namespace variant_detail
  {
    template <class R, class V, class T1, class F>
    R match_variant(V& x, meta::types<T1>, F&& f)
    {
      T1 const& res = ::boost::get<T1>(x);
      return f(res);
    }
    template <class R, class V, class T1, class T2, class ...Ts, class F>
    R match_variant(V& x, meta::types<T1, T2, Ts...>, F&& f)
    {
      T1 const * res = ::boost::get<T1>(&x);
      if (res) return f(*res);
      else return match_variant<R>(x, meta::types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class R, class V, class T, class ...Ts, class F1, class F2, class ...Fs>
    R match_variant(V& x, meta::types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs)
    {
      return match_variant<R>(x, meta::types<T, Ts...>(),
          functional::overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
      );
    }
  }

  template <class R, class ...Types, class F>
  R match(meta::type<R>, meta::type<boost::variant<Types...>>, boost::variant<Types...> const& x, F&& f)
  {
    return variant_detail::match_variant<R>(x, meta::types<Types...>{}, std::forward<F>(f));
  }
}

#endif // header
