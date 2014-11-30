/*!
@file
Defines `YAFPL_TYPECLASS` and `YAFPL_BINARY_TYPECLASS`.

Copied from Boost.Hana
@copyright Louis Dionne 2014
@copyright Vicente J. Botet Escribe 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef YAFPL_V1_CORE_TYPECLASS_HPP
#define YAFPL_V1_CORE_TYPECLASS_HPP

#include <yafpl/v1/config.hpp>

#include <yafpl/v1/core/datatype.hpp>
#include <yafpl/v1/core/disable.hpp>
#include <yafpl/v1/core/when.hpp>


namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
    //! @defgroup group-typeclasses Type classes
    //! General purpose type classes provided by the library.
    //!
    //! @todo
    //! - Provide a way to check type class requirements like
    //!   `Functor f => Applicative f`.
    //! - Consider inheriting from a base class even when no mcd is required.
    //!   That would allow us to _not_ include a useless mcd.

    namespace core_detail {
        template <typename Void, typename Typeclass, typename ...Args>
        struct default_instance {
            using type = disable;
        };

        template <typename Typeclass, typename ...Args>
        struct default_instance<
            decltype((void)(typename Typeclass::template default_instance<Args...>*)0),
            Typeclass, Args...
        > {
            using type = typename Typeclass::template default_instance<Args...>;
        };
    }

    //! @ingroup group-core
    //! Defines a unary type class.
    //! @hideinitializer
    //!
    //! Use this macro at public scope when defining a type class to create
    //! the boilerplate necessary for a unary type class.
    //!
    //! ### Example
    //! @include example/core/unary_typeclass.cpp
    #define YAFPL_TYPECLASS(NAME)                                      \
        /** @cond */                                                        \
        template <typename T, typename = void>                              \
        struct instance                                                     \
            : instance<T, yafpl::when<true>>                        \
        { };                                                                \
                                                                            \
        template <typename T>                                               \
        struct instance<T, decltype((void)(typename T::yafpl_ ## NAME*)0)>   \
            : T::yafpl_ ## NAME                                              \
        { };                                                                \
                                                                            \
        template <typename T, bool condition>                               \
        struct instance<T, yafpl::when<condition>>                  \
            : yafpl::core_detail::default_instance<                 \
                void, NAME, T                                               \
            >::type                                                         \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/

    //! @ingroup group-core
    //! Defines a binary type class.
    //! @hideinitializer
    //!
    //! This is equivalent to `YAFPL_TYPECLASS`, except it creates a
    //! type class accepting two data types. This is useful for type classes
    //! like `Comparable`, whose methods are binary and should be dispatched
    //! using the data types of both arguments.
    //!
    //! ### Example
    //! @include example/core/binary_typeclass.cpp
    #define YAFPL_BINARY_TYPECLASS(NAME)                               \
        /** @cond */                                                        \
        template <typename T, typename U, typename = void>                  \
        struct instance                                                     \
            : instance<T, U, yafpl::when<true>>                     \
        { };                                                                \
                                                                            \
        template <typename T, typename U, bool condition>                   \
        struct instance<T, U, yafpl::when<condition>>               \
            : yafpl::core_detail::default_instance<                 \
                void, NAME, T, U                                            \
            >::type                                                         \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/
}
}

#endif // !YAFPL_CORE_TYPECLASS_HPP
