//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_FUNCTIONAL_PATTERN_HPP
#define YAFPL_V1_FUNCTIONAL_PATTERN_HPP

#include <yafpl/v1/config.hpp>
#include <utility>

namespace yafpl {
  YAFPL_INLINE_NAMESPACE(v1) {

    template< class Type, class Tag >
    struct pattern_t
    {
      typedef Type value_type;
      typedef Tag tag_type;

      explicit constexpr pattern_t(Type const& v) : value(v) {}
      explicit constexpr pattern_t(Type && v) : value(std::move(v)) {}

      value_type value;

      friend constexpr bool operator==(pattern_t const& x, pattern_t const y)
          // requires EQ<Type>
      {
        return x.value == y.value;
      }
      friend constexpr bool operator!=(pattern_t const& x, pattern_t const y)
          // requires EQ<Type>
      {
        return x.value != y.value;
      }
      friend constexpr bool operator<(pattern_t const& x, pattern_t const y)
          // requires Ord<Type>
      {
        return x.value < y.value;
      }
      friend constexpr bool operator<=(pattern_t const& x, pattern_t const y)
          // requires Ord<Type>
      {
        return x.value <= y.value;
      }
      friend constexpr bool operator>(pattern_t const& x, pattern_t const y)
          // requires Ord<Type>
      {
        return x.value > y.value;
      }
      friend constexpr bool operator>=(pattern_t const& x, pattern_t const y)
          // requires Ord<Type>
      {
        return x.value >= y.value;
      }

    };

    template< class Tag >
    struct pattern_t<void, Tag>
    {
      typedef void value_type;
      typedef Tag tag_type;

      friend constexpr bool operator==(pattern_t const& x, pattern_t const y)
      {
        return true;
      }
      friend constexpr bool operator!=(pattern_t const& x, pattern_t const y)
      {
        return false;
      }
      friend constexpr bool operator<(pattern_t const& x, pattern_t const y)
      {
        return false;
      }
      friend constexpr bool operator<=(pattern_t const& x, pattern_t const y)
      {
        return true;
      }
      friend constexpr bool operator>(pattern_t const& x, pattern_t const y)
      {
        return false;
      }
      friend constexpr bool operator>=(pattern_t const& x, pattern_t const y)
      {
        return true;
      }

    };

    template< class Tag, class Type >
    pattern_t<Type, Tag> pattern(Type && x)
    {
      return pattern_t<Type, Tag>(std::forward<Type>(x));
    }
    template< class Tag>
    pattern_t<void, Tag> pattern()
    {
      return pattern_t<void, Tag>();
    }
  }
}

#define YAFPL_NEWTYPE(name) \
    struct BOOST_JOIN(name,_) {}; \
    using BOOST_JOIN(name,_t)  = ::yafpl::v1::pattern_t<void, BOOST_JOIN(name,_)>; \
    constexpr BOOST_JOIN(name,_t) name = {}

#define YAFPL_NEWTYPE_1(name) \
    struct BOOST_JOIN(name,_) {}; \
    template <class T> \
    using BOOST_JOIN(name,_t)  = ::yafpl::v1::pattern_t<T, BOOST_JOIN(name,_)>; \
    template <class T> \
    constexpr BOOST_JOIN(name,_t)<T> name(T&& x) { return BOOST_JOIN(name,_t)<T>(std::forward<T>(x)); } \
    inline constexpr BOOST_JOIN(name,_t)<void> name() { return BOOST_JOIN(name,_t)<void>(); }



#endif // header
