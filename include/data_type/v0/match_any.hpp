//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef DATA_TYPE_V0_MATCH_ANY_HPP
#define DATA_TYPE_V0_MATCH_ANY_HPP

#include <data_type/v0/config.hpp>
//#include <tags/v0/tagged.hpp>
//#include <tags/v0/tag.hpp>
#include <meta/v0/type.hpp>
#include <meta/v0/types.hpp>
#include <functional/v0/select.hpp>
#include <functional/v0/overload.hpp>

#include <boost/any.hpp>

#include <typeindex>

namespace boost
{
  namespace any_detail
  {
    template <class R, class T1, class F>
    R match(any& x, meta::types<T1>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return f();
    }
    template <class R, class T1, class T2, class ...Ts, class F>
    R match(any& x, meta::types<T1, T2, Ts...>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return any_detail::match<R>(x, meta::types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class R, class T, class ...Ts, class F1, class F2, class ...Fs>
    R match(any& x, meta::types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs)
    {
      return any_detail::match<R>(x, meta::types<T, Ts...>(),
          functional::make_overload<R>(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
      );
    }
    template <class R, class Types, class ...Fs>
    R match(functional::selector<any, Types> && x, Fs&&... fs)
    {
      return any_detail::match<R>(x.value, Types
          {}, std::forward<Fs>(fs)...);
    }
  }

  template <class R, class Types, class F>
  R match(meta::type<R>, meta::type<functional::selector<any, Types>>, functional::selector<any, Types> const& x, F&& f)
  {
    return any_detail::match<R>(x.value, Types{}, std::forward<F>(f));
  }
}

#endif // header
