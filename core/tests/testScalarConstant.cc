#include "milan/core/ScalarConstant.hh"
#include "milan/core/Parameter.hh"
#include "gtest/gtest.h"

#include "test_macros.h"

TEST(ScalarConstant, construction)
{
    using namespace milan;
    for (sizetype i = 0; i <10; ++i)
    {
        ScalarConstant x = 0.5*i-2;
        Parameter fakeParameter("fake");
        
        EXPECT_DOUBLE_EQ(x.getValue(),0.5*i-2);
        EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
        EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],0.5*i-2);
        EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],0.5*i-2);
        EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
    }
    
}

TEST(ScalarConstant, add)
{
    using namespace milan;
    for (sizetype i = 0; i <10; ++i)
    {
        for (sizetype j = 0; j <10; ++j)
        {
            
            const double a = 0.5*i-2;
            const double b = 0.11*j-0.05;
            const double result = a+b;
            
            ScalarConstant x = a;
            x+=b;
            Parameter fakeParameter("fake");
            
            EXPECT_DOUBLE_EQ(x.getValue(),result);
            EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
        }
    }
}

TEST(ScalarConstant, sub)
{
    using namespace milan;
    for (sizetype i = 0; i <10; ++i)
    {
        for (sizetype j = 0; j <10; ++j)
        {
            const double a = 0.5*i-2;
            const double b = 0.11*j-0.05;
            const double result = a-b;
            
            ScalarConstant x = a;
            x-=b;
            Parameter fakeParameter("fake");
            
            EXPECT_DOUBLE_EQ(x.getValue(),result);
            EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
        }
    }
}

TEST(ScalarConstant, mul)
{
    using namespace milan;
    for (sizetype i = 0; i <10; ++i)
    {
        for (sizetype j = 0; j <10; ++j)
        {
            const double a = 0.5*i-2;
            const double b = 0.11*j-0.05;
            const double result = a*b;
            
            ScalarConstant x = a;
            x*=b;
            Parameter fakeParameter("fake");
            
            EXPECT_APPROX_FLOAT_EQ(x.getValue(),result);
            EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
            EXPECT_APPROX_FLOAT_EQ(x.getValueAndDerivatives({})[0],result);
            EXPECT_APPROX_FLOAT_EQ(x.getValueAndDerivatives({fakeParameter})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
        }
    }
}


TEST(ScalarConstant, div)
{
    using namespace milan;
    for (sizetype i = 0; i <10; ++i)
    {
        for (sizetype j = 0; j <10; ++j)
        {
            const double a = 0.5*i-2;
            const double b = 0.11*j-0.05;
            const double result = a/b;
            
            ScalarConstant x = a;
            x/=b;
            Parameter fakeParameter("fake");
            
            EXPECT_DOUBLE_EQ(x.getValue(),result);
            EXPECT_DOUBLE_EQ(x.getDifferential(fakeParameter),0);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[0],result);
            EXPECT_DOUBLE_EQ(x.getValueAndDerivatives({fakeParameter})[1],0);
        }
    }
}
