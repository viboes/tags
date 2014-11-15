//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TAGS_V0_TAGGED_HPP
#define TAGS_V0_TAGGED_HPP

#include <tags/v0/config.hpp>

namespace tags {
  CONFIG_INLINE_NAMESPACE(v0) {

    template< class Type, class Tag >
    struct tagged
    {
        typedef Type value_type;
        typedef Tag tag_type;
    };

    template< class T>
    struct value_type {
      typedef T type;
    };

    template< class T, class Tag >
    struct value_type<tagged<T, Tag> > {
      typedef T type;
    };

    template< class T >
    using value_type_t = typename value_type<T>::type;
  }
}

#endif // header
