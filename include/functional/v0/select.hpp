//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTIONAL_V0_SELECT_HPP
#define FUNCTIONAL_V0_SELECT_HPP

#include <tags/v0/config.hpp>
#include <meta/v0/none.hpp>
#include <meta/v0/types.hpp>

#include <utility>
#include <type_traits>

namespace functional {
  CONFIG_INLINE_NAMESPACE(v0) {

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

  }
}

#endif // header
