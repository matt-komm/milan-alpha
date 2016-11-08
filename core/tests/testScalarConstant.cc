#include "milan/core/ScalarConstant.hh"
#include "milan/core/Parameter.hh"
#include "gtest/gtest.h"

TEST(ScalarConstant, construction)
{
    using namespace milan;
    ScalarConstant x = 5;
    Parameter fakeParameter("fake");
    
    EXPECT_DOUBLE_EQ(x.getValue(),5);
    EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],5);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],5);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
    
}
TEST(ScalarConstant, operators)
{
    using namespace milan;
    ScalarConstant x = 5;
    Parameter fakeParameter("fake");
    x+=10;
    EXPECT_DOUBLE_EQ(x.getValue(),15);
    EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],15);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],15);
    EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
}
