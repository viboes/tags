//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_FUCTIONAL_V1_OVERLOAD_HPP
#define YAFPL_FUCTIONAL_V1_OVERLOAD_HPP

#include <yafpl/v1/config.hpp>

#include <functional>
#include <type_traits>
#include <utility>

namespace yafpl
{
  YAFPL_INLINE_NAMESPACE(v1)
  {

    namespace detail
    {
      template <class F>
      struct forwarder : F
      {
        using F::operator();
        constexpr forwarder(F fct) : F(std::move(fct))
        {}
      };

      template< class R, class ...X >
      struct forwarder<R(*)(X...)> {
          using type = R(*)(X...);
          type f;

          constexpr forwarder(type f) : f(f) { }

          constexpr R operator () (X &&... x) const {
              return f(std::forward<X>(x)...);
          }
      };

      template<class R, class O, class...X>
      struct forwarder<R(O::*)(X...)> : forwarder<decltype(std::mem_fn(std::declval<R(O::*)(X...)>()))> {
        using base = forwarder<decltype(std::mem_fn(std::declval<R(O::*)(X...)>()))>;
        using type = R(O::*)(X...);

        constexpr forwarder(type f) : base(std::mem_fn(f)) { }
        using base::operator();
      };

      template <class R, class F>
      struct explicit_forwarder : F
      {
        using result_type = R;
        using F::operator();
        constexpr explicit_forwarder(F fct) : F(std::move(fct))
        {}
      };

      template< class R, class ...X >
      struct explicit_forwarder<R, R(X...)> {
          using result_type = R;
          using type = R(*)(X...);
          type f;

          constexpr explicit_forwarder(type f) : f(f) { }

          constexpr R operator () (X &&... x) const {
              return f(std::forward<X>(x)...);
          }
      };

      template<class R, class O, class...X>
      struct explicit_forwarder<R, R(O::*)(X...)> : explicit_forwarder<R, decltype(std::mem_fn(std::declval<R(O::*)(X...)>()))> {
        using base = explicit_forwarder<R,decltype(std::mem_fn(std::declval<R(O::*)(X...)>()))>;
        using type = R(O::*)(X...);

        constexpr explicit_forwarder(type f) : base(std::mem_fn(f)) { }
        using base::operator();
      };

      template <class F, class ...Fs>
      struct overloader;

      template <class F>
      struct overloader<F> : forwarder<F>
      {
        template <class OF>
        constexpr overloader(OF&& of)
        : forwarder<F>(std::forward<OF>(of))
        {}

        using forwarder<F>::operator();
      };

      template <class F1, class F2, class ...Fs>
      struct overloader<F1, F2, Fs...>
      : forwarder<F1>, overloader<F2, Fs...>
      {
        template <class OF1, class OF2, class ...OFs>
        constexpr overloader(OF1&& of1, OF2&& of2, OFs&& ...ofs)
        : forwarder<F1>(std::forward<OF1>(of1)), overloader<F2, Fs...>(std::forward<OF2>(of2), std::forward<OFs>(ofs)...)
        {}

        using forwarder<F1>::operator();
        using overloader<F2, Fs...>::operator();
      };

      ///
      template <class R, class ...Fs>
      struct explicit_overloader : overloader<Fs...>
      {
        using result_type = R;
        using overloader<Fs...>::operator();

        template <class ...OFs>
        constexpr explicit_overloader(OFs&& ...fs) : overloader<Fs...>(std::forward<OFs>(fs)...) {}
      };

    } // detail

    template <class F>
    constexpr auto overload(F && f) noexcept(noexcept(std::forward<F>(f)))
    {
      return std::forward<F>(f);
    }

    template <class F1, class F2, class ... Fs>
    constexpr auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return detail::overloader<std::decay_t<F1>,std::decay_t<F2>, std::decay_t<Fs>...>(
          std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fcts)...);
    }

    template <class R, class F>
    constexpr auto overload(F && f) noexcept(noexcept( detail::explicit_forwarder<R, F>(std::forward<F>(f)) ))
    {
      return detail::explicit_forwarder<R, F>(std::forward<F>(f));
    }

    template <class R, class F1, class F2, class ... Fs>
    constexpr auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return detail::explicit_overloader<R, std::decay_t<F1>,std::decay_t<F2>, std::decay_t<Fs>...>(
          std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fcts)...);
    }

  } // version
} // yafpl

#endif // header
