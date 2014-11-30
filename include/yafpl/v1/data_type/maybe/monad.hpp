//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
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


#ifndef YAFPL_V1_DATA_TYPE_MAYBE_MONAD_HPP
#define YAFPL_V1_DATA_TYPE_MAYBE_MONAD_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/data_type/maybe/maybe.hpp>
#include <yafpl/v1/type_class/monad/monad.hpp>
#include <yafpl/v1/meta/type.hpp>

#include <utility>

// maybe monad instance
namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  // mreturn : T -> maybe<U>
  // Make use of impicit construction from mreturn_t<T>
  //
  // mbind : maybe<T> -> (T -> maybe<U>) -> maybe<U>
  // The definition using pattern matching should be no the more efficient. Just presented here as an example.
  template <class T, class F>
    auto mbind(meta::type<maybe<T>>, maybe<T> const& x, F&& fct)
  {
    using R = decltype(fct(std::declval<T>()));
    return match<R>(x,
        [fct](just_t<T> a) { return fct(a.value); },
        [](nothing_t) { return nothing; }
    );
  }
}
}

#endif // header
