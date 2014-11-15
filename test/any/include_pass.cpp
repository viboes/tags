//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <tags/v0/any.hpp>
#include <functional/v0/match.hpp>

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
  using namespace tags;
  {
    // example
    boost::any a = 2;
    accept<void, int, std::string>(a,
        [](int& i)
        {
        },
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  //#if defined D2014

  {
    // example
    boost::any a = 2;
    accept<void, int>(a,
        [](int& i)
        {},
        [](auto&)
        {
          BOOST_TEST(false);
        }
    );
  }
  //#endif

  {
    // example
    boost::any a = std::string("2");
    int i = accept<int, int, std::string>(a,
        [](int& i)
        {
          BOOST_TEST(false);
          return i;
        },
        [](std::string& i)
        { return 1;}
    );
    std::cout << i << std::endl;
  }
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    accept<void>(a, types<int, std::string>{},
        [](int& i)
        {},
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    match<void>(a, types<int, std::string>{},
        [](int& i)
        {},
        [](std::string& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = std::string("2");
    match<void>(a, types<int, std::string>{},
        [](int& i)
        {
          BOOST_TEST(false);
        },
        [](std::string& i)
        {},
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    match<void>(a, types<int, std::string>{},
        [](std::string& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {
        }
    );
  }
  {
    // example capture
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int j = 8;
    match<void>(a, types<int, std::string>{},
        [&j](std::string& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    // example ...
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    match<void>(a, types<int>{},
        [](int& i)
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // example select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int j = 8;
    match<void>(select<types<int, std::string>>(a),
        [&j](std::string& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    // functional::match select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = "2";
    functional::match<void>(select<types<int>>(a),
        [](int const&i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    // functional::match_tuple select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    boost::any b = 2;
    functional::match_all<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(a)),
        [](int const &i, int const &j )
        {
        },
        [](auto const &i, auto const &j )
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }

#if 0
  // compile fail as there is no overload for 'int'
  {
    boost::any a = 2;
    match<void>(a, types<int, std::string>{},
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // compile fail as there is no overload for cath all 'boost::any'
    boost::any a = 2;
    match<void>(a, types<std::string>{},
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
#endif
  return boost::report_errors();
}


