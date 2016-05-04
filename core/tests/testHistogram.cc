#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"


TEST(Histogram1, construction)
{
    using namespace milan;
    
    typedef Histogram<float32,1> Histogram1F;
    typedef Binning<float32> BinningF;
    
    sizetype N = 10;
    Histogram1F hist{{BinningF(N,-1,1)}};
    EXPECT_EQ(hist.getBinning(0).size(),N);
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_FLOAT_EQ(hist.getContent({i}),0);
        EXPECT_FLOAT_EQ(hist.getError({i}),0);
        
        hist.setContent({i},i*0.1);
        hist.setError({i},i*0.2);
        EXPECT_FLOAT_EQ(hist.getError({i}),i*0.2);
        hist.setError2({i},i*0.2);
    }
    
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_FLOAT_EQ(hist.getContent({i}),i*0.1);
        EXPECT_FLOAT_EQ(hist.getError({i}),std::sqrt(i*0.2));
        EXPECT_FLOAT_EQ(hist.getError2({i}),i*0.2);
    }
}

TEST(Histogram2, construction)
{
    using namespace milan;
    
    typedef Histogram<float32,2> Histogram1F;
    typedef Binning<float32> BinningF;
    
    sizetype N = 10;
    Histogram1F hist{{BinningF(N,-1,1),BinningF(N*2,-1,1)}};
    EXPECT_EQ(hist.getBinning(0).size(),N);
    EXPECT_EQ(hist.getBinning(1).size(),N*2);
    for (sizetype i = 0; i < N+2; ++i)
    {
        for (sizetype j = 0; j < N*2+2; ++j)
        {
            EXPECT_FLOAT_EQ(hist.getContent({i,j}),0);
            EXPECT_FLOAT_EQ(hist.getError({i,j}),0);
            
            hist.setContent({i,j},i*0.1+j*0.3);
            hist.setError({i,j},i*0.2+j*0.7);
            EXPECT_FLOAT_EQ(hist.getError({i,j}),i*0.2+j*0.7);
            hist.setError2({i,j},i*0.2+j*0.7);
        }
    }
    
    for (sizetype i = 0; i < N; ++i)
    {
        for (sizetype j = 0; j < N*2+2; ++j)
        {
            EXPECT_FLOAT_EQ(hist.getContent({i,j}),i*0.1+j*0.3);
            EXPECT_FLOAT_EQ(hist.getError({i,j}),std::sqrt(i*0.2+j*0.7));
            EXPECT_FLOAT_EQ(hist.getError2({i,j}),i*0.2+j*0.7);
        }
    }
}

