#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"

#include "gtest/gtest.h"

TEST(HistogramFunction, clone)
{
    using namespace milan;
    typedef HistogramInterface<1> HistogramInterface1D;
    typedef Histogram<1> Histogram1D;
    
    
    Histogram1D hist({Binning(50,-1,1)});
    EXPECT_EQ(hist.getContent({1}),0);
    hist.setContent({1},1.0);
    EXPECT_EQ(hist.getContent({1}),1);
    
    std::shared_ptr<const HistogramInterface1D> histClone = hist.clone();
    EXPECT_EQ(hist.getContent({1}),1);
    EXPECT_EQ(histClone->get().getContent({1}),1);
    
    hist.setContent({1},2.0);
    EXPECT_EQ(hist.getContent({1}),2);
    EXPECT_EQ(histClone->get().getContent({1}),1);
    
}

TEST(HistogramFunction, assign)
{
    using namespace milan;
    typedef HistogramFunction<1> HistogramFunction1D;
    typedef Histogram<1> Histogram1D;
    
    
    Histogram1D hist1({Binning(50,-1,1)});
    hist1.setContent({1},1.0);
    
    //HistogramFunction1D histFct1 = Histogram1D({Binning(50,-1,1)});
    //EXPECT_EQ(histFct1.getHistogram().getContent({1}),0);
    
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent({1},2.0);

    HistogramFunction1D histFct1 = hist1;
    HistogramFunction1D histFct2 = hist2;
    
    EXPECT_EQ(histFct1.get().getContent({1}),1);
    EXPECT_EQ(histFct2.get().getContent({1}),2);
    
    HistogramFunction1D histFct3 = histFct1+histFct2;
    EXPECT_EQ(histFct3.get().getContent({1}),3);
    
    HistogramFunction1D histFct4 = histFct1+histFct2+histFct1;
    Histogram1D result = histFct4.get();
    EXPECT_EQ(result.getContent({1}),4);
    
    
    ///HistogramFunction1D hist = Histogram1D();
    
    /*
    
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent({1},2.0);
    
    HistogramFunction1D histFct3 = hist2;
    EXPECT_EQ(histFct3.getHistogram().getContent({1}),2);
    
    HistogramFunction1D addHist1 = histFct2 + hist1;
    EXPECT_EQ(addHist1.getHistogram().getContent({1}),2);
    
    HistogramFunction1D addHist2 = histFct2 + histFct3;
    EXPECT_EQ(addHist2.getHistogram().getContent({1}),3);
    
    HistogramFunction1D addHist3 = histFct1 + histFct2 + histFct3;
    EXPECT_EQ(addHist3.getHistogram().getContent({1}),3);
    
    addHist3+=addHist3;
    EXPECT_EQ(addHist3.getHistogram().getContent({1}),3);
    */
}


