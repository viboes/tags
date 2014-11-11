//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <tags/v0/any.hpp>

#include <string>

#include <boost/detail/lightweight_test.hpp>

struct t1
{};

struct t2
{};

namespace tags
{
  template<>
  struct tag_type<t1>
  {
    typedef int type;
  };
  template<>
  struct tag_type<t2>
  {
    typedef std::string type;
  };
}

int main()
{
  using namespace boost;
  using namespace tags;
  {
    // example
    any a = 2;
    accept<void, int, std::string>(a,
        [](int& i)
        {},
        [](std::string& i)
        {}
    );
  }
#if defined D2014
  {
    // example
    any a = 2;
    accept<void, int>(a,
        [](int& i)
        {},
        [](auto&)
        {}
    );
  }
#endif
  {
    // example
    any a = std::string("2");
    int i = accept<int, int, std::string>(a,
        [](int& i)
        { return i;},
        [](std::string& i)
        { return 1;}
    );
    std::cout << i << std::endl;
  }
  {
    // example
    // taking a list of accepted types for this argument.
    any a = 2;
    accept<void>(a, types<int, std::string>(),
        [](int& i)
        {},
        [](std::string& i)
        {}
    );
  }

  return boost::report_errors();
}

