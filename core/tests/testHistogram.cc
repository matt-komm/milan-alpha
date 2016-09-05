#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"


TEST(Histogram1D, copy)
{
    using namespace milan;
    sizetype N = 10;
    Histogram hist({Binning(N,-1,1)});
    for (sizetype i = 0; i < N+2; ++i)
    {
        hist.setContent({i},i*0.1);
        hist.setError({i},i*0.2);
    }
    Histogram hist_copy(hist);
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_DOUBLE_EQ(hist_copy.getContent({i}),i*0.1);
        EXPECT_DOUBLE_EQ(hist_copy.getError({i}),i*0.2);
    }
    for (sizetype i = 0; i < N+2; ++i)
    {
        hist.setContent({i},i*0.3);
        hist.setError({i},i*0.4);
        EXPECT_DOUBLE_EQ(hist_copy.getContent({i}),i*0.1);
        EXPECT_DOUBLE_EQ(hist_copy.getError({i}),i*0.2);
    }
    for (sizetype i = 0; i < N+2; ++i)
    {
        hist_copy.setContent({i},i*0.5);
        hist_copy.setError({i},i*0.6);
        EXPECT_DOUBLE_EQ(hist.getContent({i}),i*0.3);
        EXPECT_DOUBLE_EQ(hist.getError({i}),i*0.4);
    }
}

TEST(Histogram1D, move)
{
    using namespace milan;
    sizetype N = 10;
    Histogram hist({Binning(N,-1,1)});
    for (sizetype i = 0; i < N+2; ++i)
    {
        hist.setContent({i},i*0.1);
        hist.setError({i},i*0.2);
    }
    Histogram hist_move(std::move(hist));
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_DOUBLE_EQ(hist_move.getContent({i}),i*0.1);
        EXPECT_DOUBLE_EQ(hist_move.getError({i}),i*0.2);
    }
}

TEST(Histogram1D, construction)
{
    using namespace milan;
    
    
    sizetype N = 10;
    Histogram hist({Binning(N,-1,1)});
    EXPECT_EQ(hist.getBinning(0).size(),N);
    hist.getContent({0});
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_DOUBLE_EQ(hist.getContent({i}),0);
        EXPECT_DOUBLE_EQ(hist.getError({i}),0);
        
        hist.setContent({i},i*0.1);
        hist.setError({i},i*0.2);
        EXPECT_DOUBLE_EQ(hist.getError({i}),i*0.2);
        hist.setError2({i},i*0.2);
    }
    
    for (sizetype i = 0; i < N+2; ++i)
    {
        
        EXPECT_DOUBLE_EQ(hist.getContent({i}),i*0.1);
        EXPECT_DOUBLE_EQ(hist.getError({i}),std::sqrt(i*0.2));
        EXPECT_DOUBLE_EQ(hist.getError2({i}),i*0.2);
        
    }
}

TEST(Histogram2D, construction)
{
    using namespace milan;

    sizetype N = 10;
    Histogram hist({Binning(N,-1,1),Binning(N*2,-1,1)});
    EXPECT_EQ(hist.getBinning(0).size(),N);
    EXPECT_EQ(hist.getBinning(1).size(),N*2);

    for (sizetype i = 0; i < N+2; ++i)
    {
        for (sizetype j = 0; j < N*2+2; ++j)
        {
            EXPECT_DOUBLE_EQ(hist.getContent({i,j}),0);
            EXPECT_DOUBLE_EQ(hist.getError({i,j}),0);
            
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
            EXPECT_DOUBLE_EQ(hist.getContent({i,j}),i*0.1+j*0.3);
            EXPECT_DOUBLE_EQ(hist.getError({i,j}),std::sqrt(i*0.2+j*0.7));
            EXPECT_DOUBLE_EQ(hist.getError2({i,j}),i*0.2+j*0.7);
        }
    }
}

TEST(Histogram1D, global_binning)
{
    using namespace milan;
    
    sizetype N = 10;
    Histogram h({Binning(N,-1,1)});
    for (sizetype i = 0; i < N+2; ++i)
    {
        h.setContent({i},i);
    }
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_DOUBLE_EQ(h.getContent(i),i);
        h.setContent(i,2*i);
    }
    for (sizetype i = 0; i < N+2; ++i)
    {
        EXPECT_DOUBLE_EQ(h.getContent({i}),2*i);
    }
}

TEST(Histogram2D, global_binning)
{
    using namespace milan;
    
    sizetype N = 1;
    sizetype M = 2;
    Histogram h({Binning(N,-1,1),Binning(M,-1,1)});
    for (sizetype i = 0; i < N+2; ++i)
    {
        for (sizetype j = 0; j < M+2; ++j)
        {
            h.setContent({i,j},i*i+j);
        }
    }
    for (sizetype i = 0; i < N+2; ++i)
    {
        for (sizetype j = 0; j < M+2; ++j)
        {
            EXPECT_DOUBLE_EQ(h.getContent(j*(N+2)+i),i*i+j);
            h.setContent(j*(N+2)+i,2*i-j*j);
        }
    }
    
    for (sizetype i = 0; i < N+2; ++i)
    {
        for (sizetype j = 0; j < M+2; ++j)
        {
            EXPECT_DOUBLE_EQ(h.getContent({i,j}),2*i-j*j);
        }
    }
}

