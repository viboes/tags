//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompboost::anying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <yafpl/v1/data_type/boost/any.hpp>
#include <yafpl/v1/functional/match.hpp>
#include <yafpl/v1/functional/tags.hpp>

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

int main()
{
  {
    int a = 2;
    boost::any b = 2;
    yafpl::match<void>(std::make_tuple(a, yafpl::select<yafpl::types<int, std::string>>(b)),
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
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int i = yafpl::match<int>(yafpl::select<yafpl::types<int, std::string>>(a),
        [](int i)
        {
          return 0;
        },
        [](std::string const& i)
        {
          BOOST_TEST(false);
          return 1L;
        },
        [](...)
        {
          BOOST_TEST(false);
          return 2;
        }
    );
    BOOST_TEST(i==0);
  }
  using namespace yafpl;
  using namespace yafpl::meta;
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int i = match<int>(select<types<int, std::string>>(a),
        [](int i)
        {
          return 0;
        },
        [](std::string const& i)
        {
          BOOST_TEST(false);
          return 1L;
        },
        [](...)
        {
          BOOST_TEST(false);
          return 2;
        }
    );
    BOOST_TEST(i==0);
  }
  {
    // example
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    match<void>(select<types<int, std::string>>(a),
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
    int i = 0;
    boost::any a = &i;
    match<void>(select<types<int*>>(a),
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
    // example
    // taking a list of accepted types for this argument.
    int i = 0;
    boost::any a = std::ref(i);
    match<void>(select<types<std::reference_wrapper<int>>>(a),
        [](std::reference_wrapper<int> i)
        {},
        [](std::string i)
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
    // example
    // taking a list of accepted types for this argument.
    boost::any a = std::string("2");
    match<void>(select<types<int, std::string>>(a),
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
    match<void>(select<types<int, std::string>>(a),
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
    match<void>(select<types<int, std::string>>(a),
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
    match<void>(select<types<int>>(a),
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
    // example select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    int j = 8;
    match<void>(select<types<int, std::string>>(a),
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
    // match select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = "2";
    match<void>(select<types<int>>(a),
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
    // match select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    match<void>(select<types<int>>(a),
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
    // match select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = 2;
    boost::any b = 2;
    match<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
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
    match<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
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
    match<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
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
    // todo: this must work.
    int a = 2;
    boost::any b = 2;
    match<void>(std::make_tuple(a, select<types<int, std::string>>(b)),
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
  // compile fails
  {
    boost::any a = 2;
    boost::any b = std::string("2");
    match<void>(std::make_tuple(select<types<int, std::string>>(a), select<types<int, std::string>>(b)),
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
    // match select<types<>>(a)
    // taking a list of accepted types for this argument.
    boost::any a = make_tagged<t1>(1);
    match<void>(select<tags<t1>>(a),
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
    match<void>(select<tags<t1,t2>>(a),
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
    match<void>(std::make_tuple(select<tags<t1, t2>>(ta), select<tags<t1, t2>>(tb)),
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
    match<void>(std::make_tuple(select<tags<t1, t2>>(ta), select<tags<t1, t2>>(tb)),
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
  {
    int i;
    boost::any a = make_tagged<t3>(i);
    match<void>(select<tags<t3>>(a),
        [](tagged<int&, t3> i)
        {
        },
        [](auto && i)
        {
          std::cout << typeid(i).name() << std::endl;
          BOOST_TEST(false);
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }
  {
    int i;
    boost::any a = make_tagged<t4>(&i);
    match<void>(select<tags<t4>>(a),
        [](tagged<int*, t4> i)
        {
        },
        [](auto && i)
        {
          std::cout << typeid(i).name() << std::endl;
          BOOST_TEST(false);
        },
        [](...)
        {
          BOOST_TEST(false);
        }
    );
  }


  return boost::report_errors();
}

