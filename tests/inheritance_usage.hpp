// #include "../Throwing.hpp"

// template <t_contract... contracts> struct CombineContracts {
//   CombineContracts(contracts... cs) : m_t(std::make_tuple(cs...)){};
//   std::tuple<contracts...> m_t;
// };

// template <t_contract... contracts>
// CombineContracts(contracts...) -> CombineContracts<contracts...>;

// using namespace Contract_ns::Throwing;

// struct IBASE {
//   // API...
//   virtual void makeSound() = 0;
// };

// struct dad : public IBASE {
//   using MyConditions = Condition<std::logic_error, decltype(falser)>;
//   using MyContract = Contract<MyConditions>;
//   MyContract c;
//   dad() : c(TCONTRACT(pre<std::logic_error>(falser, "pre falser"))){};
//   void makeSound() override { auto C = c; }
// };

// struct child : public dad {
//   using MyContract =
//       Contract<Condition<std::logic_error, decltype(truer)>, dad::MyConditions>;
//   MyContract c;
//   child()
//       : c(std::experimental::source_location::current(),
//           std::tuple_cat(dad::c.getConditions(),
//                          boost::hana::make_tuple(
//                              pre<std::logic_error>(truer, "pre truer")))){};
//   void makeSound() override { auto C = c; };
// };
