//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_TAGS_V0_TAG_HPP
#define BOOST_TAGS_V0_TAG_HPP

#include <tags/v0/config.hpp>
#include <tags/v0/tagged.hpp>
#include <typeindex>

namespace tags {
  TAGS_INLINE_NAMESPACE(v0) {

    /// An absolute tag has associated two informations available at compile time
    ///   * The type associated to a tag know at compile time. To do so the user must specialize the tag_type struct.
    ///   * A unique index that can be used in ordered containers

    template< class Tag >
    struct tag_type;

    template< class Tag >
    using tag_type_t = typename tag_type<Tag>::type;

#if 0
    // specialization for t1
    template<>
    struct tag_type<t1> {
      typedef int type;
    };
#endif

    /// With C++11 compilers
    template< class Tag >
    std::type_index tag_index() {
      return std::type_index(typeid(tagged<typename tag_type<Tag>::type, Tag>));
    }
  }
}

#endif // header
