//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FUCTIONAL_V0_OVERLOAD_HPP
#define FUCTIONAL_V0_OVERLOAD_HPP

#include <functional/v0/config.hpp>
#include <utility>
#include <type_traits>

namespace functional
{
  CONFIG_INLINE_NAMESPACE(v0)
  {

    namespace detail
    {
      using namespace std;

      template <class... Fs>
      struct overloader;

      template <class F>
      struct overloader<F> : F
      {
        using F::operator();
        overloader(F fct) : F(move(fct))
        {}
      };

      template <class F, class... Fs>
      struct overloader<F, Fs...>
      : F, overloader<Fs...>
      {
        using base_type = overloader<Fs...>;
        using F::operator();
        using base_type::operator();
        overloader(F fct, Fs... fcts)
        : F(move(fct)),
        base_type(move(fcts)...)
        {}
      };

      ///
      template <class R, class... Fs>
      struct overloader_ret : overloader<Fs ...>
      {
        using result_type = R;
        overloader_ret(Fs... fcts) : overloader<Fs ...>(std::forward<Fs>(fcts)...) {}
      };

      ///
//      template <class R, class... Fs>
//      struct overloader;
//
//      template <class R, class F>
//      struct overloader<R, F> : F
//      {
//        using result_type = R;
//        using F::operator();
//        overloader(F fct) : F(move(fct))
//        {}
//      };
//
//      template <class R, class F, class... Fs>
//      struct overloader<R, F, Fs...>
//      : F, overloader<R, Fs...>
//      {
//        using base_type = overloader<R, Fs...>;
//        using F::operator();
//        using base_type::operator();
//        overloader(F fct, Fs... fcts)
//        : F(move(fct)),
//        base_type(move(fcts)...)
//        {}
//      };

    } // detail

    template <class ... Fs>
    auto overload(Fs &&... fcts)
    {
      return detail::overloader<std::decay_t<Fs>...>(std::forward<Fs>(fcts)...);
    }

    template <class R, class ... Fs>
    auto explicit_overload(Fs &&... fcts)
    {
      return detail::overloader_ret<R, std::decay_t<Fs>...>(std::forward<Fs>(fcts)...);
    }

  } // v0
} // functional

#endif // header
