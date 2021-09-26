#include "../Throwing.hpp"
//TODO:
// 1. RAII for each function - member function (call operator) to generate contract from member contract variable and additional contracts.
// 2. member variable template type deduction

using namespace Contract_ns::Throwing;

struct IBASE{
  //API...
  virtual void makeSound() = 0;
};

/************************************************/
// weird design, no caching of contract object.

template<t_condition... conditions>
struct CBASE{
  //contract definition
  Contract_ns::Throwing::Contract<conditions...> dc;
};


struct dad : public IBASE, private CBASE<Condition<std::logic_error, decltype(falser)>>
{
  void makeSound() override{

  }
  //overriding interface and injecting contract cheking into it
};

struct child : public dad, private CBASE<Condition<std::runtime_error, decltype(truer)>>
{
  void makeSound() override{

  }
};

/*****************************************/

struct dad2 : public IBASE{
  using MyContract = Contract_ns::Throwing::Contract<Condition<std::logic_error, decltype(falser)>;
  MyContract c{pre<std::logic_error>(falser, "pre falser")};
  void makeSound() override{
    auto C = c;
  }
};


struct child2 : public dad2{
  using MyContract = Contract_ns::Throwing::Contract<Condition<std::logic_error, decltype(falser)>;
  MyContract c{pre<std::logic_error>(truer, "pre truer")};
  void makeSound() override{
    auto C = c + dad2::c;
  }

};



/*****************************************/

// bad because user types are now littered with template.
template<t_condition... conditions>
struct A {
    A(conditions... conditions_)
      : dc(TCONTRACT(pre<std::logic_error>(falser, "pre falser"), // this class' own conditions
      conditions_...))
    {
    };    // other ones the deriving classes may add
  // class's stuff ...
  // ...
  // condition is last
  Contract_ns::Throwing::Contract<conditions...> dc;
};

// class B : public A {
//   B()
//       : A(4, 2, pre(...), invar(...)) // injecting derived's conditions
//         {};
// };