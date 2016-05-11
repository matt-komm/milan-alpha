#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"

#include "gtest/gtest.h"


TEST(HistogramFunction, assign)
{
    using namespace milan;
    typedef HistogramFunction<1> HistogramFunction1D;
    typedef Histogram<1> Histogram1D;
    
    HistogramFunction1D histFct1 = Histogram1D({Binning(50,-1,1)});
    EXPECT_EQ(histFct1.getHistogram().getContent({1}),0);
    
    Histogram1D other({Binning(50,-1,1)});
    HistogramFunction1D histFct2 = other;
    EXPECT_EQ(histFct2.getHistogram().getContent({1}),0);
    
    HistogramFunction1D addHist1 = histFct1 + other;
    EXPECT_EQ(addHist1.getHistogram().getContent({1}),0);
    
    HistogramFunction1D addHist2 = histFct1 + histFct2;
    EXPECT_EQ(addHist2.getHistogram().getContent({1}),0);
}


