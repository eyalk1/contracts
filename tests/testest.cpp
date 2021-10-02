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

// using namespace Contract_ns::Manual;

auto maybe = [] { return !bool(rand() % 100); };

template <exception E>
auto pre1 = [](argumentless_function auto f, std::string_view sv,
              Contract_ns::cond_type type) {
  return Condition<std::logic_error, decltype(f), decltype(&defExcGen)>(f, sv,
                                                                       type, &defExcGen);
};

template <exception E>
auto pre2 = [](argumentless_function auto f, std::string_view sv,
              Contract_ns::cond_type type, descGen auto dg) {
  return Condition<std::logic_error, decltype(f), decltype(dg)>(f, sv, type,
                                                                dg);
};

template<exception E>
overload pre{pre1<E>, pre2<E>};

TEST(HelloTest, BasicAssertions) {

  try {
    // auto c = DryContract(post<std::logic_error>(
    //     maybe, "pre falser"));
    auto c1 = DryContract(pre<std::logic_error>(maybe, "pre super", precondition));
    try {
      auto c2 = Contract(std::experimental::source_location::current(),
                         pre<std::logic_error>(maybe, "maybe", precondition,
                                               defExcGen));
      //   TCONTRACT({c, c1}, post<std::runtime_error>(truer, "pre truer"));
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
