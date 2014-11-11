//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_TAGS_V0_TAG_ANY_HPP
#define BOOST_TAGS_V0_TAG_ANY_HPP

#include <tags/v0/config.hpp>
#include <tags/v0/tagged.hpp>
#include <tags/v0/tag.hpp>

#include <boost/any.hpp>

#include <typeindex>

namespace tags {
  TAGS_INLINE_NAMESPACE(v0) {

    struct tag_any {
      std::type_index index;
      boost::any value;

      template <class T>
      tag_any(std::type_index const& index, T& x) :
        index(index),
        value(x)
      {}
    };

    template <class Tag, class T>
    tag_any make_tag_any(T const& x)
    {
      return tag_any(tag_index<Tag>(), x);
    }

    template <class Tag>
    typename tag_type<Tag>::type const* tag_cast(tag_any const* x) {
      if (x->index != tag_index<Tag>()) return 0;
      return boost::any_cast<typename tag_type<Tag>::type* >(&x->value);
    }
    template <class Tag>
    typename tag_type<Tag>::type* tag_cast(tag_any * x) {
      if (x->index != tag_index<Tag>()) return 0;
      return boost::any_cast<typename tag_type<Tag>::type>(&x->value);
    }

    template <class Tag>
    typename tag_type<Tag>::type const& tag_cast(tag_any const& x) {
      typename tag_type<Tag>::type const* res = tag_cast<Tag>(&x);
      if (res == 0) throw  std::bad_cast();
      return *res;
    }
    template <class Tag>
    typename tag_type<Tag>::type & tag_cast(tag_any & x) {
      typename tag_type<Tag>::type* res = tag_cast<Tag>(&x);
      if (res == 0) throw  std::bad_cast();
      return *res;
    }

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
    template < class ...Ts >
    struct types {};
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



    template <class R, class Tag, class F>
    R tag_accept(tag_any& x, F&& f) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      else return throw boost::bad_any_cast();
    }

    template <class R, class Tag, class ... Tags, class F, class ... Fs>
    R tag_accept(tag_any& x, F&& f, Fs&&... fs) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      else return tag_accept<R, Tags...>(x, std::forward<Fs>(fs)...);
    }

    template <class R, class Tag, class F>
    R tag_accept(tag_any& x, types<Tag>, F&& f) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      else return throw boost::bad_any_cast();
    }

    template <class R, class Tag, class ... Tags, class F, class ... Fs>
    R tag_accept(tag_any& x, types<Tag, Tags...>, F&& f, Fs&&... fs) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      else return tag_accept<R>(x, types<Tags...>(), std::forward<Fs>(fs)...);
    }

    template <class R, class ... Tags, class ... Fs>
    R tag_accept(boost::any& x, Fs&&... fs) {
      tag_any * res = boost::any_cast<tag_any>(&x);
      if (res) return tag_accept<R, Tags...>(*res, std::forward<Fs>(fs)...);
      return throw boost::bad_any_cast();
    }

    template <class R, class ... Tags, class ... Fs>
    R tag_accept(boost::any& x, types<Tags...>, Fs&&... fs) {
      tag_any * res = boost::any_cast<tag_any>(&x);
      if (res) return tag_accept<R>(*res, types<Tags...>(), std::forward<Fs>(fs)...);
      return throw boost::bad_any_cast();
    }

#if 0
    // example
    struct t1 {}
    tag_any ta = make_tag_any<t1>(1);
    ...
    tag_accept<void, t1, t2>(ta,
        [](t1, int& i) {...},
        [](t2, string& i) {...}
    );
#endif

#if 0
    // example
    // alternative: make use of tagged
    // NOT YET IMPLEMENTED
    struct t1 {}
    tag_any ta = make_tag_any<t1>(1);
    ...
    tag_accept<void, t1, t2>(ta,
        [](tagged<int,    t1> const& i) {...},
        [](tagged<string, t2> const& s) {...}
    );
#endif


#if 0
    // example
    struct t1 {}
    any a = make_tag_any<t1>(1);
    ...
    tag_accept<void, t1>(a,
        [](t1 , int& i) {...}
    );
#endif


#if 0
    // example
    // alternative: make use of select
    // NOT YET IMPLEMENTED
    struct t1 {}
    tag_any ta = make_tag_any<t1>(1);
    tag_any tb = make_tag_any<t2>(2);
    ...
    tag_accept<>(select<t1, t2>(ta), select<t1, t2>(tb)
        [](t1, int    const& i, t1, int     const& j) {...},
        [](t1, int    const& i, t2, string  const& s) {...},
        [](t2, string const& s, t1, int     const& j) {...},
        [](t2, string const& i, t2, string  const& j) {...},
    );
#endif

#if 0
    // example
    // alternative: make use of tagged
    // NOT YET IMPLEMENTED
    struct t1 {}
    tag_any ta = make_tag_any<t1>(1);
    tag_any tb = make_tag_any<t2>(2);
    ...
    tag_accept<>(select<t1, t2>(ta), select<t1, t2>(tb),
        [](tagged<int,    t1> const& i, tagged<int,     t1> const& j) {...},
        [](tagged<int,    t1> const& i, tagged<string,  t2> const& s) {...},
        [](tagged<string, t2> const& s, tagged<int,     t1> const& i) {...},
        [](tagged<string, t2> const& s, tagged<string,  t2> const& s) {...}
    );
#endif

  }
}

#endif // header
