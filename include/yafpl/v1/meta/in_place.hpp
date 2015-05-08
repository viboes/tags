//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_IN_PLACE_HPP
#define YAFPL_V1_META_IN_PLACE_HPP

#include <yafpl/v1/config.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  namespace meta
  {
    constexpr struct in_place_t{} in_place{};
  }
}
}
#endif // header
