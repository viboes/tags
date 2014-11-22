//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <data_type/v0/match_variant.hpp>
#include <functional/v0/match.hpp>
#include <functional/v0/tags.hpp>
#include <functional/v0/select.hpp>

#include <string>
#include <functional>

#include <boost/detail/lightweight_test.hpp>

struct t1
{};

struct t2
{};

struct t3
{};

struct t4
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
  template<>
  struct tag_type<t3>
  {
    typedef int& type;
  };
  template<>
  struct tag_type<t4>
  {
    typedef int* type;
  };
}

struct X{};

int main()
{
  using namespace meta;
  using namespace functional;
  {
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
        [](int i)
        {},
        [](X const& i)
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
    int i = 0;
    boost::variant<int*, std::string> a = &i;
    functional::match<void>(a,
        [](int* i)
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
    int i = 0;
    boost::variant<std::reference_wrapper<int>, std::string> a = std::ref(i);
    functional::match<void>(a,
        [](std::reference_wrapper<int> i)
        {},
        [](std::string const& i)
        {
          BOOST_TEST(false);
        },
        [](auto&)
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
    boost::variant<int, std::string> a = std::string("2");
    functional::match<void>(a,
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
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
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
    int j = 8;
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
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
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
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
    int j = 8;
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
        [&j](X const& i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    boost::variant<int, X> a = 2;
    functional::match<void>(a,
        [](X const&i)
        {
          BOOST_TEST(false);
        },
        [](...)
        {

        }
    );
  }
  {
    boost::variant<int, X> a = 2;
    boost::variant<int, X> b = 2;
    functional::match_all<void>(std::make_tuple(a, b),
        [](int const &i, int const &j )
        {
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
    boost::variant<int, std::string> a = 2;
    boost::variant<int, std::string> b = std::string("2");
    functional::match_all<void>(std::make_tuple(a, b),
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
    boost::variant<int, std::string> a = 2;
    boost::variant<int, std::string> b = std::string("2");
    functional::match_all<void>(std::make_tuple(a, b),
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
    boost::variant<int, std::string> a = 2;
    boost::variant<int, std::string> b = std::string("2");
    functional::match_all<void>(std::make_tuple(a, b),
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
    boost::variant<int, std::string> a = 2;
    boost::match<void>(a,
        [](std::string const& i)
        {
          BOOST_TEST(false);
        }
    );
  }
#endif

  // tagged
  {
    boost::variant<tagged<int,t1>, tagged<std::string, t2>> a = make_tagged<t1>(1);
    functional::match<void>(a,
        [](tagged<int, t1> const& i)
        {
        },
        [](tagged<std::string, t2> const& i)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    boost::variant<tagged<int,t1>, std::string> a = make_tagged<t1>(1);
    functional::match<void>(a,
        [](tagged<int, t1> const& i)
        {
        },
        [](std::string const& i)
        {
          BOOST_TEST(false);
        }
    );
  }
//  {
//    boost::any a = make_tagged<t2>("a");
//    functional::match<void>(functional::select<functional::tags<t1,t2>>(a),
//        [](tagged<int, t1> i)
//        {
//          BOOST_TEST(false);
//        },
//        [](tagged<std::string, t2> i)
//        {
//        },
//        [](...)
//        {
//          BOOST_TEST(false);
//        }
//    );
//  }
//  {
//    boost::any ta = make_tagged<t1>(1);
//    boost::any tb = make_tagged<t2>(std::string("2"));
//    match_all<void>(std::make_tuple(select<functional::tags<t1, t2>>(ta), select<functional::tags<t1, t2>>(tb)),
//        [](tagged<int, t1>           const&      i,  tagged<int, t1>         const&        j)
//        {
//          BOOST_TEST(false);
//        },
//        [](tagged<int, t1>           const&      i,  tagged<std::string, t2>  const& j)
//        {
//
//        },
//        [](tagged<std::string, t2>  const& i,  tagged<int, t1>         const&        j)
//        {
//          BOOST_TEST(false);
//        },
//        [](tagged<std::string, t2>  const& i,  tagged<std::string, t2>  const& j)
//        {
//          BOOST_TEST(false);
//        },
//        [](auto const &i, auto const &j )
//        {
//          BOOST_TEST(false);
//        },
//        [](...)
//        {
//          BOOST_TEST(false);
//        }
//    );
//  }
//  {
//    boost::any ta = make_tagged<t1>(1);
//    boost::any tb = make_tagged<t2>(std::string("2"));
//    match_all<void>(std::make_tuple(select<functional::tags<t1, t2>>(ta), select<functional::tags<t1, t2>>(tb)),
//        [](tagged<int, t1>           const&      i,  tagged<int, t1>         const&        j)
//        {
//          BOOST_TEST(false);
//        },
//        [](tagged<std::string, t2>  const& i,  tagged<int, t1>         const&        j)
//        {
//          BOOST_TEST(false);
//        },
//        [](tagged<std::string, t2>  const& i,  tagged<std::string, t2>  const& j)
//        {
//          BOOST_TEST(false);
//        },
//        [](auto const &i, auto const &j )
//        {
//        },
//        [](...)
//        {
//          BOOST_TEST(false);
//        }
//    );
//  }
//  {
//    int i;
//    boost::any a = make_tagged<t3>(i);
//    functional::match<void>(functional::select<functional::tags<t3>>(a),
//        [](tagged<int&, t3> i)
//        {
//        },
//        [](auto && i)
//        {
//          std::cout << typeid(i).name() << std::endl;
//          BOOST_TEST(false);
//        },
//        [](...)
//        {
//          BOOST_TEST(false);
//        }
//    );
//  }
//  {
//    int i;
//    boost::any a = make_tagged<t4>(&i);
//    functional::match<void>(functional::select<functional::tags<t4>>(a),
//        [](tagged<int*, t4> i)
//        {
//        },
//        [](auto && i)
//        {
//          std::cout << typeid(i).name() << std::endl;
//          BOOST_TEST(false);
//        },
//        [](...)
//        {
//          BOOST_TEST(false);
//        }
//    );
//  }


  return boost::report_errors();
}

