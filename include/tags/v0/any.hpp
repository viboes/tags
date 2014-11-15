//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TAGS_V0_ANY_HPP
#define TAGS_V0_ANY_HPP

#include <tags/v0/config.hpp>
#include <tags/v0/tagged.hpp>
#include <tags/v0/tag.hpp>
#include <tags/v0/select.hpp>
#include <meta/v0/type.hpp>
#include <meta/v0/types.hpp>
#include <functional/v0/overload.hpp>

#include <boost/any.hpp>

#include <typeindex>

namespace tags {
  CONFIG_INLINE_NAMESPACE(v0) {

    using meta::type;
    using meta::types;

    template <class R, class T1, class F>
    R match(boost::any& x, types<T1>, F&& f) {
      T1 * res = boost::any_cast<T1>(&x);
      if (res) return f(*res);
      else return f();
    }
    template <class R, class T1, class T2, class ...Ts, class F>
    R match(boost::any& x, types<T1, T2, Ts...>, F&& f) {
      T1 * res = boost::any_cast<T1>(&x);
      if (res) return f(*res);
      else return tags::match<R>(x, types<T2, Ts...>(), std::forward<F>(f));
    }

    template <class R, class T, class ...Ts, class F1, class F2, class ...Fs>
    R match(boost::any& x, types<T, Ts...>, F1&& f1, F2&& f2, Fs&&... fs) {
      return tags::match<R>(x, types<T, Ts...>(),
                      functional::make_overload<R>(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...)
                     );
    }

    template <class R, class Types, class ...Fs>
    R match(selector<boost::any, Types> && x, Fs&&... fs) {
      return tags::match<R>(x.value, Types{}, std::forward<Fs>(fs)...);
    }

    template <class R, class Types, class F>
    R match(type<R>, type<selector<boost::any, Types>>, selector<boost::any, Types> const& x, F&& f) {
      return tags::match<R>(x.value, Types{}, std::forward<F>(f));
    }

  }
}

#endif // header
