//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////


#ifndef YAFPL_V1_TYPE_CLASS_MONAD_MONAD_HPP
#define YAFPL_V1_TYPE_CLASS_MONAD_MONAD_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/functional/pattern.hpp>
#include <yafpl/v1/core/datatype.hpp>
#include <yafpl/v1/core/typeclass.hpp>
#include <yafpl/v1/meta/type.hpp>

#include <utility>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{
  //! @ingroup group-typeclasses
  //! `Monad`s are `Applicative`s with the ability to flatten values that
  //! were lifted more than once.
  //!
  //! ### Requires
  //! `Functor`, `Applicative`
  //!
  //! ### Laws
  //! Instances of `Monad` must satisfy the following laws:
  //! @code
  //!     bind(lift<M>(x), f) == f(x)
  //!     bind(m, lift<M>) == m
  //!     bind(m, [](auto x){ return bind(f(x), g); }) == bind(bind(m, f), g)
  //! @endcode
  struct Monad {
    YAFPL_TYPECLASS(Monad);

  };

  namespace monad
  {

    // mreturn : T -> M<T>
    // I don't know how this function can be implemented for each instance of a monad.
    // We need the context of the monad M<>. that means that we should use it as e.g.
    //   mreturn<maybe>(a)
    //
    // An alternative could be to define mreturn as
    //   mreturn : T -> mreturn_t<T>
    //
    // and require that the monad must define the implicit construction from this boxed value
    //   M<T>(mreturn_t<T> x);
    //
    // E.g. for maybe<T>, it can be defined as
    //   maybe<T>(just(x.value))
    //
    // That has the disadvantage that maybe cannot be defined directly as a sum type
    //   using maybe = ::boost::variant< nothing_t, just_t<T> >;
    // We need to wrap ::boost::variant< nothing_t, just_t<T> > so that we are able to add this
    // implicit constructor from box<T>.
    //
    // This has an additional disadvantage, that is, that the type maybe depends on monad :(

    YAFPL_NEWTYPE_1(mreturn);

    // mbind : M<T> -> (T -> M<U>) -> M<U>
    template <class M, class F>
      auto mbind(M const& x, F&& fct)
    {
      return mbind(meta::type<M>{}, x, std::forward<F>(fct));

    }

    //! Apply a function returning a Monad to the value(s) inside a Monad.
    //! @relates Monad
    //!
    //! @param m
    //! A `Monad` to compose monadically with the given function.
    //!
    //! @param f
    //! A function taking a value inside the `m` and returning a `Monad`.
    //! It will be called as `f(x)`, where `x` is the same argument as `f`
    //! would be called with when doing `monad::bind(m, f)`.
    //!
    //!
    //! ### Example
    //! @include example/monad/bind.cpp


    //    ../include/yafpl/v1/type_class/monad/monad.hpp:92:27: error: constexpr variable 'bind' must be initialized by a constant expression
    //        constexpr auto bind = [](auto&& m, auto&& f) -> decltype(auto) {
    #if defined __GNUC__ and ! defined __clang__
    constexpr auto bind = [](auto&& m, auto&& f) -> decltype(auto) {
        return Monad::instance<
            datatype_t<decltype(m)>
        >::bind_impl(
            std::forward<decltype(m)>(m),
            std::forward<decltype(f)>(f)
        );
    };
#endif
  }
}
}

#endif // header
