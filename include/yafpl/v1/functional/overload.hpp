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
      using namespace std;

      template <class F>
      struct forwarder : F
      {
        using F::operator();
        constexpr forwarder(F fct) : F(move(fct))
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
        constexpr explicit_forwarder(F fct) : F(move(fct))
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
        using base = forwarder<decltype(std::mem_fn(std::declval<R(O::*)(X...)>()))>;
        using type = R(O::*)(X...);

        constexpr explicit_forwarder(type f) : base(std::mem_fn(f)) { }
        using base::operator();
      };

      template <class F, class G>
      struct overloader
      : forwarder<F>, forwarder<G>
      {
        using forwarder<F>::operator();
        using forwarder<G>::operator();

        template <class F1, class G1>
        constexpr overloader(F1&& f, G1&& g)
        : forwarder<F>(move(f)),
          forwarder<G>(move(g))
        {}
      };

      ///
      template <class R, class F, class G>
      struct explicit_overloader : overloader<F, G>
      {
        using result_type = R;
        using overloader<F, G>::operator();

        template <class F1, class G1>
        explicit_overloader(F1&& f, G1&& g) : overloader<F, G>(std::forward<F>(f), std::forward<G>(g)) {}
      };

    } // detail

    template <class F>
    auto overload(F && f)
    {
      return std::forward<F>(f);
    }

    template <class F1, class F2, class ... Fs, class F12= detail::overloader<std::decay_t<F1>,std::decay_t<F2>> >
    auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return overload(F12(std::forward<F1>(f1), std::forward<F2>(f2)),
                      std::forward<Fs>(fcts)...);
    }

    template <class R, class F>
    auto overload(F && f)
    {
      return detail::explicit_forwarder<R, F>(std::forward<F>(f));
    }

    template <class R, class F1, class F2, class ... Fs, class F12= detail::explicit_overloader<R, std::decay_t<F1>,std::decay_t<F2>> >
    auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return overload<R>(F12(std::forward<F1>(f1), std::forward<F2>(f2)),
                      std::forward<Fs>(fcts)...);
    }

  } // version
} // yafpl

#endif // header
