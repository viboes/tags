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

int nonMember( float ) {
  std::cout << "int(float)" << std::endl;
  return 42;
}

int main()
{
  using namespace yafpl;
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
        &X::f
        ,
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
        &X::f
        ,
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
  return boost::report_errors();
}



