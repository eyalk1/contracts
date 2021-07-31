#include "include/ManualContract.hpp"
// #include "include/ThrowingContract.hpp"
#include "include/ManualCondition.hpp"


#include <experimental/source_location>
#include <iostream>
#include <sstream>
#include <string_view>


// TODO: add throwing predicate-provided exception.
// TODO: add tests
// TODO: add README.md
// TODO: use structures instead of triple tuples

using myError_t = std::pair<int, std::string>;

auto runtime_builder = [](std::experimental::source_location context,
                          std::string_view description, int EC) -> myError_t {
  std::stringstream ss;
  ss << description << " " << context.function_name();
  return {EC, ss.str()};
};

int main() {
  auto value_to_return = 8;
  ManualCondition bublul(
      precondition, [] { return false; }, "invariant is shit", 76);
  // auto T = TCONTRACT(post = std::pair{[] { return true; }, "post is shit"},
  //                    pre = std::pair{[] { return true; }, "pre isn't shit"},
  //                    invar = bublul);

  auto c = ManualContract(runtime_builder,
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