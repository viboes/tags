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


#ifndef DATA_TYPE_V0_MAYBE_HPP
#define DATA_TYPE_V0_MAYBE_HPP

#include <data_type/v0/config.hpp>
#include <data_type/v0/match_variant.hpp>
#include <functional/v0/pattern.hpp>

namespace data_type {
  CONFIG_INLINE_NAMESPACE(v0) {

    FUNCTIONAL_V0_NEWTYPE(nothing);
    FUNCTIONAL_V0_NEWTYPE_1(just);

    template <class T>
    using maybe = ::boost::variant< nothing_t, just_t<T> >;

  }
}

namespace functional
{
    template <class T>
    struct sum_types<data_type::maybe<T>> : meta::identity<meta::types<data_type::nothing_t, data_type::just_t<T>>> {};
};

#endif // header
