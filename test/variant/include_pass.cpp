//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <yafpl/v1/data_type/boost/variant.hpp>
#include <yafpl/v1/data_type/optional/optional.hpp>
#include <yafpl/v1/functional/match.hpp>
#include <yafpl/v1/functional/tags.hpp>
#include <yafpl/v1/functional/select.hpp>

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

namespace yafpl
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
  using namespace yafpl;
  using namespace yafpl::meta;
  {
    boost::variant<int, X> a = 2;
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(a,
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
    match<void>(std::make_tuple(a, b),
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
    match<void>(std::make_tuple(a, b),
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
    match<void>(std::make_tuple(a, b),
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
  {
    boost::variant<int, std::string, double> v(1.0);
    std::experimental::optional<double> o;
    bool b = match<bool>(std::make_tuple(v,o),
        [] (double, std::experimental::nullopt_t) { return true;},
        [] (auto, auto) { return false; }
    );
    BOOST_TEST(b);
  }
  {
    boost::variant<int, X, double> v(1.0);
    std::experimental::optional<double> o;
    bool b = match<bool>(std::make_tuple(v,o),
        [] (double, std::experimental::nullopt_t) { return true;},
        [] (...) { return false; }
    );
    BOOST_TEST(b);
  }

#if 0
  // compile fails
  {
    boost::variant<int, std::string> a = 2;
    boost::variant<int, std::string> b = std::string("2");
    match<void>(std::make_tuple(a, b),
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
    match<void>(a,
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
    match<void>(a,
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
//    match<void>(select<tags<t1,t2>>(a),
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
//    match<void>(std::make_tuple(select<tags<t1, t2>>(ta), select<tags<t1, t2>>(tb)),
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
//    match<void>(std::make_tuple(select<tags<t1, t2>>(ta), select<tags<t1, t2>>(tb)),
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
//    match<void>(select<tags<t3>>(a),
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
//    match<void>(select<tags<t4>>(a),
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

