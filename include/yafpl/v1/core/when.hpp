/*!
@file
Defines `yafpl::when` and `yafpl::is_valid`.

Copied from Boost.Hana
@copyright Louis Dionne 2014
@copyright Vicente J. Botet Escriba 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef YAFPL_V1_CORE_WHEN_HPP
#define YAFPL_V1_CORE_WHEN_HPP

#include <yafpl/v1/config.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
    //! @ingroup group-core
    //! Enable a type class instance or a data type specialization only if a
    //! boolean condition is true.
    //!
    //! @internal
    //! ### Rationale for using `when` instead of `std::enable_if`
    //! Using `when` is necessary for two reasons. First, a non-type template
    //! argument may not depend on a template parameter of a partial
    //! specialization, so we need to wrap the `bool` condition into a
    //! type. Second, `when` is used to control the priority of partial
    //! specializations in a finer grained manner than what can be achieved
    //! with the usual `typename Enable = void` and `std::enable_if` pattern.
    //! @endinternal
    //!
    //! ### Example
    //! @include example/core/when.cpp
    template <bool condition>
    struct when;

    //! @ingroup group-core
    //! Used in conjunction with `when` to enable a type class instance or a
    //! data type specialization only if an expression is well-formed.
    //!
    //! Specifically, `is_valid<...>` is always equivalent to the constant
    //! expression `true`. When used as `when<is_valid<...>>` inside a type
    //! class or data type specialization, SFINAE will cause the partial
    //! specialization to fail when the expression is ill-formed.
    //!
    //! ### Example
    //! @include example/core/is_valid.cpp
    template <typename ...T>
    //constexpr bool is_valid = true;
    constexpr bool is_valid() { return  true; };

}
}

#endif // header
