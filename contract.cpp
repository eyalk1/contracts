#include "condition.hpp"
#include "ThrowingContract.hpp"
#include "ManualContract.hpp"

#include <string_view>
#include <experimental/source_location>
#include <sstream>
#include <iostream>

using myError_t = std::pair<int, std::string>;

auto runtime_builder = [](std::experimental::source_location context, std::string_view description, int EC) -> myError_t
    {
        std::stringstream ss;
        ss << description << " " << context.function_name();
        return {EC, ss.str()};};

template<typename F, typename... conditions>
using MyContract = ManualContract<myError_t, F, conditions...>;

int main()
{
    auto value_to_return = 8;
    condition bublul(precondition, []{return 78;}, "invariant is shit");
    // auto T = TCONTRACT(post=std::pair{[]{return true;}, "post is shit"},
    //                       pre=std::pair{[]{return true;}, "pre isn't shit"},
    //                       invariant=bublul);

    auto c = MyContract(runtime_builder,
                        post=std::pair{[]{return 9;}, "post is shit"},
                        pre=std::pair{[]{return 0;}, "pre isn't shit"},
                        invar=bublul);

    if (!c.check(precondition | invariant))
    {
        std::cout << "in main " << c.error.second << std::endl;
        return c.error.first;
    }

    // ...

    // if (!c.check(invariant))
    // {return c.error.first;}

    // //...

    // if(!c.check(postcondition))
    // {return c.error.first;}

    return value_to_return;
}