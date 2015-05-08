// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef YAFPL_V1_TYPE_CLASS_APPLICATIVE_MAKE_HPP
#define YAFPL_V1_TYPE_CLASS_APPLICATIVE_MAKE_HPP

#include <yafpl/v1/config.hpp>
#include <yafpl/v1/meta/in_place.hpp>
#include <yafpl/v1/meta/type_constructor.hpp>
#include <yafpl/v1/meta/underlying_type.hpp>

#include <utility>
#include <type_traits>

namespace yafpl
{
YAFPL_INLINE_NAMESPACE(v1)
{

  namespace type_class
  {
    struct functor {
      template <class F>
      struct instance {};
    };
    struct applicative {
      template <class F>
      struct instance {};
    };
    struct monad {
      template <class M>
      struct instance {};
    };
  }

  namespace functor
  {
    // Minimal definition set
    // fmap:: A->B -> F<A> -> F<B>

    // fmap:: A->B, F<A> -> F<B>
    template <class A_B, class F_A, class A = meta::underlying_type_t<std::decay_t<F_A>> >
    auto fmap(A_B&& f, F_A&& f_a) -> meta::rebind<F_A, decltype(f(declval<A>()))>
    {
      using namespace functor_custom;
      return fmap(type_class::functor::template instance<F_A>{}, std::forward<A_B>(f), std::forward<F_A>(f_a));
    }

//    // forget:: Functor F => F<A> -> F<void>
//    template <class F_A>
//    auto forget(F_A&& f_a) -> meta::rebind<F_A, void>
//    {
//      return functor::fmap(functional::constant<void>(), m_a);
//    }
  }
  namespace functor_custom
  {
    // forward customization
    template <class A_B, class F_A>
    auto fmap(type_class::functor<F_A>, A_B&& f, F_A&& f_a)
    // requires fa.fmap(std::forward<A_B>(f)) is well defined
    {
      return fa.fmap(std::forward<A_B>(f));
    }
  }

  namespace applicative
  {
    using namespace functor;

    // Minimal complete definition
    // make, apply

    // make()
    template <template <class ...> class M>
    M<void> make()
    {
      using namespace applicative_custom;
      return make(type_class::applicative<M<void>>{});
    }

    // make overload: requires a template class, deduce the underlying type
    template <template <class ...> class M, int = 0, int..., class X>
    M<std::decay_t<X>>
    make(X&& x)
    {
      using namespace applicative_custom;
      return make(type_class::applicative<M<std::decay_t<X>>>{}, std::forward<X>(x));
    }

    // make overload: requires a type construcor, deduce the underlying type
    template <class TC, int = 0, int..., class X>
    typename std::enable_if<meta::is_type_constructor<TC>::value,
      apply<TC, std::decay_t<X>>
    >::type
    make(X&& x)
    {
      using namespace applicative_custom;
      return make(type_class::applicative<apply<TC, std::decay_t<X>>>{}, std::forward<X>(x));
    }

    // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
    template <class M, int = 0, int..., class X>
    typename std::enable_if<   ! meta::is_type_constructor<M>::value
                       //&&   is_same<X, meta::underlying_type_t<M>>::value
    , M
    >::type
    make(X&& x)
    {
      using namespace applicative_custom;
      return make(type_class::applicative<M>{}, std::forward<X>(x));
    }

    // make emplace overload: requires a type with a specific underlying type, don't deduce the underlying type from X
    template <class M, class ...Args>
    M make(Args&& ...args)
    {
      using namespace applicative_custom;
      return make(type_class::applicative<M>{}, meta::in_place, std::forward<Args>(args)...);
    }

    // apply:: F<A -> B>, F<A> -> F<B>
    template <class F_A_to_B, class F_A,
              class A_to_B =  meta::underlying_type_t<std::decay_t<F_A_to_B>>,
              class A =       meta::underlying_type_t<std::decay_t<F_A>> >
    auto apply(F_A_to_B&& f_ab, F_A && f_a) -> rebind<F_A, decltype(declval<A_to_B>(declval<A>()))>
    {
      using namespace applicative_custom;
      return apply(type_class::applicative<F_A>{}, std::forward<F_A_to_B>(f_ab), std::forward<F_A>(f_a));
    }

    // lift :: Applicative F -> (A... -> B) -> F<A>... -> F<B>
  }
  namespace applicative_custom
  {
    // default customization point for TC<void> default constructor
    template <class M>
    M make(type_class::applicative<M>)
    // requires is_default_constructible<M> and is_same<void, meta::underlying_type_t<M>>
    {
      return M();
    }

    // default customization point for constructor from X
    template <class M,   class X>
    M make(type_class::applicative<M>,
        X&& x)
    // requires is_constructible<M, decat_t<X>>
    {
      return M(std::forward<X>(x));
    }

    // default customization point for in_place constructor
    template <class M, class ...Args>
    M make(type_class::applicative<M>,
        meta::in_place_t, Args&& ...args)
    // requires is_constructible<M, inplace_t, decay_t<Args>...>
    {
      return M(meta::in_place, std::forward<Args>(args)...);
    }

    template <class F_A_to_B, class F_A,
              class A_to_B =  meta::underlying_type_t<std::decay_t<F_A_to_B>>,
              class A =       meta::underlying_type_t<std::decay_t<F_A>> >
    auto apply(type_class::applicative<F_A>,
        F_A_to_B&& f_ab, F_A && f_a)
    // requires fa.apply(std::forward<F_A_to_B>(f_ab)) is well defined
    {
      return f_a.apply(std::forward<F_A_to_B>(f_ab));
    }
  }
  namespace monad
  {
    using namespace functor;

    // M<A>, A -> M<B> -> M<B>
    template <class M_A, class A_to_M_B, class A = meta::underlying_type_t<std::decay_t<M_A>> >
    auto bind(M_A&& m_a, A_to_M_B&& f_a_mb) -> decltype(f(declval<A>()))>
    {
      using namespace functor_custom;
      return bind(type_class::monad<M_A>{}, std::forward<F_A>(m_a), std::forward<A_to_M_B>(f_a_mb));
    }

  }
  namespace monad_custom
  {
    // M<A>, A -> M<B> -> M<B>
    template <class M_A, class A_to_M_B, class A = meta::underlying_type_t<std::decay_t<M_A>> >
    auto bind(type_class::monad<M_A>,
        M_A&& m_a, A_to_M_B&& f_a_mb) -> decltype(f(declval<A>()))>
    {
      return m_a.bind(std::forward<A_to_M_B>(f_a_mb));
    }
  }
}
}
}

#endif // header
