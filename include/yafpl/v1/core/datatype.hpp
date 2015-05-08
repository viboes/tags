/*!
@file
Defines `yafpl::datatype` and `yafpl::datatype_t`.

Copied from Boost.Hana
@copyright Louis Dionne 2014
@copyright Vicente J. Botet Escriba 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef YAFPL_CORE_DATATYPE_HPP
#define YAFPL_CORE_DATATYPE_HPP

#include <yafpl/v1/core/when.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
    //! @defgroup group-datatypes Data types
    //! General purpose data types provided by the library.

    namespace core_detail {
        template <typename T, typename Enable = void>
        struct default_datatype { using type = T; };

        template <typename T>
        struct default_datatype<T, decltype((void)(typename T::yafpl_datatype*)0)> {
            using type = typename T::yafpl_datatype;
        };
    }

    //! @ingroup group-core
    //! Metafunction returning the data type associated to `T`.
    //!
    //! First, note that `datatype<T>` is always equivalent to `datatype<U>`,
    //! where `U` is the type `T` after being stripped of all references and
    //! cv-qualifiers. Then, `datatype<U>` returns
    //! @code
    //!     U::yafpl_datatype
    //! @endcode
    //! if that expression is valid, and `U` otherwise. It can also be
    //! specialized to customize the data type of `U` without requiring `U`
    //! to have a nested `yafpl_datatype` type. Finally, it is also possible
    //! to use `when` to enable a `datatype` specialization only when some
    //! boolean condition is true or when some expression is well-formed with
    //! `is_valid`.
    //!
    //! ### Example
    //! @snippet example/core/datatype.cpp main
    template <typename T, typename = void>
    struct datatype
    //! @cond
        : datatype<T, when<true>>
    //! @endcond
    { };

    template <typename T, bool condition>
    struct datatype<T, when<condition>> {
        using type = typename core_detail::default_datatype<T>::type;
    };

    template <typename T> struct datatype<T const> : datatype<T> { };
    template <typename T> struct datatype<T volatile> : datatype<T> { };
    template <typename T> struct datatype<T const volatile> : datatype<T> { };
    template <typename T> struct datatype<T&> : datatype<T> { };
    template <typename T> struct datatype<T&&> : datatype<T> { };

    //! @ingroup group-core
    //! Alias to `datatype<T>::%type`.
    template <typename T>
    using datatype_t = typename datatype<T>::type;
}
}

#endif // header
