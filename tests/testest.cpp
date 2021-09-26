#include <gtest/gtest.h>

#include "inheritance_usage.hpp"

#include "../CommonCondition.hpp"
// #include "../Manual.hpp"

#include "../utility.hpp"

#include <concepts>
#include <iostream>
#include <type_traits>

// using namespace Contract_ns::Manual;

TEST(HelloTest, BasicAssertions) {
    child a;
    // auto c = Contract(runtime_builder,
    //                   pre(falser, "pre falser", 80),
    //                   Condition(falser, "falser pre", 69, precondition));
    // if(auto error = c.check(precondition); error.has_value())
    // {
    //   std::cout << error.value().second << '\n';
    //   std::cout << error.value().first << '\n';
    // }
}