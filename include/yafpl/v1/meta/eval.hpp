//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_V1_META_EVAL_HPP
#define YAFPL_V1_META_EVAL_HPP

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
namespace meta
{
  /// evaluates a meta-expression \p T by returning the nested \c T::type
  template <class T>
  using eval = typename T::type;

}
}
}

#endif // header
