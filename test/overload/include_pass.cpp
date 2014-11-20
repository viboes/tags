//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <functional/v0/overload.hpp>

#include <boost/detail/lightweight_test.hpp>




int main()
{
  using namespace functional;
  {
    int i = overload(
        [](int i)
        {
          return i;
        },
        [](std::string && i)
        {
          return i;
        },
        [](...)
        {
        }
    )(1);
    BOOST_TEST(i == 1);
  }
  {
    std::string i = overload(
        [](int i)
        {
          return i;
        },
        [](std::string const& i)
        {
          return i;
        },
        [](...)
        {
        }
    )(std::string("a"));
    BOOST_TEST(i == "a");
  }
  return boost::report_errors();
}



