//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_FUCTIONAL_V0_MATCH_HPP
#define YAFPL_FUCTIONAL_V0_MATCH_HPP

#include <yafpl/v1/config.hpp>

#include <yafpl/v1/functional/apply.hpp>
#include <yafpl/v1/functional/overload.hpp>
#include <yafpl/v1/meta/identity.hpp>
#include <yafpl/v1/meta/id.hpp>
#include <yafpl/v1/meta/types.hpp>
#include <yafpl/v1/meta/void_.hpp>
#include <yafpl/v1/type_class/sum_type/sum_type_alternatives.hpp>

#include <utility>
#include <type_traits>
#include <tuple>

namespace yafpl
{

  YAFPL_INLINE_NAMESPACE(v1)
  {

    using meta::types;
    using meta::id;

#if ! defined YAFPL_X1
      template <class R, class T, class F>
      R match(yafpl::meta::id<R>, T const& x, F&& f)
      {
        return f(x);
      }
#else
    template <class T, class F>
    auto match(T const& x, F&& f) -> decltype(f(x))
    {
      return f(x);
    }
#endif

    namespace detail
    {
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
          members(std::move(members)),
          sums(std::move(sums))
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
          : super(r, fct, std::move(members), std::move(sums))
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
          overload<R>(
              [&](auto *r)
              {
                *r = apply(this->fct, std::move(odts));
              },
              [&](void *)
              {
                apply(this->fct, std::move(odts));
              }
          )(this->r);
        }

        template <class... ODTs, class ST, class... OSTs>
        void dispatch_helper(tuple<ODTs const&...> &&odts, ST const& sum, OSTs const&... osts) const
        {
          struct applier_type : applier<applier_type, R, F, tuple<ODTs const&...>, tuple<OSTs const&...>, sum_type_alternatives_t<ST> >
          {
            using super = applier<applier_type, R, F, tuple<ODTs const&...>, tuple<OSTs const&...>, sum_type_alternatives_t<ST>>;
            using super::super;
          };

          // customization point
#if ! defined YAFPL_X1
          match(id<R>{}, sum, applier_type(this->r, this->fct, std::move(odts), std::forward_as_tuple(osts...)));
#else
          match(sum, applier_type(this->r, this->fct, std::move(odts), std::forward_as_tuple(osts...)));
#endif
        }

        template <size_t... i, size_t... j, size_t... k>
        void dispatch(index_sequence<i...>, T const& v, index_sequence<j...>) const
        {
          dispatch_helper(std::forward_as_tuple(get<i>(this->members)..., v), get<j>(this->sums)...);
        }
      };

      template <class R>
      struct storage
      {
        R get()
        { return std::move(r);}
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
      decltype(auto) apply_impl(F &&fct, ST const& sum, STs const&... osts)
      {
        struct applier_type : applier<applier_type, R, F, tuple<>, tuple<STs const&...>, sum_type_alternatives_t<ST> >
        {
          using super = applier<applier_type, R, F, tuple<>, tuple<STs const&...>, sum_type_alternatives_t<ST> >;
          using super::super;
        };

        storage<R> r;
        // customization point
#if ! defined YAFPL_X1
        match(id<R>{}, sum, applier_type(r.ptr(), fct, std::forward_as_tuple(), std::forward_as_tuple(osts...)));
#else
        match(sum, applier_type(r.ptr(), fct, std::forward_as_tuple(), std::forward_as_tuple(osts...)));
#endif
        return std::move(r).get();
      }

      template <class T>
      struct is_tuple : std::false_type {};
      template <class T>
      struct is_tuple<T const> : is_tuple<T> {};
      template <class T>
      struct is_tuple<T volatile> : is_tuple<T> {};
      template <class T>
      struct is_tuple<T const volatile> : is_tuple<T> {};
      template <class ...Ts>
      struct is_tuple<std::tuple<Ts...>> : std::true_type {};

      template< class, class = void >
      struct has_result_type_member : std::false_type { };
      template< class T >
      struct has_result_type_member<T, meta::void_<typename T::result_type>> : std::true_type { };

      template< class T1, class T2 >
      struct and_;
      template< class T >
      struct and_<std::true_type, T> : T {};
      template< class T >
      struct and_<std::false_type, T> : std::false_type {};

      template< class T >
      struct have_result_type_member_x;
      template< class T >
      struct have_result_type_member_x<types<T>> : has_result_type_member<T> {};
      template< class T1, class T2, class ...Ts >
      struct have_result_type_member_x<types<T1, T2, Ts...>>
        : and_<
            has_result_type_member<T1>,
            have_result_type_member_x<types<T2, Ts...>>
          >
      {};

      template< class ...Ts >
      struct have_result_type_member : have_result_type_member_x<types<Ts...>> { };

      template <class F, class ...Fs>
      struct deduced_result_type {
        using type =  typename F::result_type;
      };
      template <class ...Fs>
      using deduced_result_type_t = typename deduced_result_type<Fs...>::type;
    } // detail

    // explicit result type
    template <class R, class ST, class F1, class F2, class... Fs, typename std::enable_if<
      ! detail::is_tuple<ST>::value, int>::type = 0>
    decltype(auto) match(ST const& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
#if ! defined YAFPL_X1
      return match(id<R>{}, that, overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...));
#else
      return match(that, overload(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...));
#endif
    }
    template <class R, class ST, class F1, class... Fs, typename std::enable_if<
      ! detail::is_tuple<ST>::value, int>::type = 0>
    decltype(auto) match(ST & that, F1 && f1, Fs &&... fs)
    {
#if ! defined YAFPL_X1
      return match(id<R>{}, that, overload(std::forward<F1>(f1), std::forward<Fs>(fs)...));
#else
      return match(that, overload(std::forward<F1>(f1), std::forward<Fs>(fs)...));
#endif
    }

    // explicit result type on a product of sum types
    template <class R, class... STs, class... Fs>
    decltype(auto) match(std::tuple<STs...> const& those, Fs &&... fcts)
    {
      return apply(
          [&](auto && ... args) -> decltype(auto)
          {
            return detail::apply_impl<R>(overload(std::forward<Fs>(fcts)...), std::forward<decltype(args)>(args)...);
          },
          those);
    }

    // result type deduced the nested typedef result_type of all functions
    template <class ST, class F1, class F2, class... Fs, typename std::enable_if<
      ! detail::is_tuple<ST>::value
      && detail::have_result_type_member<F1>::value, int>::type = 0>
    decltype(auto) match(ST const& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
      using R = detail::deduced_result_type_t<std::decay_t<F1>, std::decay_t<F2>, std::decay_t<Fs>...>;
      return match<R>(that, std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...);
    }
    template <class ST, class F1, class F2, class... Fs, typename std::enable_if<
      ! detail::is_tuple<ST>::value
      && detail::have_result_type_member<F1>::value, int>::type = 0>
    decltype(auto) match(ST& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
      using R = detail::deduced_result_type_t<std::decay_t<F1>, std::decay_t<F2>, std::decay_t<Fs>...>;
      return match<R>(that, std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Fs>(fs)...);
    }

    // result type deduced the nested typedef result_type of all functions on a product of sum types
    template <class... STs, class F, class... Fs, typename std::enable_if<
      detail::have_result_type_member<F>::value, int>::type = 0>
    decltype(auto) match(std::tuple<STs...> const& those, F && f, Fs &&... fs)
    {
      using R = detail::deduced_result_type_t<std::decay_t<F>, std::decay_t<Fs>...>;
      return match<R>(those, std::forward<F>(f), std::forward<Fs>(fs)...);
    }

  } // version
} // yafpl

#endif // header
