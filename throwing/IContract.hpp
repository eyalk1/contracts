#ifndef ICONTRACT__HPP
#define ICONTRACT__HPP

#include "../CommonCondition.hpp"

#include <experimental/source_location>

namespace Contract_ns::Throwing {

struct IContract {
  virtual void check_conditions(
      cond_type filt, std::experimental::source_location const &location) const = 0;
};

} // namespace Contract_ns::Throwing

#endif // ICONTRACT__HPP