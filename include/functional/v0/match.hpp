//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FUCTIONAL_V0_MATCH_HPP
#define FUCTIONAL_V0_MATCH_HPP

#include <functional/v0/config.hpp>

#include <functional/v0/apply.hpp>
#include <meta/v0/identity.hpp>
#include <meta/v0/none.hpp>
#include <meta/v0/types.hpp>

#include <utility>
#include <type_traits>
#include <tuple>

namespace functional
{

  CONFIG_INLINE_NAMESPACE(v0)
  {

    using meta::types;
    using meta::type;

    template <class ST>
    struct sum_types: meta::identity<typename ST::types>
    {
    };

    template <class ST>
    using sum_types_t = typename sum_types<ST>::type;

    namespace detail
    {
      using namespace std;

      template <class Final, class Result, class Functor, class DataTypeTuple, class STs, class Types>
      class applier;

      template <class Final, class R, class F, class ... DTs, class ... STs>
      class applier<Final, R, F, tuple<DTs const&...>, tuple<STs const&...>, meta::types<> >
      {
      public:

        applier(R *r, F &fct, tuple<DTs const&...> &&members,
        tuple<STs const&...> &&sums)
        : fct(fct),
          r(r),
          members(move(members)),
          sums(move(sums))
        {}

        F &fct;
        R *r;
        tuple<DTs const&...> members;
        tuple<STs const&...> sums;

        void operator()() const
        {
          fct();
        }
      };

      template <class Final, class R, class F, class... DTs, class... STs, class T, class... Ts>
      class applier<Final, R, F, tuple<DTs const&...>, tuple<STs const&...>, types<T, Ts...>>
      : public applier<Final, R, F, tuple<DTs const&...>, tuple<STs const&...>, types<Ts...>>
      {
      public:

        using super = applier<Final, R, F, tuple<DTs const&...>, tuple<STs const&...>, types<Ts...>>;
        /* Pass everything up to the base case. */
        applier(R *r, F &fct, tuple<DTs const&...> &&members, tuple<STs const&...> &&sums)
          : super(r, fct, move(members), move(sums))
        {}

        using super::operator();
        void operator()(T const&v) const
        {
          dispatch(index_sequence_for<DTs...>(), v, index_sequence_for<STs...>());
        }

      private:
        template <class... ODTs>
        void dispatch_helper(
            tuple<ODTs const&...> &&odts) const
        {
          overload(
              [&](auto *r)
              {
                *r = functional::apply(this->fct, move(odts));
              },
              [&](void *)
              {
                functional::apply(this->fct, move(odts));
              }
          )(this->r);
        }

        template <class... ODTs, class ST, class... OSTs>
        void dispatch_helper(tuple<ODTs const&...> &&odts, ST const& sum, OSTs const&... osts) const
        {
          struct applier_type : applier<applier_type, R, F, tuple<ODTs const&...>, tuple<OSTs const&...>, sum_types_t<ST> >
          {
            using super = applier<applier_type, R, F, tuple<ODTs const&...>, tuple<OSTs const&...>, sum_types_t<ST>>;
            using super::super;
          };

          // customization point
          match(type<R>{}, type<ST>{}, sum, applier_type(this->r, this->fct, move(odts), forward_as_tuple(osts...)));
        }

        template <size_t... i, size_t... j, size_t... k>
        void dispatch(index_sequence<i...>, T const& v, index_sequence<j...>) const
        {
          dispatch_helper(forward_as_tuple(get<i>(this->members)..., v), get<j>(this->sums)...);
        }
      };

      template <class R>
      struct storage
      {
        R get()
        { return move(r);}
        R *ptr()
        { return &r;}
        R r;
      };

      template <>
      struct storage<void>
      {
        void get() &&
        {}
        void *ptr()
        { return nullptr; }
      };

      template <class R, class F, class ST, class... STs>
      decltype(auto) apply(F &&fct, ST const& sum, STs const&... osts)
      {
        struct applier_type : applier<applier_type, R, F, tuple<>, tuple<STs const&...>, sum_types_t<ST> >
        {
          using super = applier<applier_type, R, F, tuple<>, tuple<STs const&...>, sum_types_t<ST> >;
          using super::super;
        };

        storage<R> r;
        // customization point
        match(type<R>{}, type<ST>{}, sum, applier_type(r.ptr(), fct, forward_as_tuple(), forward_as_tuple(osts...)));
        return move(r).get();
      }

    } // detail

    template <class R, class ST, class... Fs>
    decltype(auto) match(ST const& that, Fs &&... fcts)
    {
      using namespace std;

      return detail::apply<R>(overload(forward<Fs>(fcts)...), that);
    }

    template <class R, class... STs, class... Fs>
    decltype(auto) match_all(std::tuple<STs...> const& those, Fs &&... fcts)
    {
      using namespace std;

      return functional::apply(
          [&](auto && ... args) -> decltype(auto)
          {
            return detail::apply<R>(overload(forward<Fs>(fcts)...), forward<decltype(args)>(args)...);
          },
          those);
    }

  } // v0
} // functional

#endif // header
