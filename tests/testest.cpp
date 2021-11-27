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
// TODO: add MakeWet and MakeDry overloads in Dry contract - they bake in themselves as bases.

// using namespace Contract_ns::Manual;
using namespace Contract_ns::Throwing;

auto maybe = [] { return !bool(rand() % 2); };

TEST(HelloTest, BasicAssertions) {
  srand(time(NULL));
  for (int i = 0; i < 100; i++) {
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
}
