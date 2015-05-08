/*!
@file
Defines `yafpl::disable`.

Copied from Boost.Hana
@copyright Louis Dionne 2014
@copyright Vicente J. Botet Escriba 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef YAFPL_V1_CORE_DISABLE_HPP
#define YAFPL_V1_CORE_DISABLE_HPP

#include <yafpl/v1/config.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{

  //! @ingroup group-core
  //! Explicitly disable a type class instance.
  //!
  //! This is meant as a way to disable a type class instance provided
  //! through a predicate which, for some reason, is not desirable for
  //! a given data type.
  //!
  //! ### Example
  //! @include example/core/disable.cpp
  struct disable { };

}
} // yafpl

#endif // heade
