//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_ALWAYS_HPP
#define YAFPL_V1_META_ALWAYS_HPP

#include <yafpl/v1/meta/eval.hpp>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
namespace meta
{
  /// \brief A Metafunction Class that always returns \c T.
  template<typename T>
  struct always
  {
  private:
    // Redirect through a class template for compilers that have not
    // yet implemented CWG 1558:
    // <http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558>
    template<typename...>
    struct impl
    {
      using type = T;
    };
  public:
    template<typename...Ts>
    using apply = eval<impl<Ts...>>;
  };

}
}
}


#endif // header
