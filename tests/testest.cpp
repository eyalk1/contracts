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
#include <time.h>
#include <type_traits>

// TODO: add static assertions for simple error messages all over.
// TODO: add MakeWet and MakeDry overloads in Dry contract - they bake in
// themselves as bases.

// using namespace Contract_ns::Manual;
using namespace Contract_ns::Throwing;

auto maybe = [] { return !bool(rand() % 2); };
auto surely = [] {return true;};
auto surely_not =[] { return false;};

TEST(HelloTest, BasicAssertions) {
  srand(time(NULL));
  for (int i = 0; i < 1; i++) {
    try {
      //   // std::array<std::reference_wrapper<Contract_ns::Throwing::IContract
      //   // const>,1>({std::ref(c1)})
      // Bases b1(c);
      auto c2 = DryContract(pre<std::logic_error>(surely, "maybe", defExcGen));
      auto c3 = c2.MakeDry(pre<std::runtime_error>(surely_not, "definetly"));
    } catch (const std::logic_error &e) {
      std::cerr << e.what() << "\nhello\n";
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << "\nbye bye\n";
    }
  }

}
