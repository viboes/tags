//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_TAGS_V0_MAP_TAG_ANY_HPP
#define BOOST_TAGS_V0_MAP_TAG_ANY_HPP

#include <tags/v0/config.hpp>
#include <tags/v0/tagged.hpp>
#include <tags/v0/tag.hpp>
#include <tags/v0/types.hpp>

#include <boost/any.hpp>

#include <typeindex>
#include <map>

namespace tags {
  TAGS_INLINE_NAMESPACE(v0) {

    struct map_tag_any {
      typedef std::map<std::type_index, boost::any> map_type;
      map_type data;

      map_tag_any()
      {}
    };

    template <class Tag, class T>
    std::pair<map_tag_any::map_type::iterator, bool> tag_insert(map_tag_any& map, T const& x)
    {
      return map.data.insert(std::pair<std::type_index, boost::any>(tag_index<Tag>(), x));
    }

    template <class Tag>
    typename tag_type<Tag>::type const* tag_cast(map_tag_any const* x) {
      map_tag_any::map_type::const_iterator it = x->data.find(tag_index<Tag>());
      if (it == x->data.end()) return 0;
      return boost::any_cast<typename tag_type<Tag>::type* >(&it->second);
    }
    template <class Tag>
    typename tag_type<Tag>::type* tag_cast(map_tag_any * x) {
      map_tag_any::map_type::iterator it = x->data.find(tag_index<Tag>());
      if (it == x->data.end()) return 0;
      return boost::any_cast<typename tag_type<Tag>::type>(&it->second);
    }

    template <class Tag>
    typename tag_type<Tag>::type const& tag_cast(map_tag_any const& x) {
      typename tag_type<Tag>::type const* res = tag_cast<Tag>(&x);
      if (res == 0) throw  std::bad_cast();
      return *res;
    }
    template <class Tag>
    typename tag_type<Tag>::type & tag_cast(map_tag_any & x) {
      typename tag_type<Tag>::type* res = tag_cast<Tag>(&x);
      if (res == 0) throw  std::bad_cast();
      return *res;
    }

    /////
    template <class R, class Tag, class F>
    R tag_accept(map_tag_any& x, F&& f) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      return R();
    }

    template <class R, class Tag, class ... Tags, class F, class ... Fs>
    R tag_accept(map_tag_any& x, F&& f, Fs&&... fs) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) f(Tag{}, *res);
      return tag_accept<R, Tags...>(x, std::forward<Fs>(fs)...);
    }

    template <class R, class Tag, class F>
    R tag_accept(map_tag_any& x, types<Tag>, F&& f) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) return f(Tag{}, *res);
      return R();
    }

    template <class R, class Tag, class ... Tags, class F, class ... Fs>
    R tag_accept(map_tag_any& x, types<Tag, Tags...>, F&& f, Fs&&... fs) {
      typedef typename tag_type<Tag>::type T;
      T* res = tag_cast<Tag>(&x);
      if (res) f(Tag{}, *res);
      return tag_accept<R>(x, types<Tags...>(), std::forward<Fs>(fs)...);
    }

    template <class R, class ... Tags, class ... Fs>
    R mtag_accept(boost::any& x, Fs&&... fs) {
      map_tag_any * res = boost::any_cast<map_tag_any>(&x);
      if (res) return tag_accept<R, Tags...>(*res, std::forward<Fs>(fs)...);
      return R();
    }

    template <class R, class ... Tags, class ... Fs>
    R mtag_accept(boost::any& x, types<Tags...>, Fs&&... fs) {
      map_tag_any * res = boost::any_cast<map_tag_any>(&x);
      if (res) return tag_accept<R>(*res, types<Tags...>(), std::forward<Fs>(fs)...);
      return throw boost::bad_any_cast();
    }

#if 0
    // example
    struct t1 {}
    map_tag_any ta = make_tag_any<t1>(1);
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
    map_tag_any ta = make_tag_any<t1>(1);
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
    map_tag_any ta = make_tag_any<t1>(1);
    map_tag_any tb = make_tag_any<t2>(2);
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
    map_tag_any ta = make_tag_any<t1>(1);
    map_tag_any tb = make_tag_any<t2>(2);
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
