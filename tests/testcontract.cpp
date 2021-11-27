#include "include/ManualCondition.hpp"
#include "include/ManualContract.hpp"
#include "include/Condition.hpp"
#include "include/ThrowingContract.hpp"
#include "include/CommonCondition.hpp"

#include <experimental/source_location>
#include <iostream>
#include <sstream>
#include <string_view>

using Contract_ns::Throwing::DryContract;
using Contract_ns::Throwing::Condition;
using Contract_ns::precondition;
using Contract_ns::invariant;
using Contract_ns::postcondition;
using Contract_ns::Throwing::pre;
using Contract_ns::Throwing::invar;
using Contract_ns::Throwing::post;

// TODO: add tests
// TODO: add README.md


int main() {
  auto value_to_return = 0;
  // Condition bublul(
  //     precondition, [] { return false; }, "invariant is shit", 76);

  auto T = TCONTRACT(pre<std::logic_error> =
                         Condition([] { return true; }, "poopy poop"),
                     invar<std::runtime_error> =
                         Condition([] { return false; }, "invariant is shit"));

  auto c = DryContract(runtime_builder,
                      // post=std::pair{[]{return 9;}, "post is shit"},
                      // pre=std::pair{[]{return 0;}, "pre isn't shit"},
                      pre_m=bublul);

  if (!c.check(precondition | invariant))
  {
      std::cout << "in main " << c.m_error.second << std::endl;
      return c.m_error.first;
  }

  // ...

  // if (!c.check(invariant))
  // {return c.m_error.first;}

  // //...

  // if(!c.check(postcondition))
  // {return c.m_error.first;}

  return value_to_return;
}