#include <gtest/gtest.h>

#include "../CommonCondition.hpp"
// #include "../Manual.hpp"
#include "../Throwing.hpp"
#include "../throwing/IContract.hpp"

#include "../utility.hpp"

#include <boost/hana/tuple.hpp>

#include <array>
#include <concepts>
#include <iostream>
#include <stdlib.h>
#include <type_traits>

// TODO: add static assertions for simple error messages all over.
// TODO: delete move and copy constructors

// using namespace Contract_ns::Manual;
using namespace Contract_ns::Throwing;

auto maybe = [] { return !bool(rand() % 100); };

TEST(HelloTest, BasicAssertions) {

  try {
    auto c = DryContract(post<std::logic_error>(maybe, "pre falser"));
    // auto c1 = DryContract(pre<std::logic_error>(maybe, "pre super"));
    try {
    //   // std::array<std::reference_wrapper<Contract_ns::Throwing::IContract
    //   // const>,1>({std::ref(c1)})
      // Bases b1(c);
      auto c2 =
          Contract(std::experimental::source_location::current(), Bases(c),
                   pre<std::logic_error>(maybe, "maybe", defExcGen));
    } catch (const std::logic_error &e) {
      std::cerr << e.what() << "\nhello\n";
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << "\nbye bye\n";
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\nthis is fucking shit\n";
  }
  // child a;
  // auto c = Contract(runtime_builder,
  //                   pre(falser, "pre falser", 80),
  //                   Condition(falser, "falser pre", 69, precondition));
  // if(auto error = c.check(precondition); error.has_value())
  // {
  //   std::cout << error.value().second << '\n';
  //   std::cout << error.value().first << '\n';
  // }
}
