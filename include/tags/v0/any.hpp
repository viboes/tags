//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_TAGS_V0_ANY_HPP
#define BOOST_TAGS_V0_ANY_HPP

#include <tags/v0/config.hpp>
#include <tags/v0/tagged.hpp>
#include <tags/v0/tag.hpp>
#include <tags/v0/types.hpp>

#include <boost/any.hpp>

#include <typeindex>

namespace tags {
  TAGS_INLINE_NAMESPACE(v0) {

    ///
    template <class R, class T, class F>
    R accept(boost::any& x, F&& f) {
      T * res = boost::any_cast<T>(&x);
      if (res) return f(*res);
      else throw boost::bad_any_cast();
    }
    template <class R, class T, class F, class G>
    R accept(boost::any& x, F&& f, G&& g) {
      T * res = boost::any_cast<T>(&x);
      if (res) return f(*res);
      else return g(x);
    }
    template <class R, class T, class ...Ts, class F, class ...Fs>
    R accept(boost::any& x, F&& f, Fs&&... fs) {
      T * res = boost::any_cast<T>(&x);
      if (res) return f(*res);
      else return accept<R, Ts...>(x, std::forward<Fs>(fs)...);
    }

    ///
    template <class R, class T, class F>
    R accept(boost::any & x, types<T>, F&& f) {
      T * res = boost::any_cast<T>(&x);
      if (res) return f(*res);
      else return throw boost::bad_any_cast();
    }
    template <class R, class T, class ...Ts, class F, class ...Fs>
    R accept(boost::any& x, types<T, Ts...>, F&& f, Fs&&... fs) {
      T * res = boost::any_cast<T>(&x);
      if (res) return f(*res);
      else return accept<R>(x, types<Ts...>(), std::forward<Fs>(fs)...);
    }

//    template <class R, class ...Ts, class ...Fs>
//    R accept(matcher<boost::any, Ts...>& x, Fs&&... fs) {
//      return accept<R>(x.value, types<Ts...>(), std::forward<Fs>(fs)...);
//    }


#if 0
    // example
    any a = 2;
    accept<void, int, string>(a,
        [](int& i) {...},
        [](string& i) {...}
    );
#endif
#if 0
    // example
    any a = 2;
    accept<void, int>(a,
        [](int& i) {...},
        [](auto&) {...}
    );
#endif
#if 0
    // example
    any a = 2;
    accept<void, int, string>(a,
        [](int& i) {...},
        [](string& i) {...}
    );
#endif

#if 0
    // example
    // taking a list of accepted types for this argument.
    any a = 2;
    accept<void>(a, types<int, string>()),
        [](int& i) {...},
        [](string& i) {...}
    );
#endif

#if 0
    // example
    // taking a wrapper listing the accepted types for this argument.
    any a = 2;
    accept<void>(select<int, string>(a)),
        [](int& i) {...},
        [](string& i) {...}
    );
#endif

#if 0
    // NOT YET IMPLEMENTED
    // example
    struct t1 {}
    any ta = 1;
    any tb = std::string("1");
    ...
    accept<void>(select<int, std::string>(ta), select<int, std::string>(tb),
        [](int    const& i, int     const& j) {...},
        [](int    const& i, string  const& j) {...},
        [](string const& i, int     const& j) {...},
        [](string const& i, string  const& j) {...},
    );
#endif

#if 0
    // NOT YET IMPLEMENTED
    // example
    struct t1 {}
    any ta = 1;
    any tb = std::string("1");
    ...
    accept<void>(select<int, short, std::string>(ta), select<int, std::string>(tb),
      [CAPTURE] struct {
        void operator()(int    const& a, int   const& b) {...};
        void operator()(short  const& a, int   const& b) {...};
        void operator()(auto   const& a, auto  const& b) {...};
      }()
    );
#endif



  }
}

#endif // header
