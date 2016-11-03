#include "milan/core/Binning.hh"

#include "gtest/gtest.h"


#include <limits>

TEST(Binning, bin_info)
{
    using namespace milan;
    
    Binning b(10,-1,1);
    EXPECT_EQ(b.size(),10);
    
    EXPECT_DOUBLE_EQ(b.getBinLowerEdge(1),-1);
    EXPECT_DOUBLE_EQ(b.getBinLowerEdge(10),0.8);
    EXPECT_DOUBLE_EQ(b.getBinLowerEdge(11),1);
    
    EXPECT_DOUBLE_EQ(b.getBinUpperEdge(0),-1);
    EXPECT_DOUBLE_EQ(b.getBinUpperEdge(1),-0.8);
    EXPECT_DOUBLE_EQ(b.getBinUpperEdge(10),1);
    
    EXPECT_DOUBLE_EQ(b.getBinCenter(1),-0.9);
    EXPECT_DOUBLE_EQ(b.getBinCenter(10),0.9);
    
    EXPECT_DOUBLE_EQ(b.getBinWidth(1),0.2);
    EXPECT_DOUBLE_EQ(b.getBinWidth(10),0.2);
}

TEST(Binning, wrong_binning)
{
    using namespace milan;
    
    EXPECT_NO_THROW(Binning(1,1,2));
    EXPECT_NO_THROW(Binning(1,0,1));
    EXPECT_NO_THROW(Binning(1,-1,1));
    EXPECT_NO_THROW(Binning(1,-2,-1));
    
    EXPECT_NO_THROW(Binning(10,1,2));
    EXPECT_NO_THROW(Binning(10,0,1));
    EXPECT_NO_THROW(Binning(10,-1,1));
    EXPECT_NO_THROW(Binning(10,-2,-1));
    
    EXPECT_ANY_THROW(Binning(0,1,2));
    EXPECT_ANY_THROW(Binning(0,0,1));
    EXPECT_ANY_THROW(Binning(0,-1,1));
    EXPECT_ANY_THROW(Binning(0,-2,-1));
    
    EXPECT_ANY_THROW(Binning(1,2,1));
    EXPECT_ANY_THROW(Binning(1,1,0));
    EXPECT_ANY_THROW(Binning(1,1,-1));
    EXPECT_ANY_THROW(Binning(1,-1,-2));
    
    EXPECT_ANY_THROW(Binning(10,2,1));
    EXPECT_ANY_THROW(Binning(10,1,0));
    EXPECT_ANY_THROW(Binning(10,1,-1));
    EXPECT_ANY_THROW(Binning(10,-1,-2));
    
    EXPECT_NO_THROW(Binning({10,11,12,13}));
    EXPECT_NO_THROW(Binning({10,10.5,12,13}));
    EXPECT_NO_THROW(Binning({10,10.5,10.6,10.7}));
    
    EXPECT_ANY_THROW(Binning({10,10,11,12,13}));
    EXPECT_ANY_THROW(Binning({10,11,10.5,12,13}));
    
    EXPECT_ANY_THROW(Binning({10,11,11,12,13}));
    EXPECT_ANY_THROW(Binning({10,12,11.5,10,13}));
    
    EXPECT_ANY_THROW(Binning({10,11,12,13.5,13}));
    EXPECT_ANY_THROW(Binning({10,11,13,12,13.5})); 
    
    EXPECT_ANY_THROW(Binning({}));
}

TEST(Binning, compare)
{
    using namespace milan;
    
    EXPECT_TRUE(Binning(1,1,2)==Binning(1,1,2));
    EXPECT_TRUE(Binning(1,1.0,2)==Binning(1,1,2));
    EXPECT_TRUE(Binning(1,1,2)==Binning(1,1,2.0));
    EXPECT_TRUE(Binning(1,1.0,2.0)==Binning(1,1,2));
    EXPECT_TRUE(Binning(1,1.0,2.0)==Binning(1,1.0,2));
    EXPECT_TRUE(Binning(1,1.0,2.0)==Binning(1,1.0,2.0));
    
    EXPECT_TRUE(Binning(10,1,2)==Binning(10,1,2));
    EXPECT_TRUE(Binning(10,1.0,2)==Binning(10,1,2));
    EXPECT_TRUE(Binning(10,1,2)==Binning(10,1,2.0));
    EXPECT_TRUE(Binning(10,1.0,2.0)==Binning(10,1,2));
    EXPECT_TRUE(Binning(10,1.0,2.0)==Binning(10,1.0,2));
    EXPECT_TRUE(Binning(10,1.0,2.0)==Binning(10,1.0,2.0));
    
    EXPECT_TRUE(Binning({10,11,12,13})==Binning({10,11,12,13}));
    EXPECT_TRUE(Binning({10.0,11,12,13})==Binning({10,11,12,13}));
    EXPECT_TRUE(Binning({10.0,11.0,12,13})==Binning({10,11,12,13}));
    EXPECT_TRUE(Binning({10.0,11,12.0,13})==Binning({10,11,12,13}));
    EXPECT_TRUE(Binning({10.0,11.0,12.0,13.0})==Binning({10,11,12,13}));
    
    EXPECT_FALSE(Binning(10,1+std::numeric_limits<double>::epsilon()*2,2)==Binning(10,1,2));
    EXPECT_FALSE(Binning(1,1+std::numeric_limits<double>::epsilon()*2,2)==Binning(10,1,2));
    EXPECT_FALSE(Binning(10,1+std::numeric_limits<double>::epsilon()*2,2+std::numeric_limits<double>::epsilon()*2)==Binning(10,1,2));
    EXPECT_FALSE(Binning(1,1+std::numeric_limits<double>::epsilon()*2,2+std::numeric_limits<double>::epsilon()*2)==Binning(10,1,2));
}

TEST(Binning,find)
{
    using namespace milan;
    
    Binning b(10,0,1);
    
    EXPECT_EQ(b.findBin(-0.001),0);
    
    EXPECT_EQ(b.findBin(0.00),1);
    EXPECT_EQ(b.findBin(0.0001),1);
    EXPECT_EQ(b.findBin(0.0999),1);
    EXPECT_EQ(b.findBin(0.10),2);
    EXPECT_EQ(b.findBin(0.1001),2);
    EXPECT_EQ(b.findBin(0.1999),2);
    EXPECT_EQ(b.findBin(0.20),3);
    EXPECT_EQ(b.findBin(0.2001),3);
    EXPECT_EQ(b.findBin(0.2999),3);
    EXPECT_EQ(b.findBin(0.30),4);
    EXPECT_EQ(b.findBin(0.3001),4);
    EXPECT_EQ(b.findBin(0.3999),4);
    
    EXPECT_EQ(b.findBin(0.80),9);
    EXPECT_EQ(b.findBin(0.8001),9);
    EXPECT_EQ(b.findBin(0.90),10);
    EXPECT_EQ(b.findBin(0.9001),10);
    EXPECT_EQ(b.findBin(0.9999),10);
    
    EXPECT_EQ(b.findBin(1.0),11);
    EXPECT_EQ(b.findBin(1.0001),11);
    
    for (sizetype ibin = 0; ibin < b.size(); ++ibin)
    {
        EXPECT_EQ(b.findBin(b.getBinCenter(ibin+1)),ibin+1);
        EXPECT_EQ(b.findBin(b.getBinLowerEdge(ibin+1)+std::numeric_limits<double>::epsilon()),ibin+1);
        EXPECT_EQ(b.findBin(b.getBinUpperEdge(ibin+1)-std::numeric_limits<double>::epsilon()),ibin+1);
        EXPECT_EQ(b.findBin(b.getBinUpperEdge(ibin+1)),ibin+2);
    }
}



