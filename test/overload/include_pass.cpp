//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <yafpl/v1/functional/overload.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <boost/detail/lightweight_test.hpp>

struct X {
  X& f(char) {
    std::cout << "X::f" << std::endl;
    return *this;
  }
  bool operator==(X const&) { return true; }
};
struct Y {
};


struct function_with_state {
  void operator ()(int arg) { invoked = true; }
  bool invoked = false;
};
struct function_without_state {
  constexpr void operator ()(int arg) const noexcept { }
};

struct function_without_state2 {
  constexpr int operator ()(int arg) const noexcept { return arg;}
};
struct function_without_state3 {
  int operator ()(int arg) const noexcept { return arg;}
};

int nonMember( float ) {
  std::cout << "int(float)" << std::endl;
  return 42;
}


namespace cppljevans {
  //Purpose:
  //  See if can multi-inherit classes
  //  with member function with same
  //  name but different signature.
  //
  //Result:
  //  Works.
  //
  //===========
  template<int I>
  struct udt_i
  /**@brief
   *  User Defined Type.
   */
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream& os
      , udt_i const&
      )
      { return os<<"udt_i<"<<I<<">";
      }
  };
  template<int I>
  struct functor_i
  /**@brief
   *  User Defined Functor.
   */
  {
    using arg_t=udt_i<I>;
    void operator()(arg_t const&a)
    { std::cout<<"functor_i<"<<I<<">::operator()("<<a<<" const&).\n";
    }
  };
  template<typename Functor>
  struct forwarder
  /**@brief
   *  this->operator() forwards to Functor::operator()
   */
  {
    Functor functor_v;
    forwarder(Functor const& functor_a)
      : functor_v(functor_a)
    {}
    using arg_t=typename Functor::arg_t;
    void operator()(arg_t const&arg_v)
    { functor_v(arg_v);
    }
  };
  #ifdef __clang__
  template<typename... Functors>
  struct overloader
  /**@brief
   *  "Mixin" all the Functors::operator()'s
   *  into this class.
   */
  : public forwarder<Functors>...
  {
    overloader(Functors&&... functors_a)
    : forwarder<Functors>(functors_a)...
    {}
  };
  #else
  template<typename... Functors>
  struct overloader
  /**@brief
   *  "Mixin" all the Functors::operator()'s
   *  into this class.
   */
    ;
  template<>
  struct overloader<>
  {
    void operator()()
    {
    }
  };
  template<typename Functor0, typename... Functors>
  struct overloader<Functor0, Functors...>
  : public forwarder<Functor0>, overloader<Functors...>
  {
    overloader(Functor0&& functor0, Functors&&... functors_a)
    : forwarder<Functor0>(std::forward<Functor0>(functor0))
    , overloader<Functors...>(std::forward<Functors>(functors_a)...)
    {}
    using forwarder<Functor0>::operator();
    using overloader<Functors...>::operator();
  };
#endif
}
int main()
{
  {
    using namespace cppljevans;

    overloader
    < functor_i<0>
    , functor_i<1>
    , functor_i<2>
    >
  ovldr
    ( functor_i<0>{}
    , functor_i<1>{}
    , functor_i<2>{}
    )
    ;
  ovldr(udt_i<0>{});
  ovldr(udt_i<1>{});
  ovldr(udt_i<2>{});
  }
  using namespace yafpl;
//  {
//    auto f = overload(
//        [](int i, double d) {},
//        [](std::string str, int i) {
//        },
//        [](double d, int i) {
//          BOOST_TEST(false);
//        }
//    );
//    f(1,1); // ambiguous call compalie fails
//  }
  {
    function_without_state foo;

    auto f = overload<int>(foo,  // foo should be copied
        [](std::string str) {
      BOOST_TEST(false);
      return 1;
    }
    //,    nonMember
    );
    f(1);

  }
  {
    function_without_state foo;

    auto f = overload(foo,  // foo should be copied
        [](std::string str) {
      BOOST_TEST(false);
    },
    nonMember
    );
    f(1);

  }
  {
    function_without_state foo;

    auto f = overload(std::move(foo),  // foo should be copied
        [](std::string str) {
      BOOST_TEST(false);
    },
    [](...)
    {
      std::cout << "int(...)" << std::endl;
    }
    );
    f(1);

  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(std::ref(foo),
        [](std::string str) {
      BOOST_TEST(false);
    })(1);

    BOOST_TEST(foo.invoked);
  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(foo,
        [](std::string str) {
      BOOST_TEST(false);
    })(1);

    BOOST_TEST(! foo.invoked);
  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(std::ref(foo), [](std::string str) { })("aaa");

    BOOST_TEST(! foo.invoked);
  }
  {
    auto f = overload(
        [](int )
        {
          std::cout << "int(int)" << std::endl;
          return 1;
        },
        [](auto const&)
        {
          std::cout << "int(auto const&)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == -1);
  }
  {
    auto f = overload<int>(
        [](int )
        {
          std::cout << "int(int)" << std::endl;
          return 1;
        },
        [](auto const&)
        {
          std::cout << "int(auto const&)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == -1);
  }
  {
    auto f = overload(
        [](std::vector<int>&& )
        {
          std::cout << "int(std::vector<int>&&)" << std::endl;
          return 1;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == 1);
  }
  {
    auto f = overload<int>(
        [](std::vector<int>&& )
        {
          std::cout << "int(std::vector<int>&&)" << std::endl;
          return 1;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == 1);
  }
  {
    auto f = overload(
        [](int i)
        {
          std::cout << "int(int)" << std::endl;
          return i;
        },
        [](std::string && i)
        {
          std::cout << "string(string&&)" << std::endl;
          return i;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1) == 1);
    BOOST_TEST(f(std::string("1")) == "1");
    BOOST_TEST_EQ(f(X{}), -1);
  }
  {
    auto f = overload<int>(
        [](int i)
        {
          std::cout << "int(int)" << std::endl;
          return i;
        },
        [](std::string && i)
        {
          std::cout << "string(string&&)" << std::endl;
          return i;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1) == 1);
    BOOST_TEST(f(std::string("1")) == "1");
    BOOST_TEST_EQ(f(X{}), -1);
  }
  {
    auto f = overload(
        nonMember,
        &X::f,
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1.0) == 42);
    X x;
    BOOST_TEST(f(x, 'c') == x);
    BOOST_TEST_EQ(f(Y{}), -1);
  }
  {
    auto f = overload<int>(
        nonMember,
        &X::f,
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1.0) == 42);
    X x;
    BOOST_TEST(f(x, 'c') == x);
    BOOST_TEST_EQ(f(Y{}), -1);
  }
  {
    constexpr auto f = overload(function_without_state{},
        [](std::string str) {
      return 1;
    }
    //,    nonMember
    );
    f(1);

  }
  {
    constexpr auto f = overload(function_without_state2{},
        [](std::string str) {
      return 1;
    }
    //,    nonMember
    );
    constexpr auto x = f(1);
    static_assert(1 == x, "");

  }
  {
    constexpr auto f = overload(function_without_state3{},
        [](std::string str) {
      return 1;
    }
    //,    nonMember
    );
#if 0
    constexpr auto x = f(1); // COMPILE ERROR as expected
    //overload/include_pass.cpp:409:25: erreur: call to non-constexpr function ‘int function_without_state3::operator()(int) const’
    //     constexpr auto x = f(1);
    static_assert(1 == x, "");
#endif
    auto y = f(1);
    assert(1 == y);

  }
  {
    constexpr auto f = overload<int>(function_without_state2{},
        [](std::string str) {
      return 1;
    }
    //,    nonMember
    );
    f(1);

  }
  return boost::report_errors();
}



