#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Parameter.hh"

#include "gtest/gtest.h"

#include <memory>
/*
TEST(HistogramFunction, clone)
{
    using namespace milan;
    typedef HistogramInterface<1> HistogramInterface1D;
    typedef Histogram<1> Histogram1D;
    
    
    Histogram1D hist({Binning(50,-1,1)});
    EXPECT_EQ(hist.getContentValue({1}),0);
    hist.setContent({1},1.0);
    EXPECT_EQ(hist.getContentValue({1}),1);
    
    std::shared_ptr<const HistogramInterface1D> histClone = hist.clone();
    EXPECT_EQ(hist.getContentValue({1}),1);
    EXPECT_EQ(histClone->get().getContentValue({1}),1);
    
    hist.setContent({1},2.0);
    EXPECT_EQ(hist.getContentValue({1}),2);
    EXPECT_EQ(histClone->get().getContentValue({1}),1);
    
}
*/
TEST(HistogramFunction, assign)
{
    using namespace milan;
    typedef HistogramFunction HistogramFunction1D;
    typedef Histogram Histogram1D;
    
    
    Histogram1D hist1({Binning(50,-1,1)});
    hist1.setContent(1,1.0);
    
    //HistogramFunction1D histFct1 = Histogram1D({Binning(50,-1,1)});
    //EXPECT_EQ(histFct1.getHistogram().getContentValue({1}),0);
    
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent(1,2.0);

    HistogramFunction1D histFct1 = hist1.ref();
    HistogramFunction1D histFct2 = hist2;
    
    EXPECT_DOUBLE_EQ(histFct1.getContentValue(1),1);
    EXPECT_DOUBLE_EQ(histFct2.getContentValue(1),2);
    
    HistogramFunction1D histFct3 = histFct1+histFct2;
    EXPECT_DOUBLE_EQ(histFct3.getContentValue(1),3);
    
    HistogramFunction1D histFct4 = histFct1+histFct2+histFct1;
    
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),4);
    hist1.setContent(1,3.0);
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),8);
    
    histFct4 = histFct1+histFct4;
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),11);
    
    Parameter p1("p1",2);
    Parameter p2("p1",3);
    histFct4 = histFct4*p1;
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),22);
    histFct4 = histFct4*p1*p2;
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),132);
    
    p1.setValue(1);
    EXPECT_DOUBLE_EQ(histFct4.getContentValue(1),33);
    
}


TEST(HistogramFunction, precendence)
{
    //acutally, operator precedence as in C++ is used. No real need to care about a bug here.
    using namespace milan;
    typedef HistogramFunction HistogramFunction1D;
    typedef Histogram Histogram1D;
    
    
    Histogram1D hist1({Binning(50,-1,1)});
    hist1.setContent(1,1.0);
    
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent(1,2.0);
    
    Parameter p1("p1",2);

    HistogramFunction1D histFct1 = hist1.ref();
    HistogramFunction1D histFct2 = hist2.ref();
    
    HistogramFunction1D result1 = histFct1+histFct2*p1;
    EXPECT_DOUBLE_EQ(result1.getContentValue(1),5);
    
    HistogramFunction1D result2 = histFct1*p1+histFct2;
    EXPECT_DOUBLE_EQ(result2.getContentValue(1),4);
    
    HistogramFunction1D result3 = (histFct1+histFct2)*p1;
    EXPECT_DOUBLE_EQ(result3.getContentValue(1),6);
}


TEST(HistogramFunction, diff)
{
    using namespace milan;
    typedef HistogramFunction HistogramFunction1D;
    typedef Histogram Histogram1D;
    
    
    Histogram1D hist1({Binning(50,-1,1)});
    hist1.setContent(1,1.0);
    
    Histogram1D hist2({Binning(50,-1,1)});
    hist2.setContent(1,2.0);
    
    Parameter p1("p1",3);

    HistogramFunction1D histFct1 = hist1.ref();
    HistogramFunction1D histFct2 = hist2.ref();
    
    HistogramFunction1D result1 = histFct1+histFct2*p1;
    
    Ftype result = result1.getContentFtype(1);
    EXPECT_DOUBLE_EQ(p1.differentiate(),2);
    /*
    //1+2*p
    EXPECT_DOUBLE_EQ(result1.getDifferential(1,p1),2);
    
    HistogramFunction1D result2 = histFct1*p1+histFct2; //1*p+2
    EXPECT_DOUBLE_EQ(result2.getDifferential(1,p1),1);
    
    HistogramFunction1D result3 = histFct1*p1+histFct2*p1; //1*p+2*p
    EXPECT_DOUBLE_EQ(result3.getDifferential(1,p1),3);
    
    HistogramFunction1D result4 = histFct1*p1+histFct2*p1*p1; //1*p+2*p*p
    EXPECT_DOUBLE_EQ(result4.getDifferential(1,p1),13); //-> 1 + 2*2*p = 1+2*2*3=13
    */
}
