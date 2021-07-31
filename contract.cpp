#include "include/ManualCondition.hpp"
#include "include/ManualContract.hpp"
#include "include/ThrowingCondition.hpp"
#include "include/ThrowingContract.hpp"

#include <experimental/source_location>
#include <iostream>
#include <sstream>
#include <string_view>

using namespace Contract::Throwing;
using namespace Contract;

// TODO: add tests
// TODO: add README.md
// TODO: add ns

using myError_t = std::pair<int, std::string>;

auto runtime_builder = [](std::experimental::source_location context,
                          std::string_view description, int EC) -> myError_t {
  std::stringstream ss;
  ss << description << " " << context.function_name();
  return {EC, ss.str()};
};

int main() {
  auto value_to_return = 0;
  // ManualCondition bublul(
  //     precondition, [] { return false; }, "invariant is shit", 76);
  auto k = [] { return false; };

  Condition<decltype(k), std::runtime_error> bublul(precondition, k,
                                                    "pre is shit");

  Condition<decltype(k), std::logic_error> bublul2(invariant, k,
                                                   "invariant is shit");

  auto T = TCONTRACT(pre<std::logic_error> =
                         Condata([] { return true; }, "poopy poop"),
                     invar<std::runtime_error> =
                         Condata([] { return false; }, "invariant is shit"));

  // auto c = ManualContract(runtime_builder,
  //                     // post=std::pair{[]{return 9;}, "post is shit"},
  //                     // pre=std::pair{[]{return 0;}, "pre isn't shit"},
  //                     pre_m=bublul);

  // if (!c.check(precondition | invariant))
  // {
  //     std::cout << "in main " << c.m_error.second << std::endl;
  //     return c.m_error.first;
  // }

  // ...

  // if (!c.check(invariant))
  // {return c.m_error.first;}

  // //...

  // if(!c.check(postcondition))
  // {return c.m_error.first;}

  return value_to_return;
}