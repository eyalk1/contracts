#ifndef ICONTRACT__HPP
#define ICONTRACT__HPP

#include <experimental/source_location>

struct IContract {
  virtual void check_conditions(
      int filt, std::experimental::source_location const &location) const = 0;
};

#endif // ICONTRACT__HPP