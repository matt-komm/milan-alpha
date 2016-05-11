#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"

#include "gtest/gtest.h"


TEST(HistogramFunction, assign)
{
    using namespace milan;
    typedef HistogramFunction<1> HistogramFunction1D;
    typedef Histogram<1> Histogram1D;
    
    HistogramFunction1D histFct = Histogram1D({Binning(50,-1,1)});
    EXPECT_EQ(histFct.getHistogram().getContent({1}),0);
    
    Histogram1D other({Binning(50,-1,1)});
    HistogramFunction1D histFct1 = other;
    EXPECT_EQ(histFct1.getHistogram().getContent({1}),0);
}


