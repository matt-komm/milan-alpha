#include "milan/core/Binning.hh"

#include "gtest/gtest.h"


TEST(Binning, fixed)
{
    using namespace milan;
    
    Binning b(10,-1,1);
    EXPECT_EQ(b.size(),10);
    
    EXPECT_DOUBLE_EQ(b.getBinLowerEdge(0),-1);
    EXPECT_DOUBLE_EQ(b.getBinLowerEdge(9),0.8);
    
    EXPECT_DOUBLE_EQ(b.getBinUpperEdge(0),-0.8);
    EXPECT_DOUBLE_EQ(b.getBinUpperEdge(9),1);
    
    EXPECT_DOUBLE_EQ(b.getBinCenter(0),-0.9);
    EXPECT_DOUBLE_EQ(b.getBinCenter(9),0.9);
    
    EXPECT_DOUBLE_EQ(b.getBinWidth(0),0.2);
    EXPECT_DOUBLE_EQ(b.getBinWidth(9),0.2);
}

TEST(Binning, vector)
{
    using namespace milan;
    
    EXPECT_ANY_THROW(Binning({11,11,12}));
    EXPECT_ANY_THROW(Binning({12,11,10}));
    EXPECT_ANY_THROW(Binning({11,12,12}));
    EXPECT_ANY_THROW(Binning({11,12,11}));
    
    Binning b({10,11,12});
    EXPECT_EQ(b.size(),2);
    
    
}
