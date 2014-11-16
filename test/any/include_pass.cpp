//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <data_type/v0/match_any.hpp>
#include <functional/v0/match.hpp>
#include <functional/v0/tags.hpp>

#include <string>

#include <boost/detail/lightweight_test.hpp>

struct t1
{};

struct t2
{};

namespace functional
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
    functional::match_all<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
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
  {
    boost::any a = 2;
    boost::any b = std::string("2");
    functional::match_all<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
        [](int i, int j )
        {
          BOOST_TEST(false);
        },
        [](auto const &i, auto const &j )
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    boost::any a = 2;
    boost::any b = std::string("2");
    functional::match_all<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
        [](int i, int j )
        {
          BOOST_TEST(false);
        },
        [](auto &&i, auto &&j )
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }

#if 0
  // compile fails
  {
    boost::any a = 2;
    boost::any b = std::string("2");
    functional::match_all<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
        [](int i, int j )
        {
          BOOST_TEST(false);
        },
        [](...)
        {
        }
    );
  }
#endif
#if 0
  // compile fail as there is no overload for 'int'
  {
    boost::any a = 2;
    boost::match<void>(select<types<int, std::string>>(a)),
        [](std::string const& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    // compile fail as there is no overload for catch all 'boost::any'
    boost::any a = 2;
    boost::match<void>(select<types<std::string>>(a),
        [](std::string const& i)
        {
          BOOST_TEST(false);
        }
    );
  }
#endif
  {
    // functional::match functional::select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = make_tagged<t1>(1);
    functional::match<void>(functional::select<functional::tags<t1>>(a),
        [](tagged<int, t1> i)
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    boost::any a = make_tagged<t2>("a");
    functional::match<void>(functional::select<functional::tags<t1,t2>>(a),
        [](tagged<int, t1> i)
        {
          BOOST_TEST(false);
        },
        [](tagged<std::string, t2> i)
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    boost::any ta = make_tagged<t1>(1);
    boost::any tb = make_tagged<t2>(std::string("2"));
    match_all<void>(std::make_tuple(select<functional::tags<t1, t2>>(ta), select<functional::tags<t1, t2>>(tb)),
        [](tagged<int, t1>           const&      i,  tagged<int, t1>         const&        j)
        {
          BOOST_TEST(false);
        },
        [](tagged<int, t1>           const&      i,  tagged<std::string, t2>  const& j)
        {

        },
        [](tagged<std::string, t2>  const& i,  tagged<int, t1>         const&        j)
        {
          BOOST_TEST(false);
        },
        [](tagged<std::string, t2>  const& i,  tagged<std::string, t2>  const& j)
        {
          BOOST_TEST(false);
        },
        [](auto const &i, auto const &j )
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
    boost::any ta = make_tagged<t1>(1);
    boost::any tb = make_tagged<t2>(std::string("2"));
    match_all<void>(std::make_tuple(select<functional::tags<t1, t2>>(ta), select<functional::tags<t1, t2>>(tb)),
        [](tagged<int, t1>           const&      i,  tagged<int, t1>         const&        j)
        {
          BOOST_TEST(false);
        },
        [](tagged<std::string, t2>  const& i,  tagged<int, t1>         const&        j)
        {
          BOOST_TEST(false);
        },
        [](tagged<std::string, t2>  const& i,  tagged<std::string, t2>  const& j)
        {
          BOOST_TEST(false);
        },
        [](auto const &i, auto const &j )
        {
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }

  return boost::report_errors();
}

