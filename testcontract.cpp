#include "Manual.hpp"
// #include "Throwing.hpp"
#include "CommonCondition.hpp"

#include <experimental/source_location>
#include <iostream>
#include <sstream>
#include <string_view>

// TODO: add tests
// TODO: add README.md


int main() {
  auto value_to_return = 0;
  // Condition bublul(
  //     precondition, [] { return false; }, "invariant is shit", 76);

  // auto T = TCONTRACT(pre<std::logic_error> =
  //                        Condata([] { return true; }, "poopy poop"),
  //                    invar<std::runtime_error> =
  //                        Condata([] { return false; }, "invariant is shit"));

  auto c = Contract(runtime_builder,
                      // post=std::pair{[]{return 9;}, "post is shit"},
                      // pre=std::pair{[]{return 0;}, "pre isn't shit"},
                      pre=Condata([]{return false;}, "invariant is shit", 76));

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