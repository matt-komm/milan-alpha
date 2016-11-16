#include "milan/core/Function.hh"

#include "gtest/gtest.h"

TEST(Function, simple)
{
    using namespace milan;
    Parameter p1("mu");
    Parameter p2("bkg");
    Function fct1 = p1.ref();
    for (sizetype i = 0; i < 10; ++i)
    {
        const double value = i*0.2-1.0;
        p1.setValue(value);
        EXPECT_DOUBLE_EQ(fct1.getValue(),value);
        EXPECT_DOUBLE_EQ(fct1.getDerivative(p1),1.0);
        EXPECT_DOUBLE_EQ(fct1.getDerivative(p2),0.0);
    }
}

