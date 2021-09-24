#include <gtest/gtest.h>

#include "../Throwing.hpp"
#include "../CommonCondition.hpp"

#include <iostream>
#include <type_traits>
#include <concepts>

using namespace Contract_ns::Throwing;

TEST(HelloTest, BasicAssertions) {
  try{
    
  auto c = TCONTRACT(
    pre<std::runtime_error>([] {return true;}, "pre false"),
    post<std::logic_error>([]{return false;}, "post false"));
    // Condition<std::runtime_error>([] { return false; }, "custom", precondition),
    // pre<std::runtime_error>  = Condition([] { return false; }, "pre"),
    // post<std::runtime_error> = Condition([] { return true; }, "post")
  // );
  }catch(std::exception const& e){
    std::cout << e.what() << std::endl;
  }
}