#include <gtest/gtest.h>

#include "../CommonCondition.hpp"
#include "../Manual.hpp"

#include "../utility.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>

using namespace Contract_ns::Manual;

TEST(HelloTest, BasicAssertions) {
    auto c = Contract(runtime_builder,
                      Condition(falser, "falser pre", 69, precondition));
    if(auto error = c.check(precondition); error.has_value())
    {
      std::cout << error.value().second << '\n';
      std::cout << error.value().first << '\n';
    }
}