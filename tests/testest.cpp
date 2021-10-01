#include <gtest/gtest.h>

#include "inheritance_usage.hpp"

#include "../CommonCondition.hpp"
// #include "../Manual.hpp"
#include "../Throwing.hpp"

#include "../utility.hpp"

#include <boost/hana/tuple.hpp>

#include <concepts>
#include <iostream>
#include <type_traits>
#include <stdlib.h>

// using namespace Contract_ns::Manual;

auto maybe = []{ return !bool(rand() % 100);};


TEST(HelloTest, BasicAssertions) {
        int c = 0;
        for(auto i=0;i<1000;i++)
            if(!maybe())
                c++;

        std::cout << c << "\n";
        try
        {
            auto c = DryContract(post<std::logic_error>(maybe, "pre falser"));
            try
            {
                auto c2 = TCONTRACT(c,
                                   post<std::runtime_error>(truer, "pre truer"));
            }
            catch(const std::logic_error& e)
            {
                std::cerr << e.what() << "\nhello\n";
            }
            catch(const std::runtime_error& e)
            {
                std::cerr << e.what() << "\nbye bye\n";
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "\nthis is fucking shit\n";
        }
    // child a;
    // auto c = Contract(runtime_builder,
    //                   pre(falser, "pre falser", 80),
    //                   Condition(falser, "falser pre", 69, precondition));
    // if(auto error = c.check(precondition); error.has_value())
    // {
    //   std::cout << error.value().second << '\n';
    //   std::cout << error.value().first << '\n';
    // }
}