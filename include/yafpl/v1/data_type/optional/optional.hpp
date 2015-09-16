//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
//
//  maybe<int> x = nothing;
//  maybe<int> x = just{1};
//
//  maybe<void> x = nothing;
//  maybe<void> x = just{};


#ifndef YAFPL_V1_DATA_TYPE_OPTIONAL_OPTIONAL_HPP
#define YAFPL_V1_DATA_TYPE_OPTIONAL_OPTIONAL_HPP

#include <yafpl/v1/config.hpp>
#include <optional.hpp>
#include <yafpl/v1/meta/id.hpp>

#include <yafpl/v1/type_class/sum_type/sum_type_alternatives.hpp>
#include <yafpl/v1/type_class/monad/monad.hpp>

#include <utility>

namespace std
{
  namespace experimental
  {

#if ! defined YAFPL_X1
    template <class R, class T, class F>
    R match(meta::id<R>, optional<T> const& x, F&& f)
    {
      if (x)
          return f(*x);
        else
          return f(nullopt);
    }
#else
    template <class T, class F>
    auto match(optional<T> const& x, F&& f)
    {
      if (x)
          return f(*x);
        else
          return f(nullopt);
    }
#endif

  }
}

namespace yafpl
{
  template <class T>
  struct sum_type_alternatives<std::experimental::optional<T>>: meta::identity<meta::types<std::experimental::nullopt_t, T>>
  {
  };
}

#endif // header
