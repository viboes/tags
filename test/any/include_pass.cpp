//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <data_type/v0/match_any.hpp>
#include <functional/v0/match.hpp>

#include <string>

#include <boost/detail/lightweight_test.hpp>

int main()
{
  using namespace meta;
  using namespace functional;
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    functional::match<void>(select<types<int, std::string>>(a),
        [](int i)
        {},
        [](std::string const& i)
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
    functional::match<void>(select<types<int, std::string>>(a),
        [](int i)
        {
          BOOST_TEST(false);
        },
        [](std::string const& i)
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
    functional::match<void>(select<types<int, std::string>>(a),
        [](std::string const& i)
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
    functional::match<void>(select<types<int, std::string>>(a),
        [&j](std::string i)
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
    functional::match<void>(select<types<int>>(a),
        [](int i)
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // example functional::select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int j = 8;
    functional::match<void>(select<types<int, std::string>>(a),
        [&j](std::string const& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    // functional::match functional::select<types<>>(a)
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
    // functional::match functional::select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    functional::match<void>(select<types<int>>(a),
        [](std::string const&i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    // functional::match_all functional::select<types<>>(a)
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
    boost::match<void>(a, types<int, std::string>{},
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // compile fail as there is no overload for cath all 'boost::any'
    boost::any a = 2;
    boost::match<void>(a, types<std::string>{},
        [](std::string& i)
        {
          BOOST_TEST(false);
        }
    );
  }
#endif
  return boost::report_errors();
}


