//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_APPLY_HPP
#define YAFPL_V1_META_APPLY_HPP

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
namespace meta
{
  /// applies a meta-function \p TC to the arguments \p Args
  ///
  /// apply<type_constructor_t<M>, value_type_t<M>> == M
  /// value_type_t<apply<TC, T>> == T
  /// type_constructor_t<apply<TC, T>> == TC

  template<class TC, class... Args>
  using apply = typename TC::template apply<Args...>;

}
}
}

#endif // header
