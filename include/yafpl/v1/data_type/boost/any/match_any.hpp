//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_DATA_TYPE_BOOST_ANY_MATCH_ANY_HPP
#define YAFPL_V1_DATA_TYPE_BOOST_ANY_MATCH_ANY_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/meta/id.hpp>
#include <yafpl/v1/meta/types.hpp>
#include <yafpl/v1/functional/select.hpp>
#include <yafpl/v1/functional/overload.hpp>

#include <boost/any.hpp>

#include <typeindex>

namespace boost
{
  namespace any_detail
  {
#if ! defined YAFPL_X1
    template <class R, class T1, class F>
    R match_any(any& x, yafpl::meta::types<T1>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return f();
    }
    template <class R, class T1, class T2, class ...Ts, class F>
    R match_any(any& x, yafpl::meta::types<T1, T2, Ts...>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return match_any<R>(x, yafpl::meta::types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class R, class T, class ...Ts, class F1, class F2, class ...Fs>
    R match_any(any& x, yafpl::meta::types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs)
    {
      return match_any<R>(x, yafpl::meta::types<T, Ts...>(),
          yafpl::overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
      );
    }
    template <class R, class Types, class ...Fs>
    R match_any(yafpl::selector<any, Types> && x, Fs&&... fs)
    {
      return match_any<R>(x.value, Types{}, std::forward<Fs>(fs)...);
    }
#else
    template <class T1, class F>
    auto match_any(any& x, yafpl::meta::types<T1>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return (decltype(f(*res)))f();
    }
    template <class T1, class T2, class ...Ts, class F>
    auto match_any(any& x, yafpl::meta::types<T1, T2, Ts...>, F&& f)
    {
      T1 * res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return (decltype(f(*res)))match_any(x, yafpl::meta::types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class T, class ...Ts, class F1, class F2, class ...Fs>
    auto match_any(any& x, yafpl::meta::types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs)
    {
      return match_any(x, yafpl::meta::types<T, Ts...>(),
          yafpl::overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
      );
    }
    template <class Types, class ...Fs>
    auto match_any(yafpl::selector<any, Types>& x, Fs&&... fs)
    {
      return match_any(x.value, Types{}, std::forward<Fs>(fs)...);
    }

    template <class T1, class F>
    auto match_any(any const& x, yafpl::meta::types<T1>, F&& f)
    {
      T1  const* res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return (decltype(f(*res)))f();
    }
    template <class T1, class T2, class ...Ts, class F>
    auto match_any(any const& x, yafpl::meta::types<T1, T2, Ts...>, F&& f)
    {
      T1  const* res = any_cast<T1>(&x);
      if (res) return f(*res);
      else return (decltype(f(*res)))match_any(x, yafpl::meta::types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class T, class ...Ts, class F1, class F2, class ...Fs>
    auto match_any(any const& x, yafpl::meta::types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs)
    {
      return match_any(x, yafpl::meta::types<T, Ts...>(),
          yafpl::overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
      );
    }
    template <class Types, class ...Fs>
    auto match_any(yafpl::selector<any, Types> const& x, Fs&&... fs)
    {
      return match_any(x.value, Types{}, std::forward<Fs>(fs)...);
    }
#endif
  }

#if ! defined YAFPL_X1
  template <class R, class Types, class F>
  R match(yafpl::meta::id<R>, yafpl::selector<any, Types> const& x, F&& f)
  {
    return any_detail::match_any<R>(x.value, Types{}, std::forward<F>(f));
  }
#else
  template <class Types, class F>
  auto match(yafpl::selector<any, Types> const& x, F&& f)
  {
    return any_detail::match_any(x.value, Types{}, std::forward<F>(f));
  }
  template <class Types, class F>
  auto match(yafpl::selector<any, Types>& x, F&& f)
  {
    return any_detail::match_any(x.value, Types{}, std::forward<F>(f));
  }
#endif
}

#endif // header
