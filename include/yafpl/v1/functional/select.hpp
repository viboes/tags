//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_FUNCTIONAL_SELECT_HPP
#define YAFPL_V1_FUNCTIONAL_SELECT_HPP

#include <yafpl/v1/config.hpp>

#include <yafpl/v1/meta/none.hpp>
#include <yafpl/v1/meta/types.hpp>
#include <yafpl/v1/meta/identity.hpp>
#include <yafpl/v1/functional/match.hpp>

#include <utility>
#include <type_traits>

namespace yafpl {
  YAFPL_INLINE_NAMESPACE(v1) {

    template < class T, class Types >
    struct selector;

    template < class T, class... Types >
    struct selector<T, meta::types<Types...>> {
      T& value;
      using types = meta::types<Types...>;
      selector(T& v) : value(v) {}
    };

    template < class Types, class T>
    selector<std::decay_t<T>, Types> select(T& v) {
      return selector<std::decay_t<T>, Types>(v);
    }

    template <class T, class Types>
    struct sum_types<selector<T, Types>>: meta::identity<Types>
    {
    };
  }
}

#endif // header
