//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_VOID_HPP
#define YAFPL_V1_META_VOID_HPP

#include <yafpl/v1/meta/always.hpp>
#include <yafpl/v1/meta/apply.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  namespace meta
  {

    /// \brief An alias for `void`.
    template<typename...Ts>
    //using void_ = void;
    using void_ = apply<always<void>, Ts...>;

  }
}
}

#endif // header
