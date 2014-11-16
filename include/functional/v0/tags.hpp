//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTIONAL_V0_TAGS_HPP
#define FUNCTIONAL_V0_TAGS_HPP

#include <functional/v0/config.hpp>
#include <meta/v0/types.hpp>

#include <utility>

namespace functional {
  CONFIG_INLINE_NAMESPACE(v0) {

    /// An absolute tag has associated two informations available at compile time
    ///   * The type associated to a tag know at compile time. To do so the user must specialize the tag_type struct.
    ///   * A unique index that can be used in ordered containers

    template< class Tag >
    struct tag_type;

    template< class Tag >
    using tag_type_t = typename tag_type<Tag>::type;

#if 0
    struct t1 {};
    // specialization for t1
    template<>
    struct tag_type<t1> {
      typedef int type;
    };
#endif

    template< class Type, class Tag >
    struct tagged
    {
        typedef Type value_type;
        typedef Tag tag_type;

        tagged() = default;
        tagged(tagged const&) = default;
        tagged(tagged &&) = default;
        tagged& operator=(tagged const&) = default;
        tagged& operator=(tagged &&) = default;

        template <class T>
        tagged(T && v)
        : val(std::forward<T>(v)) {}

        value_type& value() { return val; }
        value_type const& value() const { return val; }
        //value_type && value() && { return val; }

    private:
        value_type val;
    };

    template< class Type, class Tag >
    struct tagged<Type &, Tag>
    {
        typedef Type& value_type;
        typedef Tag tag_type;

        tagged() = default;
        tagged(tagged const&) = default;
        tagged(tagged &&) = default;
        tagged& operator=(tagged const&) = default;
        tagged& operator=(tagged &&) = default;

        tagged(Type & v)
        : val(&v) {}

        value_type& value() { return *val; }
        value_type const& value() const { return *val; }

    private:
        Type* val;
    };

    template< class Type, class Tag >
    struct tagged<Type *, Tag>
    {
        typedef Type* value_type;
        typedef Tag tag_type;

        tagged() = default;
        tagged(tagged const&) = default;
        tagged(tagged &&) = default;
        tagged& operator=(tagged const&) = default;
        tagged& operator=(tagged &&) = default;

        tagged(Type * v)
        : val(v) {}

        Type* value() { return val; }
        Type const* value() const { return val; }

    private:
        Type* val;
    };

    template< class Tag, class Type >
    tagged<tag_type_t<Tag>, Tag> make_tagged(Type && x)
    {
      return tagged<tag_type_t<Tag>, Tag>(std::forward<Type>(x));
    }

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

    template <class ...Tags>
    struct tagged_types {
      using type = meta::types<tagged<tag_type_t<Tags>, Tags> ...>;
    };

    template< class ...Tags >
    using tags = typename tagged_types<Tags...>::type;

  }
}

#endif // header
