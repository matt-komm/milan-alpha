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
    
    Histogram1D hist1({Binning(50,-1,1)});
    hist1.setContent({1},1.0);
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent({1},2.0);
    HistogramFunction1D histFct2 = hist1;
    EXPECT_EQ(histFct2.getHistogram().getContent({1}),1);
    
    HistogramFunction1D histFct3 = hist2;
    EXPECT_EQ(histFct3.getHistogram().getContent({1}),2);
    
    HistogramFunction1D addHist1 = histFct2 + hist1;
    EXPECT_EQ(addHist1.getHistogram().getContent({1}),2);
    
    HistogramFunction1D addHist2 = histFct2 + histFct3;
    EXPECT_EQ(addHist2.getHistogram().getContent({1}),3);
}


