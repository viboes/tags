//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <tags/v0/map_tag_any.hpp>

#include <string>

#include <boost/detail/lightweight_test.hpp>

struct t1
{};

struct t2
{};

struct t3
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
  template<>
  struct tag_type<t3>
  {
    typedef map_tag_any type;
  };
}

int main()
{
  using namespace boost;
  using namespace tags;

  {
    // example
    map_tag_any mta;
    tag_insert<t1>(mta, 1);
    std::cout << "t1::" << tag_cast<t1>(mta) << std::endl;

    tag_accept<void, t1, t2>(mta,
        [](t1, int& i)
        {
          std::cout << i << std::endl;
        },
        [](t2, std::string& i)
        {
          std::cout << i << std::endl;
        }
    );
  }
  {
    // example
    map_tag_any mta;
    tag_insert<t1>(mta, 1);
    std::cout << "t1::" << tag_cast<t1>(mta) << std::endl;
    tag_insert<t2>(mta, std::string("a"));
    std::cout << "t2::" << tag_cast<t2>(mta) << std::endl;
    tag_accept<void, t1, t2>(mta,
        [](t1, int& i)
        {
          std::cout << i << std::endl;
        },
        [](t2, std::string& i)
        {
          std::cout << i << std::endl;
        }
    );
  }
  {
    // example
    map_tag_any mta;
    tag_insert<t1>(mta, 1);
    std::cout << "t1::" << tag_cast<t1>(mta) << std::endl;

    map_tag_any mta3;
    tag_insert<t2>(mta3, std::string("a"));
    std::cout << "t2::" << tag_cast<t2>(mta3) << std::endl;

    tag_insert<t3>(mta, mta3);
    //std::cout << "t3::" << tag_cast<t1>(mta) << std::endl;

    tag_accept<void>(mta, types<t1, t2, t3>(),
        [](t1, int& i)
        {
          std::cout << "t1:" << i << std::endl;
        },
        [](t2, std::string& i)
        {
          std::cout << "t2:" << i << std::endl;
        },
        [](t3, map_tag_any& m)
        {
          std::cout << "t3:" << std::endl;
          tag_accept<void, t2>(m,
              [](t2, std::string& i)
              {
                std::cout << "  t2:" << i << std::endl;
              }
          );
        }
    );
  }
  {
    // example
    map_tag_any mta;
    tag_insert<t1>(mta, 1);
    std::cout << "t1::" << tag_cast<t1>(mta) << std::endl;

    tag_accept<void>(mta, types<t1, t2>(),
        [](t1, int& i)
        {
          std::cout << i << std::endl;
        },
        [](t2, std::string& i)
        {
          std::cout << i << std::endl;
        }
    );
  }
  return boost::report_errors();
}

