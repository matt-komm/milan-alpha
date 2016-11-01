#include "adept/adept.h"
#include "gtest/gtest.h"

#include <iostream>

typedef adept::adouble DiffType;

DiffType myFct(DiffType x)
{
    return x*x + fast_log(x);
}

TEST(adept,diff)
{
    adept::Stack stack;
    DiffType x = 2;
    stack.new_recording();
    DiffType res = myFct(x);
    res.set_gradient(1.0);
    stack.compute_adjoint();
    
    std::cout<<"value="<<res.value()<<", diff="<<x.get_gradient()<<std::endl;
}

