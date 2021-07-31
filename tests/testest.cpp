#include <gtest/gtest.h>

#include "../Throwing.hpp"
#include <vector>

TEST(HelloTest, BasicAssertions) {

  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  try {
    auto c = TCONTRACT(pre<std::runtime_error> =
                           Condata([&v] { return v.size() == 8; },
                                   "vector should start with 8 integers"),
                       post<std::logic_error> =
                           Condata([&v] { return v.size() == 8; },
                                   "vector size should get bigger."));
    for (uint i = 0; i < 100; i++)
      v.push_back(i);
  } catch (...) {
    std::cout << "heyo\n";
  }

  //   auto c = TCONTRACT(
  //       pre<std::runtime_error> = Condata([&v] { return v.size() == 8; },
  //                                         "vector should start with 8
  //                                         integers"),
  //       post<std::logic_error> = Condata([&v] { return v.size() > 8; },
  //                                        "vector size should get bigger."));
}