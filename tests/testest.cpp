#include <gtest/gtest.h>

#include "../CommonCondition.hpp"
// #include "../Manual.hpp"
#include "../Throwing.hpp"

#include "../utility.hpp"

#include <boost/hana/tuple.hpp>

#include <concepts>
#include <iostream>
#include <stdlib.h>
#include <type_traits>

// TODO: add static assertions for simple error messages all over.

// using namespace Contract_ns::Manual;

auto maybe = [] { return !bool(rand() % 100); };


TEST(HelloTest, BasicAssertions) {

  try {
    // auto c = DryContract(post<std::logic_error>(
    //     maybe, "pre falser"));
    auto c1 = DryContract(pre<std::logic_error>(maybe, "pre super"));
    try {
      auto c2 = CONTRACT({c1},
        pre<std::logic_error>(maybe, "maybe", defExcGen));
      //   CONTRACT({c, c1}, post<std::runtime_error>(truer, "pre truer"));
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
