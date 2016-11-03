#include "milan/algorithms/HMC.hh"

#include "gtest/gtest.h"

TEST(HMC, construction)
{
    using namespace milan;
    
    HMC hmc;
    Parameter p1("signal");
    Parameter p2("background");
    Parameter p3("background");
    
    EXPECT_EQ(hmc.getParameters().size(),0);
    hmc.addParameter(p1.ref());
    EXPECT_EQ(hmc.getParameters().size(),1);
    EXPECT_ANY_THROW(hmc.addParameter(p1.ref()));
    EXPECT_EQ(hmc.getParameters().size(),1);
    hmc.addParameter(p2.ref());
    EXPECT_EQ(hmc.getParameters().size(),2);
    EXPECT_ANY_THROW(hmc.addParameter(p3.ref()));
    EXPECT_EQ(hmc.getParameters().size(),2);
}
