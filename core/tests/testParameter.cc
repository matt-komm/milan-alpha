#include "milan/core/Parameter.hh"

#include "gtest/gtest.h"

TEST(Parameter, construction)
{
    using namespace milan;
    
    Parameter p1("test");
    EXPECT_EQ(p1.getValue(),0);
    p1.setValue(1);
    EXPECT_EQ(p1.getValue(),1);
}
