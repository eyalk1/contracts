#include "condition.hpp"

#include <boost/hana.hpp>
#include <stdexcept>
#include <sstream>
#include <experimental/source_location>

#define TCONTRACT(...) ThrowingContract(std::experimental::source_location::current(),&GenerateException,__VA_ARGS__)
#define TCONTRACT_EG(...) ThrowingContract(std::experimental::source_location::current(),__VA_ARGS__)



std::runtime_error GenerateException(std::experimental::source_location loc, std::string_view failed){
    std::stringstream ss;
    ss <<  "condition not met at entrance to the function " << loc.function_name() << " .the error is: " << failed;
    return std::runtime_error(ss.str());
}



template<typename ExceptionGen, typename... conditions>
struct ThrowingContract
{
    ThrowingContract(std::experimental::source_location _location, ExceptionGen _eg, conditions... _conditions);
    ~ThrowingContract();

    void go_over(cond_type filt);

    ExceptionGen eg;
    std::experimental::source_location location;
    boost::hana::tuple<conditions...> const m_conditions;
};


/*****************IMPLEMENTATION*****************/


template<typename ExceptionGen, typename... conditions>
void ThrowingContract<ExceptionGen, conditions...>::go_over(cond_type filt){
    boost::hana::for_each(m_conditions, [this, filt](auto const & condition){
        if(condition.m_type != filt && !(condition.pred()))
            throw eg(this->location, condition.description);
    });
}


template<typename ExceptionGen, typename... conditions>
ThrowingContract<ExceptionGen, conditions...>::ThrowingContract(std::experimental::source_location _location, ExceptionGen _eg, conditions... _conditions):
    m_conditions(_conditions...), location(_location), eg(_eg) {
        go_over(cond_type::POST);
}

template<typename ExceptionGen, typename... conditions>
ThrowingContract<ExceptionGen, conditions...>::~ThrowingContract(){
    go_over(cond_type::PRE);
}